#include "../platform/MKL25Z4/include/MKL25Z4.h"

#include "util.h"
#include "pin_config.h"

void Util_Init_Onboard_LED(void) {
    // Enable clock for onboard led ports
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    // PCR config for led ports
    PORTB->PCR[RED_ONBOARD_LED_POS] = PORT_PCR_MUX(1);
    PORTB->PCR[GREEN_ONBOARD_LED_POS] = PORT_PCR_MUX(1);
    PORTD->PCR[BLUE_ONBOARD_LED_POS] = PORT_PCR_MUX(1);

    // Set led GPIO as output
    PTB->PDDR |= MASK(RED_ONBOARD_LED_POS) | MASK(GREEN_ONBOARD_LED_POS);
    PTD->PDDR |= MASK(BLUE_ONBOARD_LED_POS);
}

void Util_Control_Onboard_LED(uint8_t r, uint8_t g, uint8_t b) {
    // Onboard LED is in active-low configuration, so clearing a bit turns it on
    if (r)
        PTB->PCOR |= MASK(RED_ONBOARD_LED_POS);
    else
        PTB->PSOR |= MASK(RED_ONBOARD_LED_POS);

    if (g)
        PTB->PCOR |= MASK(GREEN_ONBOARD_LED_POS);
    else
        PTB->PSOR |= MASK(GREEN_ONBOARD_LED_POS);

    if (b)
        PTD->PCOR |= MASK(BLUE_ONBOARD_LED_POS);
    else
        PTD->PSOR |= MASK(BLUE_ONBOARD_LED_POS);
}