#ifndef TASK_H
#define TASK_H

#include "event_buffer.h"

typedef enum {
    TASK_FLASH,
    TASK_SEQUENCE,
    TASK_DIM,
    // Put new task ids here
    NUM_TASKS // Represents the number of unique tasks
} task_id_t;

void Task_Sequence(void);
void Task_Flash(void);

void Task_Execute(task_id_t task_id);
bool Task_SendEvent(task_id_t task_id, event_t *event);
bool Task_MulticastEvent(event_t *event);
bool Task_ReadEvent(task_id_t task_id, event_t *output_event);

#endif