/**
 ******************************************************************************
 * @file			: uasrt_init.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: USART Register File
 ******************************************************************************
*/

/* Include Header */
#include "uart_reg.h"

/* HandleType Definitions */
UART_HandleTypeDef huart2;

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void USART2_UART_Init(void) {
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
}

/**
  * @brief USART2 Send a char
  * @param None
  * @retval None
  */
void UART_Send_Char(unsigned char send) {
	HAL_UART_Transmit(&huart2, &send, 1, HAL_MAX_DELAY);
}

/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(huart->Instance==USART2) {
		/* Peripheral clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/** USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX */
		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart) {
	if(huart->Instance==USART2) {
		/* Peripheral clock disable */
		__HAL_RCC_USART2_CLK_DISABLE();

		/** USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
	}
}



// TEMPORARY
#include "adc_reg.h"
void print_adc(void) {
	char msg[100];
	// Convert to string and print
	sprintf(msg, "ADC: %hu mA\r\n", Get_ADC());
	//sprintf(msg, "%hu\r\n", raw);
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
