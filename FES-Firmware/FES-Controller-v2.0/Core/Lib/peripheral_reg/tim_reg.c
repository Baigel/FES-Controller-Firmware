/**
 ******************************************************************************
 * @file			: tim_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: TIM Register File
 ******************************************************************************
*/

/* Includes */
#include "tim_reg.h"

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim7;

/* TIM2 init function */
void TIM2_Init(uint32_t period) {
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = period;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
}

/* TIM7 init function */
void TIM7_Init(void) {
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 0;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 50000;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK) {
		Error_Handler();
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle) {
	if(tim_baseHandle->Instance==TIM2) {
		/* TIM2 clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();

		/* TIM2 interrupt Init */
		//HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
		//HAL_NVIC_EnableIRQ(TIM2_IRQn);
	
	} else if(tim_baseHandle->Instance==TIM7) {
		/* TIM7 clock enable */
		__HAL_RCC_TIM7_CLK_ENABLE();

		/* TIM7 interrupt Init */
		HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM7_IRQn);
	}
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle) {
	if(tim_baseHandle->Instance==TIM2) {
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();

		/* TIM2 interrupt Deinit */
		//HAL_NVIC_DisableIRQ(TIM2_IRQn);

	} else if(tim_baseHandle->Instance==TIM7) {
		/* Peripheral clock disable */
		__HAL_RCC_TIM7_CLK_DISABLE();

		/* TIM7 interrupt Deinit */
		HAL_NVIC_DisableIRQ(TIM7_IRQn);
	}
}

