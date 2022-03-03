/**
  ******************************************************************************
  * @file			: stm32l4xx_it.c
  * @brief			: Interrupt Header File
  * @author			: Ahmed Hafeez-Baig
  ******************************************************************************
*/

#ifndef __STM32L4xx_IT_H__
#define __STM32L4xx_IT_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Exported Functions Prototypes */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void TIM1_UP_TIM16_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void DMA2_Channel4_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32L4xx_IT_H__ */
