/**
 ******************************************************************************
 * @file			: central_controller_library.h
 * @brief			: Functions for generating spi FES-controller packets
 * @author			: Ahmed Hafeez-Baig
 ******************************************************************************
*/

#ifndef __CENTRAL_CONTROLLER__
#define __CENTRAL_CONTROLLER__

/* Include Libraries */
#include <stdint.h>
#include <stdlib.h>

/* Struct Defintions */
__weak typedef struct {
    uint8_t* data;
    uint8_t size;
} DataStruct;

/* Function Declarations */
__weak DataStruct continuous_wave(char waveform, uint8_t channel, uint8_t frequency, uint8_t amplitude, uint8_t width);
__weak DataStruct enveloped_wave(char waveform, uint8_t channel, uint8_t frequency, uint8_t amplitude, uint8_t width, uint8_t rampin, uint8_t rampout, uint8_t envelopeWidth);
__weak DataStruct clear_channel(uint8_t channelAnode, uint8_t channelCathode);
__weak DataStruct disable_waves(void);
__weak DataStruct enable_waves(void);

#endif
