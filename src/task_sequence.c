#include "task_sequence.h"
#include "task.h"
#include "debug.h"
#include "util.h"
#include "scheduler.h"

static enum {ST_RED, ST_GREEN, ST_BLUE} state;

static bool mode_sequence = true;
static uint32_t wait_time = 1000;

static void HandleEvent(event_t*);

void Task_Sequence(void) {
    event_t received_event;

    // Read all incoming events & react accordingly
    while(Task_ReadEvent(TASK_SEQUENCE, &received_event)) {
        HandleEvent(&received_event);
    }

    if (!mode_sequence) {
        return;
    }

    switch(state)
    {
        case ST_RED:
            Debug_Println("Stage RED");
            Util_ControlOnboardLED(1, 0, 0);
            
            state = ST_GREEN;
            Scheduler_Delay(wait_time);
            break;
        case ST_GREEN:
            Debug_Println("Stage GREEN");
            Util_ControlOnboardLED(0, 1, 0);
            
            state = ST_BLUE;
            Scheduler_Delay(wait_time);
            break;
        case ST_BLUE:
            Debug_Println("Stage BLUE");
            Util_ControlOnboardLED(0, 0, 1);
            
            state = ST_RED;
            Scheduler_Delay(wait_time);
            break;
        default:
            state = ST_RED;
            break;
    }
}

static void HandleEvent(event_t *received_event) {
    switch (received_event->id) {

        case EVENT_BTN_MODE:
            Debug_Printf("Task Sequence reading event EVENT_BTN_MODE with data %d\r\n", received_event->data.btn_mode);

            mode_sequence = received_event->data.btn_mode == 0;
            break;

        case EVENT_BTN_TIMING:
            Debug_Printf("Task Sequence reading event EVENT_BTN_MODE with data %d\r\n", received_event->data.btn_timing);

            if (received_event->data.btn_timing == 0) {
                wait_time = 1000;
            } else {
                wait_time = 500;
            }
            break;

        default:
            break;
    }
}