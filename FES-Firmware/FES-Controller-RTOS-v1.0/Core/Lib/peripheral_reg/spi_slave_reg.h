/**
 ******************************************************************************
 * @file			: dac_init.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: DAC Register File
 ******************************************************************************
*/

#ifndef __SPI_SLAVE_REG__
#define __SPI_SLAVE_REG__

/* Include Libraries */
#include "stm32l4xx_hal.h"
#include "main.h" // For Error_Handle

/* Library Includes */
#include "spi_msp.h"

/* SPI Slave Function Prototypes */
void SPI3_Init(void);
unsigned char SPI_Slave_Get_Char(void);

#endif
