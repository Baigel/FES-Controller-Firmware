/**
******************************************************************************
 * @file                fes_wavegen.hpp
 * @author              Ahmed Hafeez-Baig
 * @date                14042021
 * @brief               Wave generation class definitions
******************************************************************************
*/

#ifndef __FES_WAVEGEN__
#define __FES_WAVEGEN__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32l4xx_hal.h"
#include "math.h"
#include "stdbool.h"
#include "fes_waveform_class.hpp"

/* Library Includes */
#include "dac_reg.h"
#include "dma_reg.h"
#include "tim_reg.h"
#include "spi_reg.h"
#include "gpio_reg.h"
#include "adc_reg.h"
#include "clock.h"

/* Define Macros */
// Defining Stimulation Channels
#define CHANNEL_1		0
#define CHANNEL_2		1
#define CHANNEL_3		2
#define CHANNEL_4		3
#define CHANNEL_5		4
#define CHANNEL_6		5
#define CHANNEL_7		6
#define CHANNEL_8		7
#define CHANNEL_9		8
#define CHANNEL_10		9
#define CHANNEL_11		10
#define CHANNEL_12		11
#define CHANNEL_13		12
#define CHANNEL_14		13
#define CHANNEL_15		14
#define CHANNEL_16		15
// Define pulse types
#define PULSE_SINE		1
#define PULSE_SQUARE	2
#define PULSE_TRIANGLE	3
#define PULSE_SAWTOOTH	4

/* Define Structs */
// Used to convert paramaters into actual waveform
typedef struct {
	uint8_t waveType;
	uint8_t channel;
	uint32_t frequency;
	uint16_t amplitude;
	uint16_t width;
	bool envelopeOnOff;
	uint16_t rampIn;
	uint16_t rampOut;
	uint16_t envelopeWidth;
	uint8_t waveDataBytes;
	bool bipolar;
	uint16_t interphaseDelay;
} WaveGenStruct;

/* Task Function Prototypes */
void fes_wavegen_task_init(void);
void fes_wavegen_task(void);
void TIM7_callback(void);
void clear_wave(uint8_t wave);
void new_wave(WaveGenStruct waveGenReceive);
void DMA_transfer_interrupt_callback(void);

#ifdef __cplusplus
}
#endif

#endif

