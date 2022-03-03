/**
 ******************************************************************************
 * @file			: spi_reg.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: SPI Register File
 ******************************************************************************
*/

#ifndef __SPI_REG_H__
#define __SPI_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "main.h"
#include <stdbool.h>

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi3;

void SPI1_Init(void);
void SPI3_Init(void);
void SPI_DMA_Rx(volatile uint8_t* buffer, uint8_t size);
bool SPI_DMA_TC_flag_set(void);

#ifdef __cplusplus
}
#endif

#endif
