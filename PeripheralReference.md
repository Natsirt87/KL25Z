# KL25Z Peripheral Reference
Provides documented examples for initialization & common use cases for all peripherals of the KL25Z.

## 1. GPIO (General Purpose Input/Output)
### Initialization
```c
void configure_gpio(void) {
    // Enable clock for PORTA and PORTB
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
    
    // Configure pin as GPIO input
    PORTA->PCR[3] = PORT_PCR_MUX(1);
    PORTB->PCR[18] = PORT_PCR_MUX(1);
    
    // Set pin direction
    PTA->PDDR &= ~(1 << 3); // Input
    PTB->PDDR |= (1 << 18); // Output
}
```

### Use

```c
void set_pin_high(void) {
    PTB->PSOR = (1 << 18); // Set PTB18 high
}

void set_pin_low(void) {
    PTB->PCOR = (1 << 18); // Set PTB18 low
}

void toggle_pin(void) {
    PTB->PTOR = (1 << 18); // Toggle PTB18
}

int read_pin(void) {
    return (PTA->PDIR & (1 << 3)); // Read PTA3
}
```
&nbsp;

## 2. PIT (Periodic Interrupt Timer)
### Initialization
```c
void configure_pit(void) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; // Enable clock for PIT
    PIT->MCR = 0x00; // Enable PIT module
    
    PIT->CHANNEL[0].LDVAL = 0xFFFFF; // Load value for timer
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; // Enable timer and interrupt
    
    NVIC_EnableIRQ(PIT_IRQn); // Enable PIT interrupt in NVIC
}
```
### Use
```c
void PIT_IRQHandler(void) {
    if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
        PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK; // Clear interrupt flag
        // Timer action
    }
}
```
&nbsp;

## 3. ADC (Analog-to-Digital Converter)
### Initialization
```c

void configure_adc(void) {
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; // Enable clock for ADC0
    ADC0->CFG1 = ADC_CFG1_MODE(2); // 12-bit conversion
    ADC0->SC1[0] = ADC_SC1_ADCH(0); // Select channel 0
}
```
### Use
```c
uint16_t read_adc(void) {
    ADC0->SC1[0] = ADC_SC1_ADCH(0); // Start conversion on channel 0
    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK)); // Wait for conversion to complete
    return ADC0->R[0]; // Read result
}
```
&nbsp;

## 4. UART (Universal Asynchronous Receiver/Transmitter)
### Initialization
```c
void configure_uart(void) {
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK; // Enable clock for UART0
    SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1); // Select FLL/PLL as UART clock source
    
    UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK); // Disable transmitter and receiver
    UART0->BDH = 0x00;
    UART0->BDL = 0x1A; // Set baud rate to 115200
    UART0->C4 = 0x0F;
    
    UART0->C1 = 0x00; // 8-bit data, no parity
    UART0->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK; // Enable transmitter and receiver
}
```

### Use
```c
void uart_send(char data) {
    while (!(UART0->S1 & UART_S1_TDRE_MASK));
    UART0->D = data;
}

char uart_receive(void) {
    while (!(UART0->S1 & UART_S1_RDRF_MASK));
    return UART0->D;
}

void uart_send_string(const char *str) {
    while (*str) {
        uart_send(*str++);
    }
}
```
&nbsp;

## 5. I2C (Inter-Integrated Circuit)
### Initialization
```c
void configure_i2c(void) {
    SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK; // Enable clock for I2C0
    
    I2C0->F = 0x14; // Set baud rate
    I2C0->C1 = I2C_C1_IICEN_MASK; // Enable I2C module
}
```
### Use
```c
void i2c_start(void) {
    I2C0->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK; // Start condition
}

void i2c_stop(void) {
    I2C0->C1 &= ~I2C_C1_MST_MASK; // Stop condition
}

void i2c_write_byte(uint8_t data) {
    I2C0->D = data;
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK; // Clear interrupt flag
}

uint8_t i2c_read_byte(void) {
    I2C0->C1 &= ~I2C_C1_TX_MASK; // Set to receive mode
    I2C0->D; // Dummy read
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK; // Clear interrupt flag
    return I2C0->D;
}
```
&nbsp;

## 6. SPI (Serial Peripheral Interface)
### Initialization
```c
void configure_spi(void) {
    SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK; // Enable clock for SPI0
    
    SPI0->C1 = SPI_C1_MSTR_MASK | SPI_C1_SPE_MASK; // Enable SPI as master
    SPI0->C2 = 0x00;
    SPI0->BR = SPI_BR_SPPR(2) | SPI_BR_SPR(2); // Set baud rate
}
```

### Use
```c
void spi_send(uint8_t data) {
    while (!(SPI0->S & SPI_S_SPTEF_MASK));
    SPI0->D = data;
    while (!(SPI0->S & SPI_S_SPRF_MASK));
    uint8_t dummy = SPI0->D; // Read data to clear flag
}

uint8_t spi_receive(void) {
    SPI0->D = 0xFF; // Send dummy byte
    while (!(SPI0->S & SPI_S_SPRF_MASK));
    return SPI0->D;
}
```
&nbsp;

## 7. DMA (Direct Memory Access)
### Initialization
```c
void configure_dma(void) {
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK; // Enable clock for DMA
    
    DMA0->DMA[0].SAR = (uint32_t)source_address; // Set source address
    DMA0->DMA[0].DAR = (uint32_t)destination_address; // Set destination address
    DMA0->DMA[0].DSR_BCR = DMA_DSR_BCR_BCR(transfer_size); // Set transfer size
    DMA0->DMA[0].DCR = DMA_DCR_EINT_MASK | DMA_DCR_ERQ_MASK | DMA_DCR_CS_MASK; // Enable interrupt and request
    
    NVIC_EnableIRQ(DMA0_IRQn); // Enable DMA interrupt in NVIC
}
```

### Use
```c
void start_dma_transfer(void) {
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK; // Clear done flag
    DMA0->DMA[0].DCR |= DMA_DCR_START_MASK; // Start transfer
}

void DMA0_IRQHandler(void) {
    if (DMA0->DMA[0].DSR_BCR & DMA_DSR_BCR_DONE_MASK) {
        DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE_MASK; // Clear interrupt flag
        // DMA transfer complete action
    }
}
```
&nbsp;

## 8. TPM (Timer/PWM Module)
### Initialization
```c
void configure_tpm(void) {
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK; // Enable clock for TPM0
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Select clock source
    
    TPM0->SC = 0; // Disable timer to configure
    TPM0->MOD = 0xFFFF; // Set modulo value for PWM period
    TPM0->SC = TPM_SC_PS(0) | TPM_SC_CMOD(1); // Set prescaler and start timer
    
    TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK; // Set edge-aligned PWM
    TPM0->CONTROLS[0].CnV = 0x8000; // Set initial duty cycle
}
```

### Use
```c
void set_pwm_duty_cycle(uint16_t duty_cycle) {
    TPM0->CONTROLS[0].CnV = duty_cycle;
}
```