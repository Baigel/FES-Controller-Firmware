/**
******************************************************************************
 * @file                fes_current_measuring.cpp
 * @author              Ahmed Hafeez-Baig
 * @date                14042021
 * @brief               Wave generation task
******************************************************************************
*/

/* Include Header File */
#include "fes_current_measuring.hpp"

/* Task Definition */
osThreadId_t currentMeasuringTaskHandle;
const osThreadAttr_t currentMeasuringTask_attributes = {
  .name = "currentMeasuringTask",
  .stack_size = 128 * 2,
  .priority = (osPriority_t) osPriorityAboveNormal4
};

/* Task Function Prototype */
static void current_measuring_task(void *argument);

/* External Function Definitions */

/**
 * @brief Initialise hardware and wavegen task
 * @param None
 * @retval None
 */
void current_measuring_task_init(void) {

	// Hardware Initialisations
	ADC1_Init();

	// TEMPORARY
	USART2_UART_Init();
	GPIO_Init();

	// Create task
	currentMeasuringTaskHandle = osThreadNew(current_measuring_task, NULL, &currentMeasuringTask_attributes);
}

/* Task Definition */

/**
 * @brief Function implementing the WavegenTask thread
 * @param argument: Not used
 * @retval None
 */
static void current_measuring_task(void *argument) {
	/* Initialisation */

	for (;;) {
		// Read ADC
		(void)Get_ADC();
		// Delay to allow potential context switch to occur
		uint32_t bufferTimer = osKernelGetSysTimerCount();
		while (bufferTimer + 1000 > osKernelGetSysTimerCount()) {};
	}

}
