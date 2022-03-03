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
//#include "uart_reg.h"

/* Constants */
static constexpr uint8_t SPI_RX_BUFFER_SIZE = 20;

/* Global Variables */
volatile uint8_t Rx_Buffer[SPI_RX_BUFFER_SIZE] = {0};
static bool SPI_Rx_flag = false;

/* Internal Function Prototypes */
static WaveGenStruct get_wave_SPI(void);
static uint8_t get_clear_channel(void);

/* Task Function Definitions */

/**
 * @brief Initialise hardware and wavegen task
 * @param None
 * @retval None
 */
void fes_slave_SPI_task_init(void) {

	// Hardware initialisations
	SPI3_Init();
	GPIO_Init();

	// Start SPI Receive on DMA
    SPI_DMA_Rx(Rx_Buffer, SPI_RX_BUFFER_SIZE);

	// TEMPORARY
	USART2_UART_Init();
}

/**
 * @brief Function implementing the slaveSPI thread
 * @param argument: Not used
 * @retval None
 */
void fes_slave_SPI_task(void) {
	/* Initialisation */
	
	if (SPI_Rx_flag) {

		UART_Send_Char(Rx_Buffer[0]);

		// FIXME: fix buffer handle code running before DMA finishes filling buffer
		// current solution waits for buffer to fill up manually and clears the buffer
		// afterwards manually as well.
		// This code below works, but it is a bodge. Need to ensure it only runs *after*
		// SPI DMA finishes to have most efficient working.
		
		if (Rx_Buffer[0] == 'W') { // Flag for incoming wave packet
			while (Rx_Buffer[10] == 0) {;} // Wait for buffer to fill up
			new_wave(get_wave_SPI());
		} else if (Rx_Buffer[0] == 'S') { // Flag for Enabling stimulation
			DAC_Enable();
		} else if (Rx_Buffer[0] == 'D') { // Flag for Disabling stimulation
			DAC_Disable();
		} else if (Rx_Buffer[0] == 'C') { // Flag for Clearing Channel
			while (Rx_Buffer[1] == 0) {;}
			clear_wave(get_clear_channel());
		} else if (Rx_Buffer[0] == 'E') { // Flag for error occurrence
			//while (Rx_Buffer[5] == 0) {;}
		}

		// Clear Buffer
		for (uint8_t i = 0; i < 10; i++) {
			Rx_Buffer[i] = 0;
		}

		// Restart DMA Rx
		HAL_SPI_Abort(&hspi3);
		HAL_SPI_DMAStop(&hspi3);
		SPI_Rx_flag = false;
		SPI_DMA_Rx(Rx_Buffer, SPI_RX_BUFFER_SIZE);
	}
}

/* Internal Function Definitions */

// TODO: Add dual channel control to spi

/**
 * @brief 
 * @param 
 * @retval None
 */
static WaveGenStruct get_wave_SPI(void) {
	// Define variables
	WaveGenStruct waveGenTransmit;

	// Get wave information from SPI
	switch (Rx_Buffer[1]) {
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
	waveGenTransmit.channel = static_cast<uint16_t>(Rx_Buffer[2]) - 1;
	waveGenTransmit.frequency = static_cast<uint16_t>(Rx_Buffer[3]);
	waveGenTransmit.amplitude = static_cast<uint16_t>(Rx_Buffer[4]) * 40; // TODO: get amplitude feedback working, so that this scaler is not needed
	waveGenTransmit.width = static_cast<uint16_t>(Rx_Buffer[5]) + (static_cast<uint16_t>(Rx_Buffer[6]) << 8);
	// Import interphase delay
	if (Rx_Buffer[7] == 't') {
		waveGenTransmit.bipolar = true;
		waveGenTransmit.interphaseDelay = static_cast<uint16_t>(Rx_Buffer[8]) + (static_cast<uint16_t>(Rx_Buffer[9]) << 8);
	} else {
		waveGenTransmit.bipolar = false;
		waveGenTransmit.interphaseDelay = 0;
	}
	// See if a envelope is being used
	if (Rx_Buffer[10] == 't') {
		waveGenTransmit.envelopeOnOff = true;
		waveGenTransmit.rampIn = static_cast<uint16_t>(Rx_Buffer[11]) + (static_cast<uint16_t>(Rx_Buffer[12]) << 8);
		waveGenTransmit.rampOut = static_cast<uint16_t>(Rx_Buffer[13]) + (static_cast<uint16_t>(Rx_Buffer[14]) << 8);
		waveGenTransmit.envelopeWidth = static_cast<uint16_t>(Rx_Buffer[15]) + (static_cast<uint16_t>(Rx_Buffer[16]) << 8);
	} else {
		waveGenTransmit.envelopeOnOff = false;
		waveGenTransmit.rampIn = 0;
		waveGenTransmit.rampOut = 0;
		waveGenTransmit.envelopeWidth = 0;
	}

	return waveGenTransmit;
}

/**
 * @brief 
 * @param 
 * @retval None
 */
static uint8_t get_clear_channel(void) {
	// Get wave information from SPI
	return static_cast<uint8_t>(Rx_Buffer[1]);
}

/* Interrupt Callback Functions */

/**
  * @brief 
  * @param 
  * @retval 
  */
void SPI3_DMA_callback(void) {
	if (SPI_DMA_TC_flag_set()) {
		//while (true) {;}
	}

	SPI_Rx_flag = true;
}

