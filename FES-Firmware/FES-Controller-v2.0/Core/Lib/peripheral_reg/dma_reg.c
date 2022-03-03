/**
 ******************************************************************************
 * @file			: dma_reg.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: DMA Register File
 ******************************************************************************
*/

/* Includes */
#include "dma_reg.h"

/**
 * @brief Enable DMA controller clock
 * @param None
 * @retval None
 */
void DMA_Init(void) {
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	/* DMA2_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
}

