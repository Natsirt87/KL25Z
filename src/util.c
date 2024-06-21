#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "util.h"
#include "board_config.h"
#include "pwm.h"
#include "debug.h"

static pwm_config_t pwm_red;
static pwm_config_t pwm_green;
static pwm_config_t pwm_blue;

static void BusyDelay(int time);

void Util_InitOnboardLED(void) {

    pwm_red.tpm = TPM2;
    pwm_red.tpm_ch = 0;
    pwm_red.period = 20;
    pwm_red.duty_cycle_min = 0;
    pwm_red.duty_cycle_max = 20;

    pwm_green.tpm = TPM2;
    pwm_green.tpm_ch = 1;
    pwm_green.period = 20;
    pwm_green.duty_cycle_min = 0;
    pwm_green.duty_cycle_max = 20;

    pwm_blue.tpm = TPM0;
    pwm_blue.tpm_ch = 1;
    pwm_blue.period = 20;
    pwm_blue.duty_cycle_min = 0;
    pwm_blue.duty_cycle_max = 20;

    PWM_Output_Init(&pwm_red, PORTB, 18, 3, 1.0);
    PWM_Output_Init(&pwm_green, PORTB, 19, 3, 1.0);
    PWM_Output_Init(&pwm_blue, PORTD, 1, 4, 1.0);
}

void Util_SetOnboardLED(double red, double green, double blue) {
    red = 1.0 - red;
    green = 1.0 - green;
    blue = 1.0 - blue;

    PWM_Write(&pwm_red, red);

    Debug_Printf("Writing %d to green", green);
    PWM_Write(&pwm_green, green);

    PWM_Write(&pwm_blue, blue);
}