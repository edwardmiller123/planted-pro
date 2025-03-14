#include <stdio.h>
#include "interrupts.h"
#include "systick.h"

extern uint32_t _stack;
extern void reset_handler(void);

__attribute__ ((section(".interrupts")))
int_table interrupt_table = {
    .initial_stack = &_stack,
    .reset = &reset_handler,
    .nmi = &nmi_handler,
    .hard_fault = &hard_fault_handler,
    .mem_manage_fault = &mem_manage_fault_handler,
    .bus_fault = &bus_fault_handler,
    .usage_fault = &usage_fault_handler,
    .reserved1 = NULL,
    .sv_call = NULL,
    .debug_monitor = &debug_monitor_handler,
    .reserved2 = NULL,
    .pend_sv = NULL,
    .systick = &systick_handler,
};