#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "standard.h"

#define LED_POS 18
#define LED_PORT PORTB
#define LED_GPIO PTB

#define MASK(x) (1UL << x)

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