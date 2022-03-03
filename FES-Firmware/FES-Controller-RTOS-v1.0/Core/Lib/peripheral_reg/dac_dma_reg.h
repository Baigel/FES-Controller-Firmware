/**
 ******************************************************************************
 * @file			: dac_dma_init.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: DAC Register File
 ******************************************************************************
*/

#ifndef __DAC_DMA_REG__
#define __DAC_DMA_REG__

/* std library Includes */
#include <stdbool.h>

/* Include */
#include "stm32l4xx_hal.h"
#include "main.h" // For Error_Handle

/* DAC DMA External Function Prototypes */
void DAC1_Init(void);
void DAC_DMA_Init(void);
void TIM2_Init(uint16_t period);
void DAC_Start(uint32_t* wave, uint16_t size);
bool DAC_DMA_Ready(void);
void DAC_Enable(void);
void DAC_Disable(void);
void DMA_interrupt_callback(void);

#endif
