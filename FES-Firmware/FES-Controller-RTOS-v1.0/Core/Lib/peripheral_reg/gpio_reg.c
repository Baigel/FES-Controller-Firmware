/**
 ******************************************************************************
 * @file			: gpio_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: GPIO Register File
 ******************************************************************************
*/

/* Include Header */
#include "gpio_reg.h"

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_11, GPIO_PIN_RESET);
	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

	/*Configure GPIO pin : PA8 PA11 */
	GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/*Configure GPIO pins : PB6 PB7 */
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	return;
}

/**
 * @brief 
 * @param None
 * @retval None
 */
void gpio_pin_PB6(GPIO_PinState pin_state) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, pin_state);
}

/**
 * @brief 
 * @param None
 * @retval None
 */
void gpio_pin_PB7(GPIO_PinState pin_state) {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, pin_state);
}

/**
 * @brief 
 * @param None
 * @retval None
 */
void gpio_pin_PA11(GPIO_PinState pin_state) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, pin_state);
}

/**
 * @brief 
 * @param None
 * @retval None
 */
void gpio_pin_PA2(GPIO_PinState pin_state) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, pin_state);
}
