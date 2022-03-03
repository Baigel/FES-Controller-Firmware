/**
******************************************************************************
 * @file                fes_wavegen.hpp
 * @author              Ahmed Hafeez-Baig
 * @date                14042021
 * @brief               Wave generation class definitions
******************************************************************************
*/

#ifndef __FES_CURRENT_MEASURING__
#define __FES_CURRENT_MEASURING__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include "math.h"

/* Library Includes */
#include "adc_reg.h"

// TEMPORARY
#include "uart_reg.h"

/* External Function Prototypes */
void current_measuring_task_init(void);

#ifdef __cplusplus
}
#endif

#endif
