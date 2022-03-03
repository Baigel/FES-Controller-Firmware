/**
 ******************************************************************************
 * @file			: stm32l4xx_it.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: 
 ******************************************************************************
*/

/* Define to prevent recursive inclusion */
#ifndef __STM32L4xx_IT_H
#define __STM32L4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported functions prototypes */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA2_Channel1_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM7_IRQHandler(void);
void SPI3_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif
