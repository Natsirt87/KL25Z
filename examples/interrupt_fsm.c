/*
  This is an example of a slightly more sophisticated program, which uses
  finite state machines and interrupts to efficiently handle user input
  and execute various tasks. It reads the status of 2 buttons, and blinks
  the onboard RGB LED in different ways based on their values.

  However, this is still a naive approach and the responsivenss is still poor,
  since busy waiting is used in state transitions. This can be solved with timers
  that allow asynchronous, non-busy waiting. This will be explored in a different
  example.

  If button 1 is pressed, the LED will flash white.
  If button 1 is not pressed, the LED will sequence between RGB values.

  If button 2 is pressed, the delay between changing RGB values will be fast.
  If button 2 is not pressed, the delay between changing RGB values will be slow.
*/

#include "../platform/MKL25Z4/include/MKL25Z4.h"

#define RED_LED_POS 18
#define GREEN_LED_POS 19
#define BLUE_LED_POS 1

#define BTN1_POS 1
#define BTN2_POS 2

#define MASK(x) (1UL << x)

#define FLASH_DELAY_SLOW 400
#define FLASH_DELAY_FAST 200
#define RGB_DELAY_SLOW 4000
#define RGB_DELAY_FAST 1000

uint8_t g_flash_LED = 0;
uint32_t g_flash_delay = FLASH_DELAY_SLOW;
uint32_t g_rgb_delay = RGB_DELAY_SLOW;

void Init(void);
void Task_Flash(void);
void Task_Sequence(void);
void Delay(uint32_t time_del);
uint8_t Button_Pressed(uint8_t btn_pos);
void Control_RGB_LED(uint8_t r, uint8_t g, uint8_t b);

void Init(void)
{
    // Enable clock for ports B & D
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;

    // Connect LED pins to GPIO
    PORTB->PCR[RED_LED_POS] |= PORT_PCR_MUX(1);
    PORTB->PCR[GREEN_LED_POS] |= PORT_PCR_MUX(1);
    PORTD->PCR[BLUE_LED_POS] |= PORT_PCR_MUX(1);

    // Connect button pins to GPIO, enable pull-up resistors & GPIO interrupt on rising edge
    PORTD->PCR[BTN1_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | 
        PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xB);
    PORTD->PCR[BTN2_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | 
        PORT_PCR_PS_MASK | PORT_PCR_IRQC(0xB);
    
    // Enable GPIO hardware interrupts for port D
    NVIC_EnableIRQ(PORTD_IRQn);

    // Set LED pins as output
    PTB->PDDR |= MASK(RED_LED_POS) | MASK(GREEN_LED_POS);
    PTD->PDDR |= MASK(BLUE_LED_POS);

    // Set button pins as input
    PTD->PDDR &= ~(MASK(BTN1_POS) | MASK(BTN2_POS));
}

int main(void)
{
    Init();

    while (1)
    {
        Task_Flash();
        Task_Sequence();
    }
}

void PORTD_IRQHandler(void)
{
    if (PORTD->ISFR & MASK(BTN1_POS))
    {
        // Button 1 was pressed or released
        if (Button_Pressed(BTN1_POS))
            g_flash_LED = 1;
        else
            g_flash_LED = 0;
    }

    if (PORTD->ISFR & MASK(BTN2_POS))
    {
        // Button 2 was pressed or released
        if (Button_Pressed(BTN2_POS))
        {
            g_flash_delay = FLASH_DELAY_FAST;
            g_rgb_delay = RGB_DELAY_FAST;
        }
        else
        {
            g_flash_delay = FLASH_DELAY_SLOW;
            g_rgb_delay = RGB_DELAY_SLOW;
        }
    }

    // Clear the Interrupt Status Flag Register
    PORTD->ISFR = 0xffffffff;
}

void Task_Flash(void)
{
    static enum {ST_ON, ST_OFF} state;
    if (g_flash_LED)
    {
        switch (state) 
        {
            case ST_ON:
                Control_RGB_LED(1, 1, 1);
                Delay(g_flash_delay);
                state = ST_OFF;
                break;
            case ST_OFF:
                Control_RGB_LED(1, 1, 1);
                Delay(g_flash_delay);
                state = ST_ON;
                break;
            default:
                state = ST_ON;
                break;
        }
    }
}

void Task_Sequence(void)
{
    // Finite state machine, performs one state at a time then returns
    static enum {ST_RED, ST_GREEN, ST_BLUE} state;
    static uint8_t waiting = 0;

    if (!g_flash_LED) 
    {
        switch(state)
        {
            case ST_RED:
                Control_RGB_LED(1, 0, 0);
                Delay(g_rgb_delay);
                state = ST_GREEN;
                break;
            case ST_GREEN:
                Control_RGB_LED(0, 1, 0);
                Delay(g_rgb_delay);
                state = ST_BLUE;
                break;
            case ST_BLUE:
                Control_RGB_LED(0, 0, 1);
                Delay(g_rgb_delay);
                state = ST_RED;
                break;
            default:
                state = ST_RED;
                break;
        }
            
    }
}

void Delay(uint32_t timeDelay)
{
    volatile int n;
    while (timeDelay--)
    {
        n = 1000;
        while (n--)
            ;
    }
}

uint8_t Button_Pressed(uint8_t btn_pos)
{
    return ((PTD->PDIR & MASK(btn_pos)) >> btn_pos) == 0;
}

void Control_RGB_LED(uint8_t r, uint8_t g, uint8_t b)
{
    // Onboard LED is in active-low configuration, so clearing a bit turns it on
    if (r)
        PTB->PCOR |= MASK(RED_LED_POS);
    else
        PTB->PSOR |= MASK(RED_LED_POS);

    if (g)
        PTB->PCOR |= MASK(GREEN_LED_POS);
    else
        PTB->PSOR |= MASK(GREEN_LED_POS);

    if (b)
        PTD->PCOR |= MASK(BLUE_LED_POS);
    else
        PTD->PSOR |= MASK(BLUE_LED_POS);
}