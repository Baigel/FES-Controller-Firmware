/**
******************************************************************************
 * @file                fes_slave_spi.hpp
 * @author              Ahmed Hafeez-Baig
 * @date                20062021
 * @brief               Receiving spi messages task
******************************************************************************
*/

#ifndef __FES_SLAVE_SPI__
#define __FES_SLAVE_SPI__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32l4xx_hal.h"

/* Library Includes */
#include "fes_wavegen.hpp"
#include "dac_reg.h"
#include "spi_reg.h"

/* Global Variables extern */
extern uint8_t RX_Buffer[10];

/* Task Function Prototypes */
void fes_slave_SPI_task_init(void);
void fes_slave_SPI_task(void);

#ifdef __cplusplus
}
#endif

#endif

