/**
******************************************************************************
 * @file                fes_slave_spi.cpp
 * @author              Ahmed Hafeez-Baig
 * @date                20062021
 * @brief               Receiving spi messages task
******************************************************************************
*/

/* Include Header File */
#include "fes_slave_spi.hpp"

// TEMPORARY
#include "uart_reg.h"

/* Macro Definitions */

/* Task Definition */
osThreadId_t slaveSPITaskHandle;
static const osThreadAttr_t slaveSPITask_attributes = {
  .name = "slaveSPITask",
  .stack_size = 128 * 2,
  .priority = (osPriority_t) osPriorityAboveNormal1
};

/* Task Function Prototype */
static void slave_SPI_task(void *argument);

/* Internal Function Prototypes */
static WaveGenStruct get_wave_SPI(void);
static uint8_t get_clear_channel(void);

/* External Function Definitions */

/**
 * @brief Initialise hardware and wavegen task
 * @param None
 * @retval None
 */
void slave_SPI_task_init(void) {

	// Hardware initialisations
	SPI3_Init();
	GPIO_Init();

	// TEMPORARY
	USART2_UART_Init();

	// Create task
	slaveSPITaskHandle = osThreadNew(slave_SPI_task, NULL, &slaveSPITask_attributes);
}

/* Task Definition */

/**
 * @brief Function implementing the slaveSPI thread
 * @param argument: Not used
 * @retval None
 */
static void slave_SPI_task(void *argument) {
	/* Initialisation */

	// TEMPORARY: Send test char
	UART_Send_Char('S');

	// Declare input buffer
	unsigned char in;
	
	/* Infinite loop */
	for(;;) {

		in = SPI_Slave_Get_Char();

		if (in == 'W') { // Flag for incomming wave packet
			UART_Send_Char(in);
			add_to_wavegen_queue(get_wave_SPI());
		} else if (in == 'S') { // Flag for Enabling stimulation
			DAC_Enable();
		} else if (in == 'D') { // Flag for Disabling stimulation
			DAC_Disable();
		} else if (in == 'C') { // Flag for Clearing Channel
			add_to_channel_clear_queue(get_clear_channel());
		} else if (in == 'E') { // Flag for error occurance
			
		}
		in = ' '; // Clear buffer
	}
}

/* Internal Function Definitions */

/**
 * @brief 
 * @param 
 * @retval None
 */
static WaveGenStruct get_wave_SPI(void) {
	// Define variables
	WaveGenStruct waveGenTransmit;
	unsigned char in;

	// Get wave information from SPI
	in = SPI_Slave_Get_Char(); // Wave pulse type
	UART_Send_Char(in);
	switch (in) {
		case 's':
			waveGenTransmit.waveType = PULSE_SINE;
			break;
		case 'q':
			waveGenTransmit.waveType = PULSE_SQUARE;
			break;
		case 'a':
			waveGenTransmit.waveType = PULSE_SAWTOOTH;
			break;
		case 't':
			waveGenTransmit.waveType = PULSE_TRIANGLE;
			break;
	}
	in = SPI_Slave_Get_Char(); // Channel
	waveGenTransmit.channel = (uint32_t)in - 1;
	in = SPI_Slave_Get_Char(); // Frequency
	waveGenTransmit.frequency = (uint32_t)in;
	in = SPI_Slave_Get_Char(); // Amplitude
	waveGenTransmit.amplitude = (uint32_t)(((uint32_t)in + 1) * 16) - 1;
	in = SPI_Slave_Get_Char(); // Pulse width
	waveGenTransmit.width = (uint32_t)in * 4;
	// See if a envelope is being used
	in = SPI_Slave_Get_Char(); // Using envelope?
	if (in == 't') {
		waveGenTransmit.envelopeOnOff = true;
		UART_Send_Char(in);
		in = SPI_Slave_Get_Char(); // Ramp in period
		UART_Send_Char(in);
		waveGenTransmit.rampIn = 10 * in;
		in = SPI_Slave_Get_Char(); // Ramp out period
		UART_Send_Char(in);
		waveGenTransmit.rampOut = 10 * in;
		in = SPI_Slave_Get_Char(); // Envelope width
		UART_Send_Char(in);
		waveGenTransmit.envelopeWidth = 50 * in;
	} else {
		waveGenTransmit.envelopeOnOff = false;
		waveGenTransmit.rampIn = 0;
		waveGenTransmit.rampOut = 0;
		waveGenTransmit.envelopeWidth = 0;
	}

	// Return
	return waveGenTransmit;
}

/**
 * @brief 
 * @param 
 * @retval None
 */
static uint8_t get_clear_channel(void) {
	// Define variables
	unsigned char in;

	// Get wave information from SPI
	in = SPI_Slave_Get_Char();
	return (uint8_t)in;
}
