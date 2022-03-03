/**
******************************************************************************
 * @file                fes_wavegen.cpp
 * @author              Ahmed Hafeez-Baig
 * @date                14042021
 * @brief               Wave generation task
******************************************************************************
*/

// TODO get adc working

/* Include Header File */
#include "fes_wavegen.hpp"

#include "usart_reg.h"
#include <cstdio>

/* Macro Definitions */
static constexpr uint8_t DATA_BITS = 			100; // Databits per wave
static constexpr uint8_t NUMBER_OF_WAVES = 		8;  // Max number of waves
static constexpr uint8_t SWITCH_MATRIX_DELAY =	10; // SPI buffer (tics)

/* Struct Definitions */
typedef struct {
	bool envelopeOnOff = false;
	uint32_t rampIn;
	uint32_t rampOut;
	uint16_t amplitude;
	uint64_t startTime;
	uint64_t envelopeWidth;
} WaveEnvelope;

// TODO: move waveform class definition to new file

/* WaveForm Class Defintition */
class Waveform {

	private: /* Declarations */

		/* Parameter Values:
		* Frequency
		* Amplitude
		* Waveform
		* Infinite/Timed (aka, should waveform go on until stopped, or for a defined period)
		* Ramp Up/Down (both how much, and for how long)
		*/

		/* Private Variable Declarations */
		uint16_t waveData[DATA_BITS] = {0};
		WaveEnvelope envelopeData;

		/* Private Function Prototypes */
		inline void update_pulse_width_DAC_timer(void);

	public:

		/* Public Variables */
		static uint8_t waveNumberTracker;
		WaveGenStruct waveParams;
		bool readyForDAC = false;
		uint64_t nextTickVal = 0;

		/* Function Declarations */
		void generate_waveform(void);
		void wave_import(WaveGenStruct waveGenReceive);
		void update_period(uint64_t current);
		bool update_envelope(void);
		void update_output_wave(void);
		void update_amplitude(uint16_t adc_value);
};
// Initialise class static members
uint8_t Waveform::waveNumberTracker = 0;

/* Global Variables */
static uint8_t CurrentOutputWave;
static Waveform waves[NUMBER_OF_WAVES + 1];

/* Internal Function Prototypes */
void update_all_envelopes(void);
void update_all_periods(void);
void update_all_output_waves(void);

/* Task Function Definitions */

/**
 * @brief Initialise hardware and wavegen task
 * @param None
 * @retval None
 */
void fes_wavegen_task_init(void) {
	// Hardware Initialisations
	GPIO_Init();
	DMA_Init();
	DAC1_Init();
	ADC1_Init();
	TIM2_Init(100);
	TIM7_Init(); // currently unused

	// Start TIM7 for interrupt (currently unused)
	HAL_TIM_Base_Start_IT(&htim7);

	// Enable DAC
	DAC_Enable();

	uint16_t f = 250;
	waves[0].waveParams.waveType = PULSE_SINE;
	waves[0].waveParams.channel = CHANNEL_2;
	waves[0].waveParams.frequency = f;
	waves[0].waveParams.amplitude = 4000;
	waves[0].waveParams.width = 50;
	waves[0].waveParams.bipolar = false;
	waves[0].waveParams.interphaseDelay = 200;
	waves[0].generate_waveform();

	waves[1].waveParams.waveType = PULSE_SINE;
	waves[1].waveParams.channel = CHANNEL_3;
	waves[1].waveParams.frequency = f;
	waves[1].waveParams.amplitude = 2000;
	waves[1].waveParams.width = 100;
	waves[1].waveParams.bipolar = false;
	waves[1].waveParams.interphaseDelay = 0;
	waves[1].generate_waveform();
	
	waves[2].waveParams.waveType = PULSE_SINE;
	waves[2].waveParams.channel = CHANNEL_3;
	waves[2].waveParams.frequency = f;
	waves[2].waveParams.amplitude = 2000;
	waves[2].waveParams.width = 40;
	waves[0].waveParams.bipolar = false;
	waves[0].waveParams.interphaseDelay = 200;
	waves[2].generate_waveform();

	waves[3].waveParams.waveType = PULSE_SINE;
	waves[3].waveParams.channel = CHANNEL_4;
	waves[3].waveParams.frequency = f;
	waves[3].waveParams.amplitude = 2000;
	waves[3].waveParams.width = 40;
	waves[3].waveParams.bipolar = false;
	waves[3].waveParams.interphaseDelay = 200;
	waves[3].generate_waveform();

	waves[4].waveParams.waveType = PULSE_SINE;
	waves[4].waveParams.channel = CHANNEL_3;
	waves[4].waveParams.frequency = f;
	waves[4].waveParams.amplitude = 2000;
	waves[4].waveParams.width = 4;
	waves[4].waveParams.bipolar = false;
	waves[4].waveParams.interphaseDelay = 200;
	waves[4].generate_waveform();

	Waveform::waveNumberTracker = 4;
}

/**
 * @brief Function implementing the WavegenTask thread
 * @param argument: Not used
 * @retval None
 */
void fes_wavegen_task(void) {
	update_all_envelopes();
	update_all_periods();
	update_all_output_waves();
}

/* Internal Function Declerations */

/**
 * @brief Update waves with envelopes
 * @param None
 * @retval None
 */
void update_all_envelopes(void) {
	for (uint8_t i = 0; i < Waveform::waveNumberTracker; i++) {
		if (!waves[i].update_envelope()) {
			for (uint8_t j = i; j < Waveform::waveNumberTracker - 1; j++) {
					waves[j] = waves[j+1];
			}
			Waveform::waveNumberTracker--;
		}
	}
}

/**
 * @brief Enable flag on all waves that are ready
 * @param None
 * @retval None
 */
void update_all_periods(void) {
	uint64_t current = getCurrentMicros(); // capture time here so all wave updating occurs from common reference
	for (uint8_t i = 0; i < Waveform::waveNumberTracker; i++) {
		waves[i].update_period(current);
	}
}

/**
 * @brief Output to DAC when it is ready on required channels
 * @param None
 * @retval None
 */
void update_all_output_waves(void) {
	if (DAC_Ready()) {
		for (uint8_t i = 0; i < Waveform::waveNumberTracker; i++) {
			if (waves[i].readyForDAC) {
				// Output to DAC
				CurrentOutputWave = i;
				waves[i].update_output_wave();
				break;
			}
		}
	}
}

/* External Function Definitions */

/**
 * @brief
 * @param
 * @retval None
 */
void clear_wave(uint8_t clearChannel) {
	// Find channel and ensure it is cleared
	for (uint8_t i = 0; i < Waveform::waveNumberTracker; i++) {
		if (waves[i].waveParams.channel == clearChannel) {
			for (uint8_t j = i; j < Waveform::waveNumberTracker - 1; j++) {
				waves[j] = waves[j+1];
			}
			Waveform::waveNumberTracker--;
		}
	}
}

/**
 * @brief
 * @param
 * @retval None
 */
void new_wave(WaveGenStruct waveGenReceive) {
	waves[Waveform::waveNumberTracker].wave_import(waveGenReceive);
	Waveform::waveNumberTracker++;
}

/* Waveform Class Function Defintions */

/**
 * @brief 
 * @param 
 * @retval None
 */
void Waveform::generate_waveform(void) {
	// First use pulsewidth to determine how many bits will be used (shorter pulsewidth => fewer bits)
	if (waveParams.width > 50) {
		waveParams.waveDataBytes = DATA_BITS;
	} else if (waveParams.width > 10) {
		waveParams.waveDataBytes = 50;
	} else if (waveParams.width > 3) {
		waveParams.waveDataBytes = 20;
	} else {
		waveParams.waveDataBytes = 8;
	}
	// Add wave to data array
	float B = M_PI / (waveParams.waveDataBytes - 1);
	for (uint16_t i = 0; i < waveParams.waveDataBytes; i++) {
		switch (waveParams.waveType) {
			case PULSE_SINE:
				waveData[i] = waveParams.amplitude * std::sin((i) * B);
				break;
			case PULSE_SQUARE:
				waveData[i] = waveParams.amplitude;
				break;
			case PULSE_TRIANGLE:
				waveData[i] = (uint16_t)(waveParams.amplitude - (2 * std::abs((waveParams.amplitude / 2) - (waveParams.amplitude * (waveParams.waveDataBytes - i - 1) / waveParams.waveDataBytes))));
				break;
			case PULSE_SAWTOOTH:
				waveData[i] = waveParams.amplitude - (waveParams.amplitude * (waveParams.waveDataBytes - i - 1) / waveParams.waveDataBytes);
				break;
		}
		waveData[waveParams.waveDataBytes - 1] = 0; // Ensure post-wave DAC output is 0
	}
}

/**
 * @brief 
 * @param 
 * @retval None
 */
void Waveform::update_period(uint64_t current) {
	// Only check if period finished if the wave has finished its
	// previous period (aka, readyForDAC is false)
	//if (!readyForDAC && getCurrentMicros() >= nextTickVal) {
	if (current >= nextTickVal) {
		// Update Period
		nextTickVal += 1000000UL / waveParams.frequency;

		// Set DAC Flag
		readyForDAC = true;

		// Immediately check if period is behind. If it is *still* behind, this
		// implies the wave has not run for a long time; in which case we don't
		// let the wave run as it will immediate run again after this
		if (current >= nextTickVal) {
			readyForDAC = false;
		}
	}
}

/**
 * @brief 
 * @param 
 * @retval None
 */
void Waveform::update_output_wave(void) {
	//uint16_t switchState = (1 << (waveParams.channel + 1));
	//SPI_master_send(&switchState, 1);
	// Change channel (SPI/DMA)
	/*if (waveParams.channel == CHANNEL_2) {
		uint16_t switchState = (1 << waveParams.channel) | (1<<CHANNEL_15);
		SPI_master_send(&switchState, 1);
	} else {
		uint16_t switchState = (1 << waveParams.channel);
		SPI_master_send(&switchState, 1);
	}//*/

	// Set the frequency
	update_pulse_width_DAC_timer();

	// Delay to allow SPI to finish
	//uint64_t bufferTimer = getCurrentMicros();
	//while (bufferTimer + SWITCH_MATRIX_DELAY > getCurrentMicros()) {};
	
	while (!DAC_Ready()) {}; // Wait incase the DAC not ready
	DAC_Start(reinterpret_cast<uint32_t*>(waveData), waveParams.waveDataBytes);

	if (waveParams.bipolar) {
		// Wait for interphasic delay
		uint64_t bufferTimer = getCurrentMicros();

		// Invert channels (bipolar)
		// TODO

		// Wait for interphasic delay
		while (bufferTimer + waveParams.interphaseDelay > getCurrentMicros()) {};

		while (!DAC_Ready()) {}; // Wait incase the DAC not ready
		DAC_Start(reinterpret_cast<uint32_t*>(waveData), waveParams.waveDataBytes);
	}

	// Clear DAC Flag
	readyForDAC = false;
}

/**
 * @brief 
 * @param 
 * @retval None
 */
void Waveform::wave_import(WaveGenStruct waveGenReceive) {
	// Import wave details
	waveParams.waveType = waveGenReceive.waveType;
	waveParams.channel = waveGenReceive.channel;
	waveParams.frequency = waveGenReceive.frequency;
	waveParams.amplitude = waveGenReceive.amplitude;
	waveParams.width = waveGenReceive.width;
	// Import biphasic wave details
	waveParams.bipolar = waveGenReceive.bipolar;
	waveParams.interphaseDelay = waveGenReceive.interphaseDelay;
	// Import ramp/envelope details
	envelopeData.envelopeOnOff = waveGenReceive.envelopeOnOff;
	envelopeData.amplitude = waveGenReceive.amplitude;
	envelopeData.rampIn = waveGenReceive.rampIn * (SystemCoreClock / 1000); // convert ms to ticks
	envelopeData.rampOut = waveGenReceive.rampOut * (SystemCoreClock / 1000); // convert ms to ticks
	envelopeData.envelopeWidth = waveGenReceive.envelopeWidth * (SystemCoreClock / 1000); // convert ms to ticks
	envelopeData.startTime = HAL_GetTick();
	// Update nextTickVal and generate wave
	nextTickVal = getCurrentMicros();
	generate_waveform();
}

/**
 * @brief 
 * @param 
 * @retval None
 */
bool Waveform::update_envelope(void) {
	// Check if envelope enabled; if not, exit
	if (!envelopeData.envelopeOnOff) {
		return true;
	}

	// Check if period is finished
	if (envelopeData.startTime + envelopeData.envelopeWidth < HAL_GetTick()) {
			return false;
	}
	// Calculate amplitude for start of trapezoidal envelope
	if (envelopeData.startTime + envelopeData.rampIn > HAL_GetTick()) {
		//waveParams.amplitude = envelopeData.amplitude / 2;
		waveParams.amplitude = ((static_cast<uint64_t>(HAL_GetTick()) - envelopeData.startTime) * envelopeData.amplitude) / envelopeData.rampIn;
	}
	// Calculate amplitude for end of trapezoidal envelope
	else if (envelopeData.startTime + envelopeData.envelopeWidth - envelopeData.rampOut < HAL_GetTick()) {
		//waveParams.amplitude = envelopeData.amplitude / 4;
		waveParams.amplitude = (((static_cast<uint64_t>(envelopeData.startTime) + envelopeData.envelopeWidth) - HAL_GetTick()) * envelopeData.amplitude)  / envelopeData.rampOut;
	} else {
		waveParams.amplitude = envelopeData.amplitude;
	}
	
	generate_waveform();
	return true;
}

/**
 * @brief Adpate the amplitude of a wave, based on its amplitude as determined by the ADC
 * @param 
 * @retval None
 */
void Waveform::update_amplitude(uint16_t adc_value) {
	// TODO: write code to adjust amplitude

	// End by re-generating waveform data
	generate_waveform();
}

/**
 * @brief 
 * @param 
 * @retval None
 */
inline void Waveform::update_pulse_width_DAC_timer(void) {
	// Set the frequency
	TIM2_Init(static_cast<int>(waveParams.width * 80.75 / waveParams.waveDataBytes));
}

/* Interrupt Callback Functions */

/**
 * @brief 
 * @param None
 * @retval None
 */
void TIM7_callback(void) {
	// TIM7 is not currently used for anything
	// Maybe used for wave updating in future
}

/**
 * @brief 
 * @param None
 * @retval None
 */
void DMA_transfer_interrupt_callback(void) {
	return;
	if (DAC_DMA_TC_flag_set()) {
		// ////// Full transfer complete //////
	} else if (DAC_DMA_HT_flag_set()) {
		// Call function to adjust current based on ADC reading
		waves[CurrentOutputWave].update_amplitude(ADC_get_value());
		// TEMPORARY
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		uint32_t adc_value = ADC_get_value();
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		char str[8];
		sprintf(str, "%lu\n\r", adc_value);
		HAL_UART_Transmit(&huart2, reinterpret_cast<unsigned char*>(str), 8, 1);
	}
}

