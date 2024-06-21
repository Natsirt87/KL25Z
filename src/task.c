#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "task.h"
#include "task_defs.h"

static volatile event_buffer_t event_buffers[NUM_TASKS];

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

bool Task_MulticastEvent(event_t *event) {
    __disable_irq();

    bool result = true;
    for (int i = 0; i < NUM_TASKS; i++) {
        bool cur_result = EventBuffer_Push(&event_buffers[i], event);
        if (!cur_result) {
            // If any tasks fail to send, overall result is failure (although we still keep going)
            result = false; 
        }
    }

    __enable_irq();
    return result;
}

bool Task_ReadEvent(task_id_t task_id, event_t *output_event) {
    __disable_irq();

    bool result = EventBuffer_Pop(&event_buffers[task_id], output_event);

    __enable_irq();
    return result;
}