/*
  This is an example of basic GPIO configuration and operation. It reads the status of
  a button on port D pin 1, and based on that value lights an LED on port B pin 18.
*/

#include "MKL25Z4.h"

#define RED_LED_PIN 18
#define BUTTON_PIN 1

#define RED_LED_PORT PORTB
#define BUTTON_PORT PORTD

#define RED_LED_GPIO FPTB
#define BUTTON_GPIO FPTD

#define RED_LED_MASK (1 << RED_LED_PIN)
#define BUTTON_MASK (1 << BUTTON_PIN)

void initGPIO(void) {
    // Enable clocks to ports
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    // Configure LED pin as GPIO
    RED_LED_PORT->PCR[RED_LED_PIN] = PORT_PCR_MUX(1);
    RED_LED_GPIO->PDDR |= RED_LED_MASK; // Set as output

    // Configure button pin as GPIO with pull-up resistor
    BUTTON_PORT->PCR[BUTTON_PIN] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    BUTTON_GPIO->PDDR &= ~BUTTON_MASK; // Set as input
} 

int main(void) {
    initGPIO();

    while (1) {
        // Check if button is pressed (active low)
        if (BUTTON_GPIO->PDIR & BUTTON_MASK) {
            RED_LED_GPIO->PCOR = RED_LED_MASK; // Turn off LED
        } else {
            RED_LED_GPIO->PSOR = RED_LED_MASK; // Turn on LED
        }
    }

    return 0;
}
