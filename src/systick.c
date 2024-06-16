#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "systick.h"
#include "util.h"

static volatile uint32_t tick_count = 0;

void SysTick_Handler(void) {
    tick_count++;
}

void Systick_Init(uint32_t time_value) {
    // Amount of time clock gets reset to after each tick (equal to time between ticks)
    SysTick->LOAD = time_value - 1;

    // Initialize value to 0 to immediately trigger interrupt
    SysTick->VAL = 0; 

    // Enables SysTick, configures to use internal clock & enable tick interrupts
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;

}

uint32_t Systick_GetTicks(void) {
    return tick_count;
}