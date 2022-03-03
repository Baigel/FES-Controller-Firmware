/**
 ******************************************************************************
 * @file			: usart_reg.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: USART Register File
 ******************************************************************************
*/

#ifndef __USART_REG_H__
#define __USART_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern UART_HandleTypeDef huart2;

void USART2_UART_Init(void);
void UART_Send_Char(unsigned char send);

#ifdef __cplusplus
}
#endif

#endif
