#ifndef TASKS_H
#define TASKS_H

#include "scheduler.h"

void Task_Sequence(void);
void Task_Flash(void);
void Task_Execute(task_id_t task_id);

#endif