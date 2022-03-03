/**
 ******************************************************************************
 * @file			: dac_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: DAC Register File
 ******************************************************************************
*/

/* Includes */
#include "dac_reg.h"

DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac_ch1;
DMA_HandleTypeDef hdma_dac_ch2;

/* DAC1 init function */
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
	/* DAC channel OUT2 config */
	//if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK) {
	//	Error_Handler();
	//}
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(dacHandle->Instance==DAC1) {
		/* DAC1 clock enable */
		__HAL_RCC_DAC1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/* DAC1 GPIO Configuration
		PA4     ------> DAC1_OUT1
		PA5     ------> DAC1_OUT2 */
		GPIO_InitStruct.Pin = GPIO_PIN_4;//|GPIO_PIN_5;
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
		hdma_dac_ch1.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		if (HAL_DMA_Init(&hdma_dac_ch1) != HAL_OK) {
			Error_Handler();
		}

		__HAL_LINKDMA(dacHandle,DMA_Handle1,hdma_dac_ch1);

		/* DAC_CH2 Init */
		/*
		hdma_dac_ch2.Instance = DMA1_Channel4;
		hdma_dac_ch2.Init.Request = DMA_REQUEST_5;
		hdma_dac_ch2.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_dac_ch2.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_dac_ch2.Init.MemInc = DMA_MINC_ENABLE;
		hdma_dac_ch2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
		hdma_dac_ch2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_dac_ch2.Init.Mode = DMA_NORMAL;
		hdma_dac_ch2.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		if (HAL_DMA_Init(&hdma_dac_ch2) != HAL_OK) {
			Error_Handler();
		}

		__HAL_LINKDMA(dacHandle,DMA_Handle2,hdma_dac_ch2);
		*/
	}
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle) {
	if(dacHandle->Instance==DAC1) {
		/* Peripheral clock disable */
		__HAL_RCC_DAC1_CLK_DISABLE();

		/* DAC1 GPIO Configuration
		PA4     ------> DAC1_OUT1
		PA5     ------> DAC1_OUT2 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
		//HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5);

		/* DAC1 DMA DeInit */
		HAL_DMA_DeInit(dacHandle->DMA_Handle1);
		//HAL_DMA_DeInit(dacHandle->DMA_Handle2);
	}
}

/**
 * @brief Is the DAC ready for output
 * @param None
 * @retval None
 */
bool DAC_Ready(void) {
	return (HAL_DMA_GetState(&hdma_dac_ch1) != HAL_DMA_STATE_BUSY);
	//return (HAL_DAC_GetState(&hdac1) != HAL_DAC_STATE_BUSY);
	//return DMA_transferComplete;
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
bool DAC_DMA_TC_flag_set(void) {
	return __HAL_DMA_GET_FLAG(&hdma_dac_ch1, DMA_FLAG_TC3);
}

/**
 * @brief 
 * @param None
 * @retval None
 */
bool DAC_DMA_HT_flag_set(void) {
	return __HAL_DMA_GET_FLAG(&hdma_dac_ch1, DMA_FLAG_HT3);
}
