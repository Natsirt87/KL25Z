# KL25Z Interrupt Reference
Examples for how to configure interrupts & set up ISRs for all peripherals on the KL25Z.

## 1. GPIO (General Purpose Input/Output) Interrupts
### Configuration
```c
void configure_gpio_interrupt(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK; // Enable clock for PORTA and PORTB
    
    // Configure pins as GPIO input with pull-up resistors
    PORTA->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 
    PORTB->PCR[18] = PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; 

    // Configure interrupts in separate operation, this is important
    PORTA->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x9); // Enable interrupt on rising edge for pin 3
    PORTB->PCR[18] = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x9); // Enable interrupt on rising edge for pin 18
    
    NVIC_EnableIRQ(PORTA_IRQn); // Enable PORTA interrupt in NVIC
    NVIC_EnableIRQ(PORTB_IRQn); // Enable PORTB interrupt in NVIC
}
```

### ISR
```c
void PORTA_IRQHandler(void) {
    if (PORTA->ISFR & (1 << 3)) {
        // Handle the interrupt for pin 3
        PORTA->ISFR = (1 << 3);  // Clear interrupt flag for pin 3
    }
}

void PORTB_IRQHandler(void) {
    if (PORTB->ISFR & (1 << 18)) {
        // Handle the interrupt for pin 18
        PORTB->ISFR = (1 << 18);  // Clear interrupt flag for pin 18
    }
}
```
&nbsp;

## 2. PIT (Periodic Interrupt Timer)
### Configuration
```c

void configure_pit_interrupt(void) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enable clock for PIT
    PIT->MCR = 0x00; // Enable PIT module
    
    PIT->CHANNEL[0].LDVAL = 0xFFFFF; // Load value for timer
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; // Enable timer and interrupt
    
    NVIC_EnableIRQ(PIT_IRQn); // Enable PIT interrupt in NVIC
}
```

### ISR
```c
void PIT_IRQHandler(void) {
    if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
        PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK; // Clear interrupt flag
        // Timer action
    }
}
```
&nbsp;

## 3. ADC (Analog-to-Digital Converter) Interrupts
### Configuration
```c

void configure_adc_interrupt(void) {
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // Enable clock for ADC0
    ADC0->CFG1 = ADC_CFG1_MODE(2); // 12-bit conversion
    ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(0); // Enable interrupt and select channel 0
    
    NVIC_EnableIRQ(ADC0_IRQn); // Enable ADC0 interrupt in NVIC
}
```

### ISR
```c
void ADC0_IRQHandler(void) {
    if (ADC0->SC1[0] & ADC_SC1_COCO_MASK) {
        // Handle the ADC conversion complete interrupt
        uint16_t result = ADC0->R[0];  // Read the result register to clear the interrupt flag
    }
}
```
&nbsp;

## 4. UART (Universal Asynchronous Receiver/Transmitter) Interrupts
### Configuration
```c
void configure_uart_interrupt(void) {
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; // Enable clock for UART0
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1); // Select FLL/PLL as UART clock source
    
    UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); // Disable transmitter and receiver
    UART0->BDH = 0x00;
    UART0->BDL = 0x1A; // Set baud rate to 115200
    UART0->C4 = 0x0F;
    
    UART0->C1 = 0x00; // 8-bit data, no parity
    UART0->C2 |= UART_C2_RIE_MASK | UART_C2_TIE_MASK; // Enable receive and transmit interrupts
    
    NVIC_EnableIRQ(UART0_IRQn); // Enable UART0 interrupt in NVIC
}
```

### ISR
```c
void UART0_IRQHandler(void) {
    if (UART0->S1 & UART_S1_RDRF_MASK) {
        // Handle UART receive interrupt
        uint8_t data = UART0->D;  // Read data to clear the flag
    }
    if (UART0->S1 & UART_S1_TDRE_MASK) {
        // Handle UART transmit interrupt
        UART0->D = next_byte_to_transmit;  // Write data to clear the flag
    }
}
```
&nbsp;

## 5. I2C (Inter-Integrated Circuit) Interrupts
### Configuration
```c

void configure_i2c_interrupt(void) {
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; // Enable clock for I2C0
    
    I2C0->F = 0x14; // Set baud rate
    I2C0->C1 = I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK; // Enable I2C module and interrupt
    
    NVIC_EnableIRQ(I2C0_IRQn); // Enable I2C0 interrupt in NVIC
}
```

### ISR
```c
void I2C0_IRQHandler(void) {
    if (I2C0->S & I2C_S_IICIF_MASK) {
        // Handle I2C interrupt (e.g., check if it's a transfer complete or an error)
        I2C0->S |= I2C_S_IICIF_MASK;  // Clear the interrupt flag
    }
}
```
&nbsp;

## 6. SPI (Serial Peripheral Interface) Interrupts
### Configuration
```c

void configure_spi_interrupt(void) {
    SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK; // Enable clock for SPI0
    
    SPI0->C1 = SPI_C1_MSTR_MASK | SPI_C1_SPE_MASK; // Enable SPI as master
    SPI0->C2 = 0x00;
    SPI0->BR = SPI_BR_SPPR(2) | SPI_BR_SPR(2); // Set baud rate
    SPI0->C1 |= SPI_C1_SPTIE_MASK | SPI_C1_SPIE_MASK; // Enable SPI transmit and receive interrupts
    
    NVIC_EnableIRQ(SPI0_IRQn); // Enable SPI0 interrupt in NVIC
}
```

### ISR
```c
void SPI0_IRQHandler(void) {
    if (SPI0->S & SPI_S_SPRF_MASK) {
        // Handle SPI receive interrupt
        uint8_t data = SPI0->D;  // Read data to clear the flag
    }
    if (SPI0->S & SPI_S_SPTEF_MASK) {
        // Handle SPI transmit interrupt
        SPI0->D = next_byte_to_transmit;  // Write data to clear the flag
    }
}
```
&nbsp;

## 7. DMA (Direct Memory Access) Interrupts
### Configuration
```c
void configure_dma_interrupt(void) {
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK; // Enable clock for DMA
    
    DMA0->DMA[0].SAR = (uint32_t)source_address; // Set source address
    DMA0->DMA[0].DAR = (uint32_t)destination_address; // Set destination address
    DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(transfer_size); // Set transfer size
    DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK; // Enable interrupt and request
    
    NVIC_EnableIRQ(DMA0_IRQn); // Enable DMA interrupt in NVIC
}
```

### ISR
```c

void DMA0_IRQHandler(void) {
    if (DMA0->DMA[0].DSR_BCR & DMA_DSR_BCR_DONE_MASK) {
        DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK; // Clear interrupt flag
        // DMA transfer complete action
    }
}
```
&nbsp;

## 8. TPM (Timer/PWM Module) Interrupts
### Configuration
```c
void configure_tpm_interrupt(void) {
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; // Enable clock for TPM0
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Select clock source
    
    TPM0->SC = 0; // Disable timer to configure
    TPM0->MOD = 0xFFFF; // Set modulo value for PWM period
    TPM0->SC = TPM_SC_PS(0) | TPM_SC_CMOD(1) | TPM_SC_TOIE_MASK; // Set prescaler, start timer, and enable interrupt
    
    TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK; // Set edge-aligned PWM
    TPM0->CONTROLS[0].CnV = 0x8000; // Set initial duty cycle
    
    NVIC_EnableIRQ(TPM0_IRQn); // Enable TPM0 interrupt in NVIC
}
```

### ISR
```c

void TPM0_IRQHandler(void) {
    if (TPM0->SC & TPM_SC_TOF_MASK) {
        TPM0->SC |= TPM_SC_TOF_MASK; // Clear overflow interrupt flag
        // Timer overflow action
    }
}
```