/**
 ******************************************************************************
 * @file			: adc_reg.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: ADC Register File
 ******************************************************************************
*/

#ifndef __ADC_REG_H__
#define __ADC_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "main.h"

void ADC1_Init(void);
uint16_t ADC_get_value(void);

#ifdef __cplusplus
}
#endif

#endif
