/**
 ******************************************************************************
 * @file			: spi_slav_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: SPI3 Slave Register File
 ******************************************************************************
*/

/* Include Header */
#include "spi_master_reg.h"

/* Peripheral Handle Definitions */
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

/**
 * @brief 
 * @param 
 * @retval None
 */
void SPI1_Init(void) {
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}

	// Init SPI
	HAL_SPI_Init(&hspi1);
}

/**
 * @brief 
 * @param 
 * @retval None
 */
void SPI_master_send(uint16_t* switchState, uint8_t size) {
	// Send new state to switching matrix
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);
	HAL_SPI_Transmit(&hspi1, (uint8_t*)switchState, size, HAL_MAX_DELAY);
}
