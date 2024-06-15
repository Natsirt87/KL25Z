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

int main(void) {
    GPIO_Init();
    Interrupt_Init();
    Util_Init_Onboard_LED();

    // Intialize onboard LED to off
    Util_Control_Onboard_LED(0, 0, 0);

    SystemCoreClockUpdate();
    Systick_Init(SystemCoreClock / 1000); // 1ms tick
    Debug_Init(57600);

    // scheduler init
    Scheduler_Init();

    // add scheduler tasks
    Scheduler_Add_Task(TASK_SEQUENCE);
    Scheduler_Add_Task(TASK_FLASH);

    // run scheduler
    Scheduler_Run();

    return 0;
}