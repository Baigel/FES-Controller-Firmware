/**
 ******************************************************************************
 * @file			: clock.c
 * @author			: Ahmed Hafeez-Baig
 * @date			: 03-12-2021
 * @brief			: Clock File
 ******************************************************************************
*/

/* Includes */
#include "clock.h"

/* Functions copied from PlatformIO libraries */

static inline uint64_t LL_SYSTICK_IsActiveCounterFlag(void) {
	return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}

uint64_t getCurrentMicros(void) {
	/* Ensure COUNTFLAG is reset by reading SysTick control and status register */
	LL_SYSTICK_IsActiveCounterFlag();
	uint64_t m = HAL_GetTick();
	const uint64_t tms = SysTick->LOAD + 1;
	__IO uint64_t u = tms - SysTick->VAL;
	if (LL_SYSTICK_IsActiveCounterFlag()) {
		m = HAL_GetTick();
		u = tms - SysTick->VAL;
	}
	return (m * 1000 + (u * 1000) / tms);
}
