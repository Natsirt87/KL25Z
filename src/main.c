#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "pin_config.h"
#include "systick.h"
#include "util.h"
#include "scheduler.h"
#include "tasks.h"
#include "debug.h"

void GPIO_Init(void) {
    // Enable clock
    SIM->SCGC5 |= SCGC5_MASK;

    // Configure buttons as GPIO w/ pull-up resistors
    BTN1_PORT->PCR[BTN1_POS] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    BTN2_PORT->PCR[BTN2_POS] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

    // Configure button GPIO as input
    BTN1_GPIO->PDDR &= ~MASK(BTN1_POS);
    BTN2_GPIO->PDDR &= ~MASK(BTN2_POS);
}

void Interrupt_Init(void) {
    // Configure button interrupts on either edge
    BTN1_PORT->PCR[BTN1_POS] |= PORT_PCR_IRQC(11); // Set interrupt on either edge
    BTN2_PORT->PCR[BTN2_POS] |= PORT_PCR_IRQC(11); // Set interrupt on either edge

    NVIC_SetPriority(BTN_IRQn, 2);
    NVIC_EnableIRQ(BTN_IRQn);
    NVIC_ClearPendingIRQ(BTN_IRQn);

    __enable_irq();
}

void Clock_Init(void) {
    // MCG_C1: CLKS (bit 7-6) = 00
    MCG->C1 |= MCG_C1_CLKS(0);  // Select PLL/FLL as clock source
     
    // MCG_C1: IREFS (bit 2)  = 1
    MCG->C1 |= MCG_C1_IREFS(1); // Select Inernal Reference clock
                                // source for FLL
     
    // MCG_C4: DRST_DRS (bit 6-5) = 01
    MCG->C4 |= MCG_C4_DRST_DRS(1); // Select DCO range as Mid range
                                   // DCO generates the FLL Clock
     
    // MCG_C4: DMX32 (bit 7) = 1
    MCG->C4 |= MCG_C4_DMX32(1);    // Select DCO frequency as 48Mhz
}

int main(void) {
    Clock_Init();
    SystemCoreClockUpdate();
    Systick_Init(SystemCoreClock / 1000); // 1ms tick

    GPIO_Init();
    Interrupt_Init();

    Debug_Init(57600);
    Util_InitOnboardLED();

    // Intialize onboard LED to off
    Util_ControlOnboardLED(0, 0, 0);
    Debug_Printf("System core clock: %d \r\n", SystemCoreClock);

    // scheduler init
    Scheduler_Init();

    // add scheduler tasks
    Scheduler_AddTask(TASK_SEQUENCE);
    Scheduler_AddTask(TASK_FLASH);

    // run scheduler
    Scheduler_Run();

    return 0;
}