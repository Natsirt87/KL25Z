#include "task_defs.h"
#include "task.h"
#include "debug.h"
#include "util.h"
#include "scheduler.h"
#include "pwm.h"

static enum {ST_INIT, ST_RUNNING} state;
static bool mode_dim = true;
static pwm_config_t pwm_config;

static void HandleEvent(event_t*);

static void Init(void) {
    Debug_Println("Initializing PWM for dim");

    pwm_config.tpm = TPM2;
    pwm_config.tpm_ch = 0;
    pwm_config.period = 20;
    pwm_config.duty_cycle_max = 20;
    pwm_config.duty_cycle_min = 0;

    PWM_Output_Init(&pwm_config, PORTB, 18, 3);

    Debug_Println("Finished PWM init");
}

void Task_Dim(void) {
    event_t received_event;

    while (Task_ReadEvent(TASK_FLASH, &received_event)) {
        HandleEvent(&received_event);
    }

    if (!mode_dim) {
        PWM_Write(&pwm_config, 1.0);
        return;
    }

    switch (state) {
        case ST_INIT:
            Init();
            state = ST_RUNNING;
            Scheduler_Delay(1000);
            break;
        case ST_RUNNING:
            PWM_Write(&pwm_config, 0.95);
            break;
        default:
            break;

    }
}

static void HandleEvent(event_t *received_event) {
    switch (received_event->id) {
        
        case EVENT_BTN_MODE:
            Debug_Printf("Task Dim reading event EVENT_BTN_MODE with data %d\r\n", received_event->data.btn_mode);

            mode_dim = received_event->data.btn_mode == 1;
            break;

        default:
            break;
    }
}