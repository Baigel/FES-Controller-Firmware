/**
 ******************************************************************************
 * @file			: spi_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: SPI Register File
 ******************************************************************************
*/

/* Includes */
#include "spi_reg.h"

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi3_rx;

/* SPI1 init function */
void SPI1_Init(void)
{
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
}

/* SPI3 init function */
void SPI3_Init(void)
{
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_SLAVE;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 7;
	hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	if (HAL_SPI_Init(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (spiHandle->Instance == SPI1)
	{
		/* SPI1 clock enable */
		__HAL_RCC_SPI1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/* SPI1 GPIO Configuration
		PA1     ------> SPI1_SCK
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI
		PB0     ------> SPI1_NSS */
		GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	else if (spiHandle->Instance == SPI3)
	{
		/* SPI3 clock enable */
		__HAL_RCC_SPI3_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/* SPI3 GPIO Configuration
		PB3 (JTDO-TRACESWO)     ------> SPI3_SCK
		PB4 (NJTRST)     ------> SPI3_MISO
		PB5     ------> SPI3_MOSI */
		GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* SPI3 DMA Init */
		/* SPI3_RX Init */
		hdma_spi3_rx.Instance = DMA2_Channel1;
		hdma_spi3_rx.Init.Request = DMA_REQUEST_3;
		hdma_spi3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_spi3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_spi3_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_spi3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_spi3_rx.Init.Mode = DMA_NORMAL;
		hdma_spi3_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
		if (HAL_DMA_Init(&hdma_spi3_rx) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(spiHandle, hdmarx, hdma_spi3_rx);

		/* SPI3 interrupt Init */
		//HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
		//HAL_NVIC_EnableIRQ(SPI3_IRQn);
	}
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{
	if (spiHandle->Instance == SPI1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_SPI1_CLK_DISABLE();

		/* SPI1 GPIO Configuration
		PA1     ------> SPI1_SCK
		PA6     ------> SPI1_MISO
		PA7     ------> SPI1_MOSI
		PB0     ------> SPI1_NSS */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_7);

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
	}
	else if (spiHandle->Instance == SPI3)
	{
		/* Peripheral clock disable */
		__HAL_RCC_SPI3_CLK_DISABLE();

		/* SPI3 GPIO Configuration
		PB3 (JTDO-TRACESWO)     ------> SPI3_SCK
		PB4 (NJTRST)     ------> SPI3_MISO
		PB5     ------> SPI3_MOSI */
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

		/* SPI3 DMA DeInit */
		HAL_DMA_DeInit(spiHandle->hdmarx);
	}
}

/**
  * @brief 
  * @param 
  * @retval 
  */
void SPI_DMA_Rx(volatile uint8_t *buffer, uint8_t size)
{
	HAL_SPI_Receive_DMA(&hspi3, buffer, size);
}

/**
 * @brief 
 * @param None
 * @retval None
 */
bool SPI_DMA_TC_flag_set(void)
{
	return __HAL_DMA_GET_FLAG(&hdma_spi3_rx, DMA_FLAG_HT1);
}
