#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

#define STK_CTRL_R 0xE000E010U
#define STK_LOAD_R 0xE000E014U
#define STK_VAL_R 0xE000E018U
#define STK_CALIB_R 0xE000E01CU

void systick_handler(void);

void configure_systick(void);

#endif