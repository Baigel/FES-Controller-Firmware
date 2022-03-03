/**
 ******************************************************************************
 * @file			: uasrt_init.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: USART Register File
 ******************************************************************************
*/

#ifndef __UART_REG__
#define __UART_REG__

/* Include Libraries */
#include "stm32l4xx_hal.h"
#include "main.h"

// TEMPORARY
#include "adc_reg.h"

/* USART Function Prototypes */
void USART2_UART_Init(void);
void UART_Send_Char(unsigned char send);

void print_adc(void);

#endif
