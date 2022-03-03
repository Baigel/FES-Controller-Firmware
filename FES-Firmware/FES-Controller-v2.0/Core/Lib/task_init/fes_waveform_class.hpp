///**
//******************************************************************************
// * @file                fes_wavegen.hpp
// * @author              Ahmed Hafeez-Baig
// * @date                14042021
// * @brief               Wave generation class definitions
//******************************************************************************
//*/
//
//#ifndef __FES_WAVEFORM_CLASS__
//#define __FES_WAVEFORM_CLASS__
//
///* Includes */
//#include "fes_wavegen.hpp"
//
///* Struct Definitions */
//typedef struct{
//	bool envelopeOnOff;
//	uint32_t rampIn;
//	uint32_t rampOut;
//	uint16_t amplitude;
//	uint64_t startTime;
//	uint64_t envelopeWidth;
//	//WaveEnvelope(void);
//} WaveEnvelope;
//
//// Used to convert paramaters into actual waveform
//typedef struct{
//	uint8_t waveType;
//	uint8_t channel;
//	uint32_t frequency;
//	uint16_t amplitude;
//	uint16_t width;
//	bool envelopeOnOff;
//	uint16_t rampIn;
//	uint16_t rampOut;
//	uint16_t envelopeWidth;
//} WaveGenStruct;
//
//extern uint8_t NUMBER_OF_WAVES;
//
///* WaveForm Class Definition */
//class Waveform {
//
//	private: /* Declarations */
//
//		/* Parameter Values:
//		* Frequency
//		* Amplitude
//		* Waveform
//		* Infinite/Timed (aka, should waveform go on until stopped, or for a defined period)
//		* Ramp Up/Down (both how much, and for how long)
//		*/
//
//		/* Private Variable Declarations */
//		uint16_t waveData[DATA_BITS] = {0};
//		WaveEnvelope envelopeData;
//
//		/* Private Function Prototypes */
//		inline void update_pulse_width_DAC_timer(void);
//
//	public:
//
//		/* Public Variables */
//		static uint8_t waveNumberTracker;
//		WaveGenStruct waveParams;
//		bool readyForDAC = false;
//		uint64_t nextTickVal = 0;
//
//		/* Function Declarations */
//		void generate_waveform(void);
//		void wave_import(WaveGenStruct waveGenReceive);
//		void update_period(uint64_t current);
//		bool update_envelope(void);
//		void update_output_wave(void);
//};
//
//extern Waveform waves[NUMBER_OF_WAVES + 1];
//
//#endif
//
//