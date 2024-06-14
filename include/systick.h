#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void Systick_Init(uint32_t ticks);
uint32_t Systick_Get_Ticks(void);

#endif