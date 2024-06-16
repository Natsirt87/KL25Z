#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void Util_InitOnboardLED(void);
void Util_ControlOnboardLED(uint8_t r, uint8_t g, uint8_t b);

#endif