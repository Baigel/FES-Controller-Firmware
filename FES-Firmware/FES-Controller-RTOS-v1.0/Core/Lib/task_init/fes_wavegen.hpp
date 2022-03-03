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
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "math.h"

/* Library Includes */
#include "dac_dma_reg.h"
#include "spi_master_reg.h"
#include "gpio_reg.h"
#include "adc_reg.h"

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
	uint16_t frequency;
	uint16_t amplitude;
	uint16_t width;
	bool envelopeOnOff;
	uint16_t rampIn;
	uint16_t rampOut;
	uint16_t envelopeWidth;
} WaveGenStruct;

/* External Function Prototypes */
void wavegen_task_init(void);
void add_to_wavegen_queue(WaveGenStruct waveGenTransmit);
void add_to_channel_clear_queue(uint8_t channel);
void read_adc(void);

#ifdef __cplusplus
}
#endif

#endif

