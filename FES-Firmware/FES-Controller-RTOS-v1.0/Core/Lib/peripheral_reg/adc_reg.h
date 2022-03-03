/**
 ******************************************************************************
 * @file			: adc_reg.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 06-09-2021
 * @brief			: ADC Register File
 ******************************************************************************
*/

#ifndef __ADC_REG__
#define __ADC_REG__

/* Include */
#include "stm32l4xx_hal.h"
#include "main.h" // For Error_Handle

/* ADC External Function Prototypes */
void ADC1_Init(void);
uint16_t Get_ADC(void);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);

#endif


