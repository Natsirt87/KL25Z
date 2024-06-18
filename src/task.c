#include "../platform/MKL25Z4/include/MKL25Z4.h";
#include "task.h"
#include "util.h"
#include "debug.h"

static event_buffer_t event_buffers[NUM_TASKS];

/* 
    TODO: Split each task into it's own file. 
    Make each state it's own function, and use a separate function to process each event. 
    Keep static variables in the file.
*/

void Task_Sequence(void) {
    static bool mode_sequence = true;
    static enum {ST_RED, ST_GREEN, ST_BLUE} state;
    
    event_t received_event;

    // Read each event in event buffer
    // Switch on event id, read data accordingly & perform action
    // Set mode_sequence here
    while(Task_ReadEvent(TASK_SEQUENCE, &received_event)) {
        switch (received_event.id) {
            case EVENT_BTN_MODE:
                mode_sequence = received_event.data.btn_mode == 0;
                break;
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
            Debug_Print("Stage RED\r\n");
            Scheduler_Delay(1000);
            state = ST_GREEN;
            break;
        case ST_GREEN:
            Util_ControlOnboardLED(0, 1, 0);
            Debug_Print("Stage GREEN\r\n");
            Scheduler_Delay(1000);
            state = ST_BLUE;
            break;
        case ST_BLUE:
            Util_ControlOnboardLED(0, 0, 1);
            Debug_Print("Stage BLUE\r\n");
            Scheduler_Delay(1000);
            
            state = ST_RED;
            break;
        default:
            state = ST_RED;
            break;
    }
}

void Task_SwitchMode(void) {
    static bool button_mode = 0;

    Scheduler_Delay(3000);
    button_mode = !button_mode;

    event_t example_event;
    example_event.id = EVENT_BTN_MODE;
    example_event.data.btn_mode = button_mode;

    Task_SendEvent(TASK_SEQUENCE, &example_event);
}

void Task_Flash(void) {
    bool mode_flash = false;
    enum {ST_ON, ST_OFF} state;

    event_t received_event;

    while(Task_ReadEvent(TASK_FLASH, &received_event)) {
        switch (received_event.id) {
            case EVENT_BTN_MODE:
                break;
            default:
                break;
        }
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