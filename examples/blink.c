#include "../platform/MKL25Z4/include/MKL25Z4.h"

#define LED_POS 1
#define LED_PORT PORTD
#define LED_GPIO PTD

#define MASK(x) (1UL << x)

void Delay(uint32_t time_del);

void init(void) {
    // Enable clocks to ports
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    LED_PORT->PCR[LED_POS] |= PORT_PCR_MUX(1);

    LED_GPIO->PDDR |= MASK(LED_POS);
} 

int main(void) {
    init();

    while (1) {
        LED_GPIO->PCOR |= MASK(LED_POS);
        Delay(1000);
        LED_GPIO->PSOR |= MASK(LED_POS);
        Delay(1000);
    }

    return 0;
}

void Delay(uint32_t timeDelay) {
    volatile int n;
    while (timeDelay--)
    {
        n = 1000;
        while (n--)
            ;
    }
}