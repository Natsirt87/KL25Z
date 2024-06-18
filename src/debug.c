/*
    Utility module for debug operations, such as writing out to the console (represented by a serial connection to the host computer).
    Include any other debug-related functions in the future.
*/

#include "../platform/MKL25Z4/include/MKL25Z4.h"
#include "util.h"

#include <stdarg.h>
#include <stdint.h>

#define UART_OVERSAMPLE_RATE 16
#define SYS_CLOCK 48e6

void Debug_Init(uint32_t baud_rate) {
    // Enable clock for UART0
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    
    // Use FLL output for UART Baud rate generator
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
    
    // Disable UART0 while changing configurations
    UART0->C2 = 0;
    
    // Calculate & set baud rate
    uint16_t sbr = (uint16_t) (SystemCoreClock / (baud_rate * UART_OVERSAMPLE_RATE));
    UART0->BDH &= ~UART0_BDH_SBR_MASK;
    UART0->BDH |= UART0_BDH_SBR(sbr >> 8);
    UART0->BDL = UART0_BDL_SBR(sbr);
    
    // Set over-sampling ratio
    UART0->C4 = UART0_C4_OSR(UART_OVERSAMPLE_RATE - 1);
    
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

void Debug_Println(const char *str) {
    // Determine the length of the input string
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    
    // Create a buffer on the stack with enough space for the new string
    // Input length + 2 for "\r\n" + 1 for null terminator
    char buffer[length + 3];

    // Copy the input string to the buffer
    for (int i = 0; i < length; i++) {
        buffer[i] = str[i];
    }

    // Add "\r\n" to the end of the buffer
    buffer[length] = '\r';
    buffer[length + 1] = '\n';

    // Null-terminate the new string
    buffer[length + 2] = '\0';

    Debug_Print(buffer);
}

void Debug_Printf(const char* format, ...) {
    char buffer[256]; // Assuming a maximum buffer size of 256
    char* buf_ptr = buffer;
    const char* str_ptr = format;
    va_list args;
    va_start(args, format);

    while (*str_ptr != '\0') {
        if (*str_ptr == '%') {
            str_ptr++;
            switch (*str_ptr) {
                case 'd': {
                    int value = va_arg(args, int);
                    char temp[32];
                    itoa(value, temp, 10);
                    char* temp_ptr = temp;
                    while (*temp_ptr) {
                        *buf_ptr++ = *temp_ptr++;
                    }
                    break;
                }
                case 's': {
                    const char* value = va_arg(args, const char*);
                    while (*value) {
                        *buf_ptr++ = *value++;
                    }
                    break;
                }
                case 'c': {
                    char value = (char) va_arg(args, int);
                    *buf_ptr++ = value;
                    break;
                }
                default:
                    *buf_ptr++ = '%';
                    *buf_ptr++ = *str_ptr;
                    break;
            }
        } else {
            *buf_ptr++ = *str_ptr;
        }
        str_ptr++;
    }
    *buf_ptr = '\0';
    va_end(args);

    // Call Debug_Print with the formatted string
    Debug_Print(buffer);
}