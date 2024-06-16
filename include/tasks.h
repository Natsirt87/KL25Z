#ifndef TASKS_H
#define TASKS_H

#include "scheduler.h"

// Define event buffers for each task
// Init function to initialize buffers
// Task_SendEvent(event_buffer_t*, event_t)

void Task_Sequence(void);
void Task_Flash(void);
void Task_Execute(task_id_t task_id);

#endif