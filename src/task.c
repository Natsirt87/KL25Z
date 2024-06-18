#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "task.h"
#include "util.h"
#include "debug.h"
#include "scheduler.h"

static event_buffer_t event_buffers[NUM_TASKS];

/* 
    TODO: Split each task into it's own file. 
    Make each state it's own function, and use a separate function to process each event. 
    Keep static variables in the file.
*/

void Task_Sequence(void) {
    static enum {ST_RED, ST_GREEN, ST_BLUE} state;

    static bool mode_sequence = true;
    static uint32_t wait_time = 1000;
    
    
    event_t received_event;

    // Read all incoming events & react accordingly
    while(Task_ReadEvent(TASK_SEQUENCE, &received_event)) {
        switch (received_event.id) {
            case EVENT_BTN_MODE:
                Debug_Printf("Task Sequence reading event EVENT_BTN_MODE with data %d\r\n", received_event.data.btn_mode);
                mode_sequence = received_event.data.btn_mode == 0;
                break;
            case EVENT_BTN_TIMING:
                Debug_Printf("Task Sequence reading event EVENT_BTN_MODE with data %d\r\n", received_event.data.btn_timing);
                if (received_event.data.btn_timing == 0) {
                    wait_time = 1000;
                } else {
                    wait_time = 500;
                }
            default:
                break;
        }
    }

    if (!mode_sequence) {
        return;
    }

    switch(state)
    {
        case ST_RED:
            Util_ControlOnboardLED(1, 0, 0);
            Debug_Println("Stage RED");
            Scheduler_Delay(wait_time);
            state = ST_GREEN;
            break;
        case ST_GREEN:
            Util_ControlOnboardLED(0, 1, 0);
            Debug_Println("Stage GREEN");
            Scheduler_Delay(wait_time);
            state = ST_BLUE;
            break;
        case ST_BLUE:
            Util_ControlOnboardLED(0, 0, 1);
            Debug_Println("Stage BLUE");
            Scheduler_Delay(wait_time);
            
            state = ST_RED;
            break;
        default:
            state = ST_RED;
            break;
    }
}

void Task_Flash(void) {
    static bool mode_flash = false;
    static uint32_t wait_time = 500;

    static enum {ST_ON, ST_OFF} state;

    

    event_t received_event;
    while (Task_ReadEvent(TASK_FLASH, &received_event)) {
        switch (received_event.id) {
            case EVENT_BTN_MODE:
                Debug_Printf("Task Flash reading event EVENT_BTN_MODE with data %d\r\n", received_event.data.btn_mode);
                mode_flash = received_event.data.btn_mode == 1;
                break;
            case EVENT_BTN_TIMING:
                Debug_Printf("Task Flash reading event EVENT_BTN_TIMING with data %d\r\n", received_event.data.btn_timing);
                if (received_event.data.btn_timing == 0) {
                    wait_time = 500;
                } else {
                    wait_time = 200;
                }
        }
    }

    if (!mode_flash) {
        return;
    }

    switch (state) {
        case ST_ON:
            Util_ControlOnboardLED(1, 0, 1);
            Scheduler_Delay(wait_time);
            Debug_Println("Flash ON");
            state = ST_OFF;
            break;
        case ST_OFF:
            Util_ControlOnboardLED(0, 0, 0);
            Scheduler_Delay(wait_time);
            Debug_Println("Flash OFF");
            state = ST_ON;
            break;
        default:
            state = ST_ON;
            break;
    }
}

void Task_Execute(task_id_t task_id) {
    switch (task_id) {
        case TASK_SEQUENCE:
            Task_Sequence();
            break;
        case TASK_FLASH:
            Task_Flash();
            break;
        default:
            break;
    }
}

bool Task_SendEvent(task_id_t task_id, event_t *event) {
    __disable_irq();

    bool result = EventBuffer_Push(&event_buffers[task_id], event);

    __enable_irq();
    return result;
}

bool Task_ReadEvent(task_id_t task_id, event_t *output_event) {
    __disable_irq();

    bool result = EventBuffer_Pop(&event_buffers[task_id], output_event);

    __enable_irq();
    return result;
}