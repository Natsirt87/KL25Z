#include "standard.h"

void Delay(uint32_t timeDelay) {
    volatile int n;
    while (timeDelay--)
    {
        n = 1000;
        while (n--)
            ;
    }
}