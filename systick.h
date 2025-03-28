#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#define STK_OFFSET 0xE010U
#define STK_BASE (CORE_PERIPH_BASE + STK_OFFSET)

#define STK_CTRL_R (STK_BASE + 0x0U)
#define STK_LOAD_R (STK_BASE + 0x04U)
#define STK_VAL_R (STK_BASE + 0x08U)
#define STK_CALIB_R (STK_BASE + 0x0CU)

void systick_handler(void);

void init_systick(void);

void sys_sleep(uint32_t ms);

#endif