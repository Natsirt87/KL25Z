/*
    Contains definitions for all pin & board related configuration data.
*/

#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// Onboard LED pin definitions
#define RED_ONBOARD_LED_POS 18
#define GREEN_ONBOARD_LED_POS 19
#define BLUE_ONBOARD_LED_POS 1

// Button pin definitions
#define BTN1_POS 13
#define BTN2_POS 16

#define BTN_PORT PORTA

// GPIO definitions
#define BTN1_GPIO PTA
#define BTN2_GPIO PTA

// Interrupt definitions
#define BTN_IRQn PORTA_IRQn

// Clock masks
#define SCGC5_MASK SIM_SCGC5_PORTA_MASK;

// Mask macro
#define MASK(x) (1UL << (x))

#endif