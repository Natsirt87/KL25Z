#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "board_config.h"
#include "isr.h"
#include "task.h"
#include "event.h"

void PORTA_IRQHandler(void) {
    // Button 1 generated the interrupt
    if (PORTA->ISFR & MASK(BTN1_POS)) {

        if (BTN1_GPIO->PDIR & MASK(BTN1_POS)) {
            // Button is not pressed
            event_t timing_event;
            timing_event.id = EVENT_BTN_TIMING;
            timing_event.data.btn_timing = 0;

            Task_SendEvent(TASK_SEQUENCE, &timing_event);
        } else {
            // Button is pressed
        }
    }

    // Button 2 generated the interrupt
    if (PORTA->ISFR & MASK(BTN2_POS)) {

    }


    // Clear the interrupt status flags
    PORTA->ISFR = 0xffffffff;
}