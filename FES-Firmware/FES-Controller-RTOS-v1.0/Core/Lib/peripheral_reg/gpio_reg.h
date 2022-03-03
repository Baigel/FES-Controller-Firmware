/**
 ******************************************************************************
 * @file			: gpio_init.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: GPIO Register File
 ******************************************************************************
*/

#ifndef __GPIO_REG__
#define __GPIO_REG__

/* Include Libraries */
#include "stm32l4xx_hal.h"
#include "main.h"

/* GPIO External Function Prototypes */
void GPIO_Init(void);
// Set/Reset Functions
void gpio_pin_PB6(GPIO_PinState pin_state);
void gpio_pin_PB7(GPIO_PinState pin_state);
void gpio_pin_PA11(GPIO_PinState pin_state);
void gpio_pin_PA2(GPIO_PinState pin_state);

#endif
