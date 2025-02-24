#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

#define systick_base 0xE000E010U

typedef struct systick_registers {
    uint32_t STCSR;
    uint32_t STRVR;
    uint32_t STCVR;
    uint32_t STCR;
} systick_registers;

void systick_handler(void);

#endif