/**
 ******************************************************************************
 * @file			: tim_reg.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: TIM Register File
 ******************************************************************************
*/

#ifndef __TIM_REG_H__
#define __TIM_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "main.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;

void TIM2_Init(uint32_t period);
void TIM7_Init(void);

#ifdef __cplusplus
}
#endif

#endif
