#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "util.h"
#include "board_config.h"
#include "pwm.h"

static pwm_config_t pwm_red;
static pwm_config_t pwm_green;
static pwm_config_t pwm_blue;

void Util_InitOnboardLED(void) {

    /* pwm_red.tpm = TPM2;
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

    PWM_Output_Init(&pwm_red, PORTB, 18, 3);
    PWM_Output_Init(&pwm_green, PORTB, 19, 3);
    PWM_Output_Init(&pwm_blue, PORTD, 1, 4); */

    /* // Enable clock for onboard led ports
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    // PCR config for led ports
    PORTB->PCR[RED_ONBOARD_LED_POS] = PORT_PCR_MUX(1);
    PORTB->PCR[GREEN_ONBOARD_LED_POS] = PORT_PCR_MUX(1);
    PORTD->PCR[BLUE_ONBOARD_LED_POS] = PORT_PCR_MUX(1);

    // Set led GPIO as output
    PTB->PDDR |= MASK(RED_ONBOARD_LED_POS) | MASK(GREEN_ONBOARD_LED_POS);
    PTD->PDDR |= MASK(BLUE_ONBOARD_LED_POS); */
}

void Util_SetOnboardLED(double red, double green, double blue) {
    red = 1.0 - red;
    green = 1.0 - green;
    blue = 1.0 - blue;

    PWM_Write(&pwm_red, red);
    PWM_Write(&pwm_green, green);
    PWM_Write(&pwm_blue, blue);
}

void Util_ControlOnboardLED(uint8_t r, uint8_t g, uint8_t b) {




    /* // Onboard LED is in active-low configuration, so clearing a bit turns it on
    if (r)
        PTB->PCOR |= MASK(18);
    else
        PTB->PSOR |= MASK(18);

    if (g)
        PTB->PCOR |= MASK(GREEN_ONBOARD_LED_POS);
    else
        PTB->PSOR |= MASK(GREEN_ONBOARD_LED_POS);

    if (b)
        PTD->PCOR |= MASK(BLUE_ONBOARD_LED_POS);
    else
        PTD->PSOR |= MASK(BLUE_ONBOARD_LED_POS); */
}