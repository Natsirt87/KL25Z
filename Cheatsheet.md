# KL-25Z Development Guide
## Pin Initialization
### Summary/Quick Start
Steps to initialize a pin:
1. Enable the clock signal to the PORT module that contains the pin
2. Set the PCR configuration for that pin (multiplexer, pull-up register, etc.)
3. Module-specific configuration, e.g. set the data direction for GPIO

Example using GPIO:

```c

#define INPUT_SHIFT 4
#define OUTPUT_SHIFT 10

#define MASK(x) (1UL << x)

int main(void) 
{
  // Enable clock for Port A & Port C
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK;

  // Set pins to use GPIO (multiplexer setting 1)
  PORTA->PCR(INPUT_SHIFT) |= PORT_PCR_MUX(1);
  PORTC->PCR(OUTPUT_SHIFT) |= PORT_PCR_MUX(1);

  // Extra configuration, enable pull-up resistor for input pin
  // PE: Pull-resistor enable
  // PS: Pull-resistor select (1 = pull-up)
  PORTA->PCR(INPUT_SHIFT) |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

  // Set GPIO data directions for input & output
  PTA->PDDR &= ~MASK(INPUT_SHIFT);
  PTB->PDDR |= MASK(OUTPUT_SHIFT);
}

```

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

### Module specific intialization
After giving the pin a multiplexer setting through its PCR, it will be connected to a certain peripheral module (or none if the setting is 0, in which case you're done).

Initializing the pin for the module it's connected to is different for each module, refer to a specific module's section to see initialization details.


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