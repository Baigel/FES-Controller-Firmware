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
#include "cmsis_os.h"

/* Library Includes */
#include "fes_wavegen.hpp"
#include "dac_dma_reg.h"
#include "spi_slave_reg.h"
#include "gpio_reg.h"

/* External Function Prototypes */
void slave_SPI_task_init(void);

#ifdef __cplusplus
}
#endif

#endif

