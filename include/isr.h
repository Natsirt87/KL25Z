/*
    Definitions for all interrupt service routines wihtin the program, excluding special cases like SysTick.
    These ISRs should be modularized and grouped into separate files if this module grows too big.
*/

#ifndef ISR_H
#define ISR_H

#include "board_config.h"

void PORTA_IQRHandler(void);

#endif