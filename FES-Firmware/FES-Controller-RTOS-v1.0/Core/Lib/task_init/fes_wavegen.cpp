/**
******************************************************************************
 * @file                fes_wavegen.cpp
 * @author              Ahmed Hafeez-Baig
 * @date                14042021
 * @brief               Wave generation task
******************************************************************************
*/

/* Include Header File */
#include "fes_wavegen.hpp"

/* Macro Definitions */
// Number of Databits to be used for the wave
static constexpr uint8_t DATA_BITS = 			50;
// How many waves supported by the hardware
static constexpr uint8_t NUMBER_OF_WAVES = 		8;
// Buffer to stop DAC outputting before switch matrix is done (number of ticks)
static constexpr uint8_t SWITCH_MATRIX_DELAY =	10;

/* Global Variables */
uint16_t adc_value;

/* Task Definition */
osThreadId_t wavegenTaskHandle;
const osThreadAttr_t wavegenTask_attributes = {
  .name = "wavegenTask",
  .stack_size = 128 * 10,
  .priority = (osPriority_t) osPriorityAboveNormal5
};

/* Thread-Safe Handle Declaration */
osMessageQueueId_t WaveQueueHandle; // Wave Gen Queue
const osMessageQueueAttr_t WaveQueue_attributes = {
  .name = "WaveQueue"
};
osMessageQueueId_t ChannelClearQueueHandle; // Channel Clear Queue
const osMessageQueueAttr_t ChannelClearQueue_attributes = {
  .name = "ChannelClearQueue"
};

/* Struct Definitions */
typedef struct {
	bool envelopeOnOff = false;
	uint32_t rampIn;
	uint32_t rampOut;
	uint16_t amplitude;
	uint32_t startTime;
	uint32_t envelopeWidth;
} WaveEnvelope;

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
		void push_pulse_width_to_DAC_timer(void);

	public:

		/* Public Variables */
		static uint8_t waveNumberTracker;
		WaveGenStruct waveParams;
		bool readyForDAC = false;
		uint32_t nextTickVal = 0;

		/* Function Declarations */
		void generate_waveform(void);
		void update_period(void);
		void output_wave(void);
		void wave_import(WaveGenStruct waveGenReceive);
		bool update_envelope(void);
};
// Initialise class static members
uint8_t Waveform::waveNumberTracker = 0;

/* Task Function Prototype */
static void wavegen_task(void *argument);

/* External Function Definitions */

/**
 * @brief Initialise hardware and wavegen task
 * @param None
 * @retval None
 */
void wavegen_task_init(void) {

	// Hardware Initialisations
	GPIO_Init();
	DAC_DMA_Init();
	DAC1_Init();
	TIM2_Init(618);
	SPI1_Init();
	ADC1_Init();

	USART2_UART_Init();
	
	// Create task
	wavegenTaskHandle = osThreadNew(wavegen_task, NULL, &wavegenTask_attributes);
}

/**
 * @brief
 * @param
 * @rNRESET, or is dependent on the power supply coming up?

On a scope or analyzer how wide is the reset pulse as built? Can you plot that against the supplies in the system?

What does the part do when it starts? Does it run at all? Does it bring up other clocks, or try to interact with external parts?

Are the capacitors and voltages at VCAP correct?etval None
 */
void add_to_wavegen_queue(WaveGenStruct waveGenTransmit) {
	// Push wave on queue
	osMessageQueuePut(WaveQueueHandle, &waveGenTransmit, 0U, 0U);
	osThreadYield();
}

/**
 * @brief
 * @param
 * @retval None
 */
void add_to_channel_clear_queue(uint8_t channel) {
	// Push wave on queue
	osMessageQueuePut(ChannelClearQueueHandle, &channel, 0U, 0U);
	osThreadYield();
}

/**
 * @brief
 * @param
 * @retval None
 */
void read_adc(void) {
	adc_value = Get_ADC();
}

/* Task Definition */

/**
 * @brief Function implementing the WavegenTask thread
 * @param argument: Not used
 * @retval None
 */
static void wavegen_task(void *argument) {
	/* Initialisation */

	// Queue Initialisations
	WaveQueueHandle = osMessageQueueNew(2, sizeof(WaveGenStruct),
			&WaveQueue_attributes);
	ChannelClearQueueHandle = osMessageQueueNew(2, sizeof(uint8_t),
			&ChannelClearQueue_attributes);

	// os Status
	osStatus_t status;

	// Queue receiving vars
	WaveGenStruct waveGenReceive; // wave receiving struct
	uint8_t clearChannel; // channel clearing receiving int

	// Create wave object
	Waveform waves[NUMBER_OF_WAVES + 1];

	// Enable the DAC before starting
	DAC_Enable();

	uint16_t f = 50;
	waves[0].waveParams.waveType = PULSE_SINE;
	waves[0].waveParams.channel = CHANNEL_2;
	waves[0].waveParams.frequency = f;
	waves[0].waveParams.amplitude = 3800;
	waves[0].waveParams.width = 3000;
	waves[0].generate_waveform();

	waves[1].waveParams.waveType = PULSE_SQUARE;
	waves[1].waveParams.channel = CHANNEL_3;
	waves[1].waveParams.frequency = f;
	waves[1].waveParams.amplitude = 2000;
	waves[1].waveParams.width = 100;
	waves[1].generate_waveform();

	waves[2].waveParams.waveType = PULSE_SAWTOOTH;
	waves[2].waveParams.channel = CHANNEL_3;
	waves[2].waveParams.frequency = f;
	waves[2].waveParams.amplitude = 2000;
	waves[2].waveParams.width = 100;
	waves[2].generate_waveform();

	waves[3].waveParams.waveType = PULSE_TRIANGLE;
	waves[3].waveParams.channel = CHANNEL_4;
	waves[3].waveParams.frequency = f;
	waves[3].waveParams.amplitude = 2000;
	waves[3].waveParams.width = 100;
	waves[3].generate_waveform();
	//*/
	Waveform::waveNumberTracker = 0;

	/* Infinite loop */
	for(;;) {
		// Get wave data from queue (if data available)
		status = osMessageQueueGet(WaveQueueHandle, &waveGenReceive, NULL, 1);
		if (status == osOK) {
			waves[Waveform::waveNumberTracker].wave_import(waveGenReceive);
			Waveform::waveNumberTracker++;
		}

		// Check for channels to clear
		status = osMessageQueueGet(ChannelClearQueueHandle, &clearChannel, NULL, 1);
		if (status == osOK) {
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

		// Update waves with envelopes
		for (uint8_t i = 0; i < Waveform::waveNumberTracker; i++) {
			if (!waves[i].update_envelope()) {
				for (uint8_t j = i; j < Waveform::waveNumberTracker - 1; j++) {
						waves[j] = waves[j+1];
				}
				Waveform::waveNumberTracker--;
			}
		}

		// Enable flag on all waves that are ready
		for (uint8_t i = 0; i < Waveform::waveNumberTracker; i++) {
			waves[i].update_period();
		}
		
		// Output to DAC when it is ready on required channels
		if (DAC_DMA_Ready()) {
			for (uint8_t i = 0; i < Waveform::waveNumberTracker; i++) {
				if (waves[i].readyForDAC) {
					// Output to DAC
					waves[i].output_wave();
					break;
				}
			}
		} else {
			osDelay(1);
		}
	}
}

/* Waveform Class Function Defintions */

/**
 * @brief 
 * @param 
 * @retval None
 */
void Waveform::generate_waveform(void) {
	// Add wave to data array
	float B = M_PI / (DATA_BITS - 1);
	for (uint16_t i = 0; i < DATA_BITS; i++) {
		switch (waveParams.waveType) {
			case PULSE_SINE:
				waveData[i] = waveParams.amplitude * std::sin((i) * B);
				break;
			case PULSE_SQUARE:
				waveData[i] = waveParams.amplitude;
				break;
			case PULSE_TRIANGLE:
				waveData[i] = (uint16_t)(waveParams.amplitude - (2 * std::abs((waveParams.amplitude / 2) - (waveParams.amplitude * (DATA_BITS - i - 1) / DATA_BITS))));
				break;
			case PULSE_SAWTOOTH:
				waveData[i] = waveParams.amplitude - (waveParams.amplitude * (DATA_BITS - i - 1) / DATA_BITS);
				break;
		}
		waveData[DATA_BITS - 1] = 0; // Ensure post-wave DAC output is 0
	}
}
 
/**
 * @brief 
 * @param 
 * @retval None
 */
void Waveform::update_period(void) {
	// If the system tick timer has overflowed, we want to reset nextTickVal
	// Simple test is if system tick is more than 1 period behind nextTickVal
	if (osKernelGetSysTimerCount() < nextTickVal - 80000000U / waveParams.frequency) {
		nextTickVal = osKernelGetSysTimerCount();
	}

	// Only check if period finished if the wave has finished its
	// previous period (aka, readyForDAC is false)
	if (!readyForDAC && osKernelGetSysTimerCount() >= nextTickVal) {
		// Update Period
		nextTickVal += 80000000U / waveParams.frequency;

		// Set DAC Flag
		readyForDAC = true;

		// Immediately check if period is behind. If it is *still* behind, this
		// implies the wave has not run for a long time; in which case we not
		// let the wave run as it will immediate run again after this
		if (osKernelGetSysTimerCount() >= nextTickVal) {
			readyForDAC = false;
		}
	}
}

/**
 * @brief 
 * @param 
 * @retval None
 */
void Waveform::output_wave(void) {

	uint16_t switchState = (1 << (waveParams.channel + 1));
	//uint16_t switchState = (1 << 1);
	SPI_master_send(&switchState, 1);
	// Change channel (SPI/DMA)
	/*if (waveParams.channel == CHANNEL_2) {
		uint16_t switchState = (1 << waveParams.channel) | (1<<CHANNEL_15);
		SPI_master_send(&switchState, 1);
	} else {
		uint16_t switchState = (1 << waveParams.channel);
		SPI_master_send(&switchState, 1);
	}//*/

	// Set the frequency
	push_pulse_width_to_DAC_timer();

	// Delay to allow SPI to finish
	//uint32_t bufferTimer = osKernelGetSysTimerCount();
	//while (bufferTimer + SWITCH_MATRIX_DELAY > osKernelGetSysTimerCount()) {};

	// I cannot for the life of me figure out a reliable way to know when the DAC/DMA is finished transferring and I have had enough. I have tried the TC flag in the DMA interrupt to synchronize with a semaphore, but it just won't work properly. Have also tried a bunch of different flags like HAL_DMA_GetState(&hdma_dac_ch1) != HAL_DMA_STATE_BUSY), but again it just won't work reliably (current solution is to add a bunch of artificial delays, but even then it is unstable below 100uS)

	while (!DAC_DMA_Ready()) {}; // Wait incase the DAC not ready
	DAC_Start((uint32_t*)waveData, DATA_BITS);

	//while (!DAC_DMA_Ready()) {}; // Wait incase the DAC not ready
	//osDelay(1);
	//DAC_Start((uint32_t*)waveData, DATA_BITS);
	//osDelay(1);

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
	// Import ramp/envelope details
	envelopeData.envelopeOnOff = waveGenReceive.envelopeOnOff;
	envelopeData.amplitude = waveGenReceive.amplitude;
	envelopeData.rampIn = waveGenReceive.rampIn * (SystemCoreClock / 1000); // convert ms to ticks
	envelopeData.rampOut = waveGenReceive.rampOut * (SystemCoreClock / 1000); // convert ms to ticks
	envelopeData.envelopeWidth = waveGenReceive.envelopeWidth * (SystemCoreClock / 1000); // convert ms to ticks
	envelopeData.startTime = osKernelGetSysTimerCount();
	// Update nextTickVal and generate wave
	nextTickVal = osKernelGetSysTimerCount();
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

	//UART_Send_Char('a');
	// Check if period is finished
	if (envelopeData.startTime + envelopeData.envelopeWidth < osKernelGetSysTimerCount()) {
			return false;
	}
	// Calculate amplitude for start of trapezoidal envelope
	if (envelopeData.startTime + envelopeData.rampIn > osKernelGetSysTimerCount()) {
		//waveParams.amplitude = envelopeData.amplitude / 2;
		waveParams.amplitude = (((uint64_t)osKernelGetSysTimerCount() - envelopeData.startTime) * envelopeData.amplitude) / envelopeData.rampIn;
	}
	// Calculate amplitude for end of trapezoidal envelope
	else if (envelopeData.startTime + envelopeData.envelopeWidth - envelopeData.rampOut < osKernelGetSysTimerCount()) {
		//waveParams.amplitude = envelopeData.amplitude / 4;
		waveParams.amplitude = ((((uint64_t)envelopeData.startTime + envelopeData.envelopeWidth) - osKernelGetSysTimerCount()) * envelopeData.amplitude)  / envelopeData.rampOut;
	} else {
		waveParams.amplitude = envelopeData.amplitude;
	}
	generate_waveform();
	return true;
}

/**
 * @brief 
 * @param 
 * @retval None
 */
void Waveform::push_pulse_width_to_DAC_timer(void) {
	// Set the frequency
	TIM2_Init(static_cast<int>(waveParams.width * 1.615));
}

