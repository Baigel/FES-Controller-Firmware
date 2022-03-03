/**
 ******************************************************************************
 * @file			: dac_init.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: DAC Register File
 ******************************************************************************
*/

#ifndef __SPI_MASTER_REG__
#define __SPI_MASTER_REG__

/* Includes */
#include "stm32l4xx_hal.h"
#include "main.h" // For Error_Handle()

/* Library Includes */
#include "spi_msp.h"

/* SPI Slave Function Prototypes */
void SPI1_Init(void);
void SPI_DMA_Init(void);
void SPI_master_send(uint16_t* switchState, uint8_t size);

#endif
