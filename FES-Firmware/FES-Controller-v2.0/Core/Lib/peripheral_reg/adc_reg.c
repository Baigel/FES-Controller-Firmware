/**
 ******************************************************************************
 * @file			: adc_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: ADC Register File
 ******************************************************************************
*/

/* Includes */
#include "adc_reg.h"

ADC_HandleTypeDef hadc1;

/* ADC1 init function */
void ADC1_Init(void) {
	ADC_ChannelConfTypeDef sConfig = {0};

	/* Common config */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}
	/* Configure Regular Channel */
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
}

uint16_t ADC_get_value(void) {
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1);
	uint16_t ADC_VAL = HAL_ADC_GetValue(&hadc1);
	//HAL_ADC_Stop(&hadc1);
	return ADC_VAL;
}

uint16_t ADC_Select_GetValue(uint8_t adc_channel) {
	ADC_ChannelConfTypeDef sConfig = {0};
	switch (adc_channel) {
		case 5:
			sConfig.Channel = ADC_CHANNEL_5;
			break;
		case 16:
			sConfig.Channel = ADC_CHANNEL_16;
			break;
		default:
			return 0;
	}
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1);
	uint16_t ADC_VAL = HAL_ADC_GetValue(&hadc1);
	//HAL_ADC_Stop(&hadc1);
	return ADC_VAL;
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	if(adcHandle->Instance==ADC1) {
	/* Initializes the peripherals clock */
		PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
		PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
		PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
		PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
		PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
		PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
		PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
		PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
		PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
		}

		/* ADC1 clock enable */
		__HAL_RCC_ADC_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/* ADC1 GPIO Configuration
		PA0     ------> ADC1_IN5
		PB1     ------> ADC1_IN16 */
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle) {
	if(adcHandle->Instance==ADC1) {
		/* Peripheral clock disable */
		__HAL_RCC_ADC_CLK_DISABLE();

		/**ADC1 GPIO Configuration
		PA0     ------> ADC1_IN5
		PB1     ------> ADC1_IN16 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);
	}
}

