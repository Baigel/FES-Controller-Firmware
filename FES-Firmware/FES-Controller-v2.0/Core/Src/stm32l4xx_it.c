/**
 ******************************************************************************
 * @file			: stm32l4xx_it.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 14-04-2021
 * @brief			: 
 ******************************************************************************
*/

/* Includes */
#include "main.h"
#include "fes_wavegen.hpp"
#include "stm32l4xx_it.h"

/* External variables */
extern DMA_HandleTypeDef hdma_dac_ch1;
extern DMA_HandleTypeDef hdma_dac_ch2;
extern DMA_HandleTypeDef hdma_spi3_rx;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
	while (1) {;}
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
	while (1) {;}
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void) {
	while (1) {;}
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void) {
	while (1) {;}
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
	while (1) {;}
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void) {
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void) {
	HAL_IncTick();
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void) {
	DMA_transfer_interrupt_callback();
	HAL_DMA_IRQHandler(&hdma_dac_ch1);
}

/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_dac_ch2);
}

/**
  * @brief This function handles DMA2 channel1 global interrupt.
  */
void DMA2_Channel1_IRQHandler(void) {
	SPI3_DMA_callback();
	HAL_DMA_IRQHandler(&hdma_spi3_rx);
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim2);
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim7);
	TIM7_callback();
}

/**
  * @brief This function handles SPI3 global interrupt.
  */
void SPI3_IRQHandler(void) {
	HAL_SPI_IRQHandler(&hspi3);
}
