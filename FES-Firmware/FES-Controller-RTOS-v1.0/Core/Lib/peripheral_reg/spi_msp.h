/**
 ******************************************************************************
 * @file			: spi_msp.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: SPI MSP File
 ******************************************************************************
*/

#ifndef __SPI_MSP__
#define __SPI_MSP__

/* Include Libraries */
#include "stm32l4xx_hal.h"
#include "main.h" // For Error_Handle()

/* External Function Prototypes */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi);

#endif
