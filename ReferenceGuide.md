# KL25Z Reference Guide
This is meant to be a quick, practical reference guide for CMSIS development on the FRDM-KL25Z board. It will not delve too deep into the underlying functionality and theory behind the concepts, but will give enough explanation to be understandable and applicable in a real program. This guide assumes a general understanding of each concept, but not exactly how to apply and use it on this board.
## Pin Initialization
### Clock Gating
For a peripheral to work, you must first enable the clock signal to that peripheral module.
This is done through the SIM->SCGC (System Clock Gating Control) registers.

```
SIM_SCGC4
UART0, UART1, I2C0, I2C1

SIM_SCGC5
PORTA - PORTE

SIM_SCGC6
RTC, PIT, TPM0, TPM1, ADC0
```

Masks exist in CMSIS that allow enabling & disabling the clock for certain peripherals in a readable way.
```
SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
etc.
```

### Pin configuration
The PORT modules allow a user to control general configuration for pins on each port.

The PCR (Pin Control Register) is the most commonly used within the PORT module. It controls the following settings:
  - Multiplexing (MUX)
  - Drive strength (DSE)
  - Noise filtering (PFE)
  - Slew rate (SRE)
  - Pull resistor enablement (PE)
  - Pull resistor direction (PS, 0 = down 1 = up)

Through the SDK, PCRs are represented as an array within the PORT data structure, where each element corresponds to a pin's PCR. 

Each setting listed above can be modified using a corresponding mask in the format `PORT_PCR_SETTING_MASK`. For example, the drive strength mask is `PORT_PCR_DSE_MASK`.

The exception is multiplexing, where it is defined as a function that takes a number as a parameter corresponding to a certain multiplexer setting, which determines which module the pin should connect to:
`PORT_PCR_MUX(x)`

Settings can be 0-7 where 0 is none, 1 is GPIO, and the rest are pin-specific and require looking at the data sheet.

This is an example of setting up PCR:\
`PORTA->PCR[4] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK`\
This sets the pin as GPIO, enables the pull resistor, and sets the pull resistor direction to up.

### Peripheral specific intialization
After giving the pin a multiplexer setting through its PCR, it will be connected to a certain peripheral module (or none if the setting is 0, in which case you're done).

Initializing the pin for the peripheral it's connected to is different for each peripheral, refer to a specific peripheral's section to see initialization details.


## GPIO
### Initialization
The only thing that needs to be initalized for a GPIO specific pin is the PDDR (Port Data Direction Register), whose bits correspond to the data direction of the pin (1 = output, 0 = input). This is done through the port-specific GPIO modules, labeled `PTA` - `PTE`.

Example: 
```c
PTA->PDDR |= MASK(OUTPUT_SHIFT);
PTC->PDDR &= ~MASK(INPUT_SHIFT);
```

### Reading
Reading is done using the `PDIR` (Port Data Input Register), accessed under each port-specific GPIO module. Each bit of the register is the input data from the corresponding pin.

Example to read data from pin 7 on port B:
```c
// If > 0 it's true, otherwise false
bool hasInput = PTB->PDIR & MASK(7);
```

### Writing
Writing data to a pin is done through 3 registers: `PSOR` (Pin Set Output Register), `PCOR` (Pin Clear Output Register), and `PTOR` (Pin Toggle Output Register). They behave as they sound, where each pin's corresponding bit will perform their described action for that pin.

Example:
```c
// On port A, output 1 to pin 5, 0 to pin 6, and toggle pin 7
PTA->PSOR |= MASK(5);
PTA->PCOR |= MASK(6);
PTA->PTOR |= MASK(7);
```

## Interrupts
### Initialization
There are 3 main steps to initialize interrupts for a specific use case.

**Peripheral**\
First, you must configure a peripheral to generate interrupt requests. This varies between peripherals, and generally includes deciding under what exact conditions the interrupt will trigger.

Here is how you configure common peripherals to generate interrupts:

*GPIO*
```c
// Tells PORTD to generate an interrupt from pin 5, based on condition code 9
/*
Condition codes for PCR:
8: When logic 0
9: On rising edge
10: on falling edge
11: on either edge
12: when logic one
*/
PORTD->PCR[5] |= PORT_PCR_IRQC(9);
```

*PIT*
```c
// TODO: Add other peripherals here as I learn how to generate interrupts
```

**NVIC**\
Next, the NVIC (Nested Vectored Interrupt Controller) must be configured to enable the corresponding interrupt source. The priority of the source can also be adjusted

```c
// Interrupt sources correspond to the peripheral that you configured in the first step
// You can see the definitions of all interrupt sources in the IRQn enum in the MKL25Z4.h header file
NVIC_EnableIRQ(PORTD_IRQn);

// Priority can be set from 0-3
NVIC_SetPriority(PORTD_IRQn, 1);

// Optional, ensure interrupts are enabled & there are none pending
NVIC_ClearPendingIRQ(PORTD_IRQn);
__enable_irq();
```

**Processor**\
This step is optional in most cases, but the processor itself also has the ability to enable or disable interrupts. On reset interrupts are enabled, but in a more complex program it is a good idea to make sure they're enabled when configuring them for a specific case.

```c
// Enables interrupts across the whole processor, in case they were disabled previously (for a critical section)
__enable_irq();
```
&nbsp;

### Operation
Once initialized, an interrupt must have a function that runs when it's generated, called an ISR (Interrupt Service Routine). The ISR must be named in a specific way so the processor knows to run it: `PERIPHERAL_IRQHandler`. The `PERIPHERAL` section is the same as the first half of the interrupt source name you gave when enabling it (e.g. `PORTD`).

The ISR takes no parameters, and will trigger when the condition for the peripheral is met for any part of the peripheral. So, within the ISR, you should check which exact pin/channel/part of the peripheral generated the interrupt so that the code can react to exactly what happened. How to do this varies between peripherals.

Once the ISR is done with it's logic, you must clear the ISF (Interrupt Status Flags) for the peripheral that generated the interrupt. This tells the processor that the interrupt has been handled and it should not run the ISR again. The exact method changes depending on the peripheral.

Here is an example ISR for GPIO:

```c
void PORTD_IRQHandler(void)
{
    if (PORTD->ISFR & MASK(5))
    {
        // PTD5 generated the interrupt
    }

    if (PORTD->ISFR & MASK(2))
    {
        // PTD2 generated the interrupt
    }

    // Clear the Interrupt Status Flag Register
    PORTD->ISFR = 0xffffffff;
}
```

