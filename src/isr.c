#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "board_config.h"
#include "isr.h"
#include "task.h"
#include "event.h"
#include "debug.h"

void PORTA_IRQHandler(void) {
    // Button 1 generated the interrupt
    if (BTN_PORT->ISFR & MASK(BTN1_POS)) {
        if (BTN1_GPIO->PDIR & MASK(BTN1_POS)) {
            Debug_Println("Button one released");

            event_t mode_event;
            mode_event.id = EVENT_BTN_MODE;
            mode_event.data.btn_mode = 0;

            Task_MulticastEvent(&mode_event);
        } else {
            Debug_Println("Button one pressed");

            event_t mode_event;
            mode_event.id = EVENT_BTN_MODE;
            mode_event.data.btn_mode = 1;

            Task_MulticastEvent(&mode_event);
        }
    }

    // Button 2 generated the interrupt
    if (BTN_PORT->ISFR & MASK(BTN2_POS)) {
        if (BTN1_GPIO->PDIR & MASK(BTN2_POS)) {
            Debug_Println("Button two released");

            event_t timing_event;
            timing_event.id = EVENT_BTN_TIMING;
            timing_event.data.btn_timing = 0;

            
            Task_MulticastEvent(&timing_event);
        } else {
            Debug_Println("Button two pressed");

            event_t timing_event;
            timing_event.id = EVENT_BTN_TIMING;
            timing_event.data.btn_timing = 1;

            Task_MulticastEvent(&timing_event);
        }
    }

    // Clear the interrupt status flags
    BTN_PORT->ISFR = 0xffffffff;
}