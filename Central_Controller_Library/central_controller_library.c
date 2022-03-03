/**
  ******************************************************************************
  * @file			: central_controller_library.c
  * @brief			: Functions for generating spi FES-controller packets
  * @author			: Ahmed Hafeez-Baig
  ******************************************************************************
*/

/* Include Header Library */
#include "central_controller_library.h"


/* External Function Definitions */

/**
  * @brief
  * @param
  * @retval
*/
__weak DataStruct clear_channel(uint8_t channelAnode, uint8_t channelCathode) {
    DataStruct clearData;
    clearData.data = malloc(sizeof(uint8_t) * 2);
    clearData.data[0] = 'C';
    clearData.data[1] = channel;
    clearData.size = 2;

    return clearData;
}

/**
  * @brief
  * @param
  * @retval
*/
__weak DataStruct continuous_wave(char waveform, uint8_t channel, uint8_t frequency, uint8_t amplitude, uint8_t width) {
    DataStruct waveData;
    waveData.data = malloc(sizeof(uint8_t) * 7);
    waveData.data[0] = 'W';
    waveData.data[1] = waveform;
    waveData.data[2] = channel;
    waveData.data[3] = frequency;
    waveData.data[4] = amplitude;
    waveData.data[5] = width;
    waveData.data[6] = 'f';
    waveData.size = 7;

    return waveData;
}

/**
  * @brief
  * @param
  * @retval
*/
__weak DataStruct enveloped_wave(char waveform, uint8_t channel, uint8_t frequency, uint8_t amplitude, uint8_t width, uint8_t rampin, uint8_t, rampout, uint8_t envelopeWidth) {
    DataStruct waveData;
    waveData.data = malloc(sizeof(uint8_t) * 10);
    waveData.data[0] = 'W';
    waveData.data[1] = waveform;
    waveData.data[2] = channel;
    waveData.data[3] = frequency;
    waveData.data[4] = amplitude;
    waveData.data[5] = width;
    waveData.data[6] = 't';
    waveData.data[7] = rampin;
    waveData.data[8] = rampout;
    waveData.data[9] = envelopeWidth;
    waveData.size = 10;

    return waveData;
}

/**
  * @brief
  * @param
  * @retval
*/
__weak DataStruct disable_waves(void) {
    DataStruct waveData;
    waveData.data = malloc(sizeof(uint8_t) * 1);
    waveData.data[0] = 'D';
    waveData.size = 1;

    return waveData;
}

/**
  * @brief
  * @param
  * @retval
*/
__weak DataStruct enable_waves(void) {
    DataStruct waveData;
    waveData.data = malloc(sizeof(uint8_t) * 1);
    waveData.data[0] = 'S';
    waveData.size = 1;

    return waveData;
}
