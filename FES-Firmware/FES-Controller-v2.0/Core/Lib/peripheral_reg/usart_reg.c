/**
 ******************************************************************************
 * @file			: usart_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: USART Register File
 ******************************************************************************
*/

/* Includes */
#include "usart_reg.h"

UART_HandleTypeDef huart2;

/* USART2 init function */

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

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	if(uartHandle->Instance==USART2) {
	/* Initializes the peripherals clock */
		PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
		PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
		}

		/* USART2 clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/* USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA15 (JTDI)     ------> USART2_RX */
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF3_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle) {
	if(uartHandle->Instance==USART2) {
		/* Peripheral clock disable */
		__HAL_RCC_USART2_CLK_DISABLE();

		/* USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA15 (JTDI)     ------> USART2_RX */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_15);
	}
}

/**
  * @brief USART2 Send a char
  * @param None
  * @retval None
  */
void UART_Send_Char(unsigned char send) {
	HAL_UART_Transmit(&huart2, &send, 1, 1);
}

