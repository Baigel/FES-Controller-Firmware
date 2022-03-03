/**
  ******************************************************************************
  * @file			: main.h
  * @brief			: Main program body header
  * @author			: Ahmed Hafeez-Baig
  ******************************************************************************
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Peripheral Includes */
#include "stm32l4xx_hal.h"
#include "adc_reg.h"
#include "dac_reg.h"
#include "dma_reg.h"
#include "spi_reg.h"
#include "tim_reg.h"
#include "gpio_reg.h"
#include "usart_reg.h"

/* Task Library Includes */
#include "fes_wavegen.hpp"
#include "fes_slave_spi.hpp"

/* Exported functions prototypes */
void Error_Handler(void);


void SPI3_DMA_callback(void);

#ifdef __cplusplus
}
#endif

#endif
