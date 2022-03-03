/**
 ******************************************************************************
 * @file			: stm32l4xx_it.c.h
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: 
 ******************************************************************************
*/

/* Includes */
#include "main.h"
#include "stm32l4xx_it.h"

/* Libraries with callback functions */
#include "dac_dma_reg.h"

#include "gpio_reg.h"

/* External variables */
extern DMA_HandleTypeDef hdma_dac_ch1;
extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim1;
extern DMA_HandleTypeDef hdma_spi1_tx;

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
	while (1) {

	}
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
	while (1) {

	}
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void) {
	while (1) {

	}
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void) {
	while (1) {

	}
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
	while (1) {

	}
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/******************************************************************************/

/**
  * @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
  */
void TIM1_UP_TIM16_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim1);
}

/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void) {
	//DMA_interrupt_callback();
	HAL_DMA_IRQHandler(&hdma_dac_ch1);
}

/**
  * @brief This function handles TIM6 global interrupt, DAC channel1 and 
  * channel2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void) {
	HAL_DAC_IRQHandler(&hdac1);
}
