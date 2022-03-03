/**
 ******************************************************************************
 * @file			: dac_reg.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: DAC Register File
 ******************************************************************************
*/

#ifndef __DAC_REG_H__
#define __DAC_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "main.h"
#include "tim_reg.h"
#include <stdbool.h>

void DAC1_Init(void);
bool DAC_Ready(void);
void DAC_Start(uint32_t* wave, uint16_t size);
void DAC_Enable(void);
void DAC_Disable(void);
bool DAC_DMA_TC_flag_set(void);
bool DAC_DMA_HT_flag_set(void);

#ifdef __cplusplus
}
#endif

#endif
