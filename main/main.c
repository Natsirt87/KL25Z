#include "stdint.h"
#include "../platform/MKL25Z4/include/MKL25Z4.h"

#define RED_LED_POS 18

#define MASK(x) (1UL << x)

void Delay(uint32_t time_del);

void init(void) {
    // Enable clocks to ports
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);

    PTB->PDDR |= MASK(RED_LED_POS);
} 

int main(void) {
    init();

    while (1) {
        PTB->PCOR |= MASK(RED_LED_POS);
        Delay(1000);
        PTB->PSOR |= MASK(RED_LED_POS);
        Delay(1000);
    }

    return 0;
}

void Delay(uint32_t timeDelay)
{
    volatile int n;
    while (timeDelay--)
    {
        n = 1000;
        while (n--)
            ;
    }
}