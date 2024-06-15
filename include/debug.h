#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

void Debug_Init(uint32_t baud_rate);
void Debug_Print(const char *str);

#endif