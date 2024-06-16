#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 5

typedef enum {
    TASK_FLASH,
    TASK_SEQUENCE
} task_id_t;

typedef enum {
    TASK_READY,
    TASK_WAITING
} task_state_t;

typedef struct {
    task_id_t task_id;
    task_state_t state;
    uint32_t last_run;
    uint32_t delay;
} task_t;

void Scheduler_Init(void);
void Scheduler_AddTask(task_id_t task_id);
void Scheduler_Run(void);
void Scheduler_Delay(uint32_t ms);

#endif