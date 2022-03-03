/**
 ******************************************************************************
 * @file			: dac_dma_init.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: DAC Register File
 ******************************************************************************
*/

/* Include Header */
#include "dac_dma_reg.h"

/* HandleType Definitions */
DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac_ch1;
TIM_HandleTypeDef htim2;

/* Internal Function Prototypes */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac);

/* External Function Definitions */

/**
 * @brief DAC1 Initialization Function
 * @param None
 * @retval None
 */
void DAC1_Init(void) {
	DAC_ChannelConfTypeDef sConfig = {0};

	/* DAC Initialization */
	hdac1.Instance = DAC1;
	if (HAL_DAC_Init(&hdac1) != HAL_OK) {
		Error_Handler();
	}
	/* DAC channel OUT1 config */
	sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
	sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
	sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
	if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
	Error_Handler();
	}
}

/**
 * @brief DAC1 DMA Initialization Function
 * @param None
 * @retval None
 */
void DAC_DMA_Init(void) {
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
}

/**
 * @brief TIM2 Initialization Function
 * @param period: changing the period of TIM2 changes the frequency of the DAC output
 * @retval None
 */
void TIM2_Init(uint16_t period) {
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

/**
 * @brief Start the DAC with the array given
 * @param wave: lookup array (0-4095)
 * @param size: size of the lookup array
 * @retval None
 */
void DAC_Start(uint32_t* wave, uint16_t size) {
	// Start the DAC DMA on channel 1
	HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t*)wave, size, DAC_ALIGN_12B_R);
	HAL_TIM_Base_Start(&htim2);
}

/**
 * @brief Is the DAC ready for output
 * @param None
 * @retval None
 */
bool DAC_DMA_Ready(void) {
	return (HAL_DMA_GetState(&hdma_dac_ch1) != HAL_DMA_STATE_BUSY);
	//return DMA_transferComplete;
}

/**
 * @brief 
 * @param None
 * @retval None
 */
void DAC_Enable(void) {
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	HAL_TIM_Base_Start(&htim2);
}

/**
 * @brief 
 * @param None
 * @retval None
 */
void DAC_Disable(void) {
	HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
	HAL_TIM_Base_Stop(&htim2);
}

/**
 * @brief 
 * @param None
 * @retval None
 */
void DMA_interrupt_callback(void) {
	if (__HAL_DMA_GET_FLAG(&hdma_dac_ch1, DMA_FLAG_TC3)) { // check if transfer complete flag set
		// Full transfer complete
	} else if (__HAL_DMA_GET_FLAG(&hdma_dac_ch1, DMA_FLAG_HT3)) {
		gpio_pin_PA11(GPIO_PIN_SET);
		read_adc();
		gpio_pin_PA11(GPIO_PIN_RESET);
		// Call function to adjust current based on ADC reading
	}
}

/* Internal Function Defintions */

/**
* @brief DAC MSP Initialization
* This function configures the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(hdac->Instance==DAC1) {
		/* Peripheral clock enable */
		__HAL_RCC_DAC1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**DAC1 GPIO Configuration
		PA4     ------> DAC1_OUT1 */
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* DAC1 DMA Init */
		/* DAC_CH1 Init */
		hdma_dac_ch1.Instance = DMA1_Channel3;
		hdma_dac_ch1.Init.Request = DMA_REQUEST_6;
		hdma_dac_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_dac_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_dac_ch1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_dac_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_dac_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_dac_ch1.Init.Mode = DMA_NORMAL;
		hdma_dac_ch1.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_dac_ch1) != HAL_OK) {
			Error_Handler();
		}

		__HAL_LINKDMA(hdac,DMA_Handle1,hdma_dac_ch1);

		/* DAC1 interrupt Init */
		HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}

}

/**
* @brief DAC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hdac: DAC handle pointer
* @retval None
*/
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac) {
	if(hdac->Instance==DAC1) {
		/* Peripheral clock disable */
		__HAL_RCC_DAC1_CLK_DISABLE();

		/**DAC1 GPIO Configuration
		PA4     ------> DAC1_OUT1 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

		/* DAC1 DMA DeInit */
		HAL_DMA_DeInit(hdac->DMA_Handle1);

		/* DAC1 interrupt DeInit */
		HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
	}
}

/**
* @brief TIM_Base MSP Initialization
* @param 
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base) {
	if(htim_base->Instance==TIM2) {
		/* Peripheral clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();
	}
}

/**
* @brief TIM_Base MSP De-Initialization
* @param 
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base) {
	if(htim_base->Instance==TIM2) {
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();
	}
}
