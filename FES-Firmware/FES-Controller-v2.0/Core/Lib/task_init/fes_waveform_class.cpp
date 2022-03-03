/**
******************************************************************************
// * @file                fes_waveform_class.cpp
// * @author              Ahmed Hafeez-Baig
// * @date                14042021
// * @brief               Waveform class definition
//******************************************************************************
//*/
//
//
//#include "fes_waveform_class.hpp"
//
///* Struct Constructors */
////WaveEnvelope::WaveEnvelope(void) {
////	envelopeOnOff = false;
////}
//
//// Initialise class static members
//uint8_t Waveform::waveNumberTracker = 0;
//
//
//Waveform waves[NUMBER_OF_WAVES + 1];
//
//constexpr uint8_t DATA_BITS = 50; // Databits per wave
//constexpr uint8_t NUMBER_OF_WAVES = 8;  // Max number of waves
//
///* Waveform Class Function Defintions */
//
///**
// * @brief 
// * @param 
// * @retval None
// */
//void Waveform::generate_waveform(void) {
//	// First use pulsewidth to determine how many bits will be used (shorter pulsewidth => fewer bits)
//	if (waveParams.width > 50) {
//		waveParams.waveDataBytes = DATA_BITS;
//	} else if (waveParams.width > 10) {
//		waveParams.waveDataBytes = 50;
//	} else if (waveParams.width > 3) {
//		waveParams.waveDataBytes = 20;
//	} else {
//		waveParams.waveDataBytes = 8;
//	}
//	// Add wave to data array
//	float B = M_PI / (waveParams.waveDataBytes - 1);
//	for (uint16_t i = 0; i < waveParams.waveDataBytes; i++) {
//		switch (waveParams.waveType) {
//			case PULSE_SINE:
//				waveData[i] = waveParams.amplitude * std::sin((i) * B);
//				break;
//			case PULSE_SQUARE:
//				waveData[i] = waveParams.amplitude;
//				break;
//			case PULSE_TRIANGLE:
//				waveData[i] = (uint16_t)(waveParams.amplitude - (2 * std::abs((waveParams.amplitude / 2) - (waveParams.amplitude * (waveParams.waveDataBytes - i - 1) / waveParams.waveDataBytes))));
//				break;
//			case PULSE_SAWTOOTH:
//				waveData[i] = waveParams.amplitude - (waveParams.amplitude * (waveParams.waveDataBytes - i - 1) / waveParams.waveDataBytes);
//				break;
//		}
//		waveData[waveParams.waveDataBytes - 1] = 0; // Ensure post-wave DAC output is 0
//	}
//}
//
///**
// * @brief 
// * @param 
// * @retval None
// */
//void Waveform::update_period(uint64_t current) {
//	// Only check if period finished if the wave has finished its
//	// previous period (aka, readyForDAC is false)
//	//if (!readyForDAC && getCurrentMicros() >= nextTickVal) {
//	if (current >= nextTickVal) {
//		// Update Period
//		nextTickVal += 1000000UL / waveParams.frequency;
//
//		// Set DAC Flag
//		readyForDAC = true;
//
//		// Immediately check if period is behind. If it is *still* behind, this
//		// implies the wave has not run for a long time; in which case we don't
//		// let the wave run as it will immediate run again after this
//		if (current >= nextTickVal) {
//			readyForDAC = false;
//		}
//	}
//}
//
///**
// * @brief 
// * @param 
// * @retval None
// */
//void Waveform::update_output_wave(void) {
//	//uint16_t switchState = (1 << (waveParams.channel + 1));
//	//SPI_master_send(&switchState, 1);
//	// Change channel (SPI/DMA)
//	/*if (waveParams.channel == CHANNEL_2) {
//		uint16_t switchState = (1 << waveParams.channel) | (1<<CHANNEL_15);
//		SPI_master_send(&switchState, 1);
//	} else {
//		uint16_t switchState = (1 << waveParams.channel);
//		SPI_master_send(&switchState, 1);
//	}//*/
//
//	// Set the frequency
//	update_pulse_width_DAC_timer();
//
//	// Delay to allow SPI to finish
//	//uint64_t bufferTimer = getCurrentMicros();
//	//while (bufferTimer + SWITCH_MATRIX_DELAY > getCurrentMicros()) {};
//	
//	while (!DAC_Ready()) {}; // Wait incase the DAC not ready
//	DAC_Start((uint32_t*)waveData, waveParams.waveDataBytes);
//
//	if (waveParams.bipolar) {
//		// Wait for interphasic delay
//		uint64_t bufferTimer = getCurrentMicros();
//
//		// Invert channels (bipolar)
//		// TODO
//
//		// Wait for interphasic delay
//		while (bufferTimer + waveParams.interphaseDelay > getCurrentMicros()) {};
//
//		while (!DAC_Ready()) {}; // Wait incase the DAC not ready
//		DAC_Start((uint32_t*)waveData, waveParams.waveDataBytes);
//	}
//
//	// Clear DAC Flag
//	readyForDAC = false;
//}
//
///**
// * @brief 
// * @param 
// * @retval None
// */
//void Waveform::wave_import(WaveGenStruct waveGenReceive) {
//	// Import wave details
//	waveParams.waveType = waveGenReceive.waveType;
//	waveParams.channel = waveGenReceive.channel;
//	waveParams.frequency = waveGenReceive.frequency;
//	waveParams.amplitude = waveGenReceive.amplitude;
//	waveParams.width = waveGenReceive.width;
//	// Import biphasic wave details
//	waveParams.bipolar = waveGenReceive.bipolar;
//	waveParams.interphaseDelay = waveGenReceive.interphaseDelay;
//	// Import ramp/envelope details
//	envelopeData.envelopeOnOff = waveGenReceive.envelopeOnOff;
//	envelopeData.amplitude = waveGenReceive.amplitude;
//	envelopeData.rampIn = waveGenReceive.rampIn * (SystemCoreClock / 1000); // convert ms to ticks
//	envelopeData.rampOut = waveGenReceive.rampOut * (SystemCoreClock / 1000); // convert ms to ticks
//	envelopeData.envelopeWidth = waveGenReceive.envelopeWidth * (SystemCoreClock / 1000); // convert ms to ticks
//	envelopeData.startTime = HAL_GetTick();
//	// Update nextTickVal and generate wave
//	nextTickVal = getCurrentMicros();
//	generate_waveform();
//}
//
///**
// * @brief 
// * @param 
// * @retval None
// */
//bool Waveform::update_envelope(void) {
//	// Check if envelope enabled; if not, exit
//	if (!envelopeData.envelopeOnOff) {
//		return true;
//	}
//
//	//UART_Send_Char('a');
//	// Check if period is finished
//	if (envelopeData.startTime + envelopeData.envelopeWidth < HAL_GetTick()) {
//			return false;
//	}
//	// Calculate amplitude for start of trapezoidal envelope
//	if (envelopeData.startTime + envelopeData.rampIn > HAL_GetTick()) {
//		//waveParams.amplitude = envelopeData.amplitude / 2;
//		waveParams.amplitude = (((uint64_t)HAL_GetTick() - envelopeData.startTime) * envelopeData.amplitude) / envelopeData.rampIn;
//	}
//	// Calculate amplitude for end of trapezoidal envelope
//	else if (envelopeData.startTime + envelopeData.envelopeWidth - envelopeData.rampOut < HAL_GetTick()) {
//		//waveParams.amplitude = envelopeData.amplitude / 4;
//		waveParams.amplitude = ((((uint64_t)envelopeData.startTime + envelopeData.envelopeWidth) - HAL_GetTick()) * envelopeData.amplitude)  / envelopeData.rampOut;
//	} else {
//		waveParams.amplitude = envelopeData.amplitude;
//	}
//	
//	generate_waveform();
//	return true;
//}
//
///**
// * @brief Adpate the amplitude of a wave, based on its amplitude as determined by the ADC
// * @param 
// * @retval None
// */
//void Waveform::update_amplitude(uint16_t adc_value) {
//	// TODO: write code to adjust amplitude
//
//	// End by re-generating waveform data
//	generate_waveform();
//}
//
///**
// * @brief 
// * @param 
// * @retval None
// */
//inline void Waveform::update_pulse_width_DAC_timer(void) {
//	// Set the frequency
//	TIM2_Init(static_cast<int>(waveParams.width * 80.75 / waveParams.waveDataBytes));
//}
//
//