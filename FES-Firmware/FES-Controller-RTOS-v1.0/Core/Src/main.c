/**
  ******************************************************************************
  * @file			: main.c
  * @brief			: Main program body
  * @author			: Ahmed Hafeez-Baig
  ******************************************************************************
*/

/* Includes */
#include "main.h"
#include "cmsis_os.h"
#include "gpio_reg.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* MCU Configuration */

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();
	SystemCoreClockUpdate();

	/* Init scheduler */
	osKernelInitialize();

	/* Create the threads */
	wavegen_task_init();
	slave_SPI_task_init();
	//current_measuring_task_init();

	/* Start scheduler */
	osKernelStart();

	/* Infinite loop */
	while (1) {;}
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
  __disable_irq();
  while (1) {;}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif
