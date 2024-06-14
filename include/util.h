#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void Util_Init_Onboard_LED(void);
void Util_Control_Onboard_LED(uint8_t r, uint8_t g, uint8_t b);

#endif