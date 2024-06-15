#include "../platform/MKL25Z4/include/MKL25Z4.h";
#include "util.h"

#define UART_OVERSAMPLE_RATE 16
#define SYS_CLOCK 48e6

void Debug_Init(uint32_t baud_rate) {
    // Enable clock for UART0
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    
    // Use FLL output for UART Baud rate generator
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    
    // Disable UART0 while changing configurations
    UART0->C2 = 0;
    
    // Set baud rate registers
    // 57600 Baud assuming a fixed system clock and oversampling rate
    UART0->BDH = 0x00;
    UART0->BDL = 0x17;
    
    // Set over-sampling ratio
    UART0->C4 = UART0_C4_OSR(15);
    
    // Set UART to 8-N-1
    UART0->C1 = 0x00;
    
    // Enable transmitter
    UART0->C2 = UART_C2_TE_MASK;
    
    // Enable clock for PORTA
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    
    // Make PTA2 UART0_Tx pin
    PORTA->PCR[2] = PORT_PCR_MUX(2);
    
    // Enable UART0 interrupt in NVIC
    NVIC_EnableIRQ(UART0_IRQn);
}

void Debug_Print(const char *str) {
    // Wait until complete string is transmitted on serial port
    // and every byte is shifted out of Transmit buffer before
    // loading new byte
    while (*str)
    {
        
        __disable_irq();
        UART0->D = *str;
        // Wait until byte is transmitted from Tx Buffer
        while (!(UART0->S1 & UART_S1_TDRE_MASK)) { 
        }
        __enable_irq();
        str++;
    }
}