#include "../platform/MKL25Z4/include/MKL25Z4.h";
#include "tasks.h"
#include "util.h"
#include "debug.h"


void Task_Sequence(void) {
    // Finite state machine, performs one state at a time then returns
    static enum {ST_RED, ST_GREEN, ST_BLUE} state;

    // Read each event in event buffer
    // Switch on event id, read data accordingly & perform action

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

void Task_Flash(void) {
    // Not implemented yet
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