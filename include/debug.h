#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

void Debug_Init(uint32_t baud_rate);
void Debug_Print(const char *str);
void Debug_Println(const char *str);
void Debug_Printf(const char* format, ...);

#endif