#include "scheduler.h"
#include "systick.h"

static scheduled_task_t tasks[MAX_TASKS];
static uint8_t task_count = 0;
static uint8_t current_task = 0;

void Scheduler_Init(void) {
    task_count = 0;
    current_task = 0;
}

void Scheduler_AddTask(task_id_t task_id) {
    if (task_count < MAX_TASKS) {
        tasks[task_count].task_id = task_id;
        tasks[task_count].state = TASK_READY;
        tasks[task_count].last_run = 0;
        tasks[task_count].delay = 0;
        task_count++;
    }
}

void Scheduler_Run(void) {
    uint32_t current_time;

    while (1) {
        current_time = Systick_GetTicks();

        for (current_task = 0; current_task < task_count; current_task++) {

            // If task is waiting, check for how long & move it to ready if delay has elapsed
            if (tasks[current_task].state == TASK_WAITING) {
                if (current_time - tasks[current_task].last_run >= tasks[current_task].delay) {
                    tasks[current_task].state = TASK_READY;
                }
            } 

            // Execute ready task & update last run time
            if (tasks[current_task].state == TASK_READY) {
                Task_Execute(tasks[current_task].task_id);
                tasks[current_task].last_run = current_time;
            }
        }
    }
}

void Scheduler_Delay(uint32_t ms) {
    tasks[current_task].delay = ms;
    tasks[current_task].state = TASK_WAITING;
}