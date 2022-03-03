/**
 ******************************************************************************
 * @file			: spi_slav_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: SPI3 Slave Register File
 ******************************************************************************
*/

/* Include Header */
#include "spi_slave_reg.h"

/* HandleType Definitions */
SPI_HandleTypeDef hspi3;

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
void SPI3_Init(void) {
	/* SPI3 parameter configuration*/
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_SLAVE;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 7;
	hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	if (HAL_SPI_Init(&hspi3) != HAL_OK) {
		Error_Handler();
	}

	// Init SPI
  	HAL_SPI_Init(&hspi3);
}

/**
  * @brief SPI3 Get Char Function
  * @param None
  * @retval char
  */
unsigned char SPI_Slave_Get_Char(void) {
	unsigned char spiData[1];
	HAL_SPI_Receive(&hspi3, spiData, 1, 10);
	return *spiData;
}
