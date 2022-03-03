/**
  ******************************************************************************
  * @file			: main.h
  * @brief			: Main program body
  * @author			: Ahmed Hafeez-Baig
  ******************************************************************************
*/

#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32l4xx_hal.h"

/* Library Includes */
#include "dac_dma_reg.h"
#include "gpio_reg.h"
#include "spi_slave_reg.h"
#include "sys_clock_reg.h"
#include "uart_reg.h"

#include "fes_wavegen.hpp"
#include "fes_slave_spi.hpp"
#include "fes_current_measuring.hpp"

/* Exported Functions Prototypes */
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
