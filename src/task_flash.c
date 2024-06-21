#include "task_defs.h"
#include "task.h"
#include "debug.h"
#include "util.h"
#include "scheduler.h"

static enum {ST_ON, ST_OFF} state;

static bool mode_flash = false;
static uint32_t wait_time = 500;

static void HandleEvent(event_t*);

void Task_Flash(void) {
    event_t received_event;

    while (Task_ReadEvent(TASK_FLASH, &received_event)) {
        HandleEvent(&received_event);
    }

    if (!mode_flash) {
        return;
    }

    switch (state) {
        case ST_ON:
            Debug_Println("Flash ON");
            Util_ControlOnboardLED(1, 0, 1);
            
            state = ST_OFF;
            Scheduler_Delay(wait_time);
            break;
        case ST_OFF:
            Debug_Println("Flash OFF");
            Util_ControlOnboardLED(0, 0, 0);
            
            state = ST_ON;
            Scheduler_Delay(wait_time);
            break;
        default:
            state = ST_ON;
            break;
    }
}

static void HandleEvent(event_t *received_event) {
    switch (received_event->id) {
        
        case EVENT_BTN_MODE:
            Debug_Printf("Task Flash reading event EVENT_BTN_MODE with data %d\r\n", received_event->data.btn_mode);

            mode_flash = received_event->data.btn_mode == 1;
            break;
        case EVENT_BTN_TIMING:
            Debug_Printf("Task Flash reading event EVENT_BTN_TIMING with data %d\r\n", received_event->data.btn_timing);

            if (received_event->data.btn_timing == 0) {
                wait_time = 500;
            } else {
                wait_time = 200;
            }
        default:
            break;
    }
}