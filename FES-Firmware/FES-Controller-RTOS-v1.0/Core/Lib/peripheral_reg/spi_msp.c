/**
 ******************************************************************************
 * @file			: spi_msp.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: SPI MSP File
 ******************************************************************************
*/

/* Include Header */
#include "spi_msp.h"

extern DMA_HandleTypeDef hdma_spi1_tx;

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi) {
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(hspi->Instance==SPI3) {
		/* Peripheral clock enable */
		__HAL_RCC_SPI3_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/** SPI3 GPIO Configuration
		PB3 (JTDO-TRACESWO)     ------> SPI3_SCK
		PB4 (NJTRST)     ------> SPI3_MISO
		PB5     ------> SPI3_MOSI */
		GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	} else if(hspi->Instance==SPI1) {
		/* Peripheral clock enable */
		__HAL_RCC_SPI1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**SPI1 GPIO Configuration
		PA1     ------> SPI1_SCK
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI
		PB0     ------> SPI1_NSS */
		GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi) {
	if (hspi->Instance==SPI3) {
		/* Peripheral clock disable */
		__HAL_RCC_SPI3_CLK_DISABLE();

		/** SPI3 GPIO Configuration
		PB3 (JTDO-TRACESWO)     ------> SPI3_SCK
		PB4 (NJTRST)     ------> SPI3_MISO
		PB5     ------> SPI3_MOSI */
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
		
	} else if (hspi->Instance==SPI1) {
		/* Peripheral clock disable */
		__HAL_RCC_SPI1_CLK_DISABLE();

		/**SPI1 GPIO Configuration
		PA1     ------> SPI1_SCK
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI
		PB0     ------> SPI1_NSS */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_6|GPIO_PIN_7);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);

	}
}
