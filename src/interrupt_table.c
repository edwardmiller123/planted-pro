#include <stdint.h>

#include "utils.h"
#include "interrupts.h"
#include "time.h"
#include "gpio.h"
#include "adc.h"
#include "usart.h"

extern uint32_t _stack;

__attribute__((section(".interrupts")))
int_table interrupt_table = {
    .initial_stack = &_stack,
    .reset = &reset_handler,
    .nmi = &nmi_handler,
    .hard_fault = &hard_fault_handler,
    .mem_manage_fault = &mem_manage_fault_handler,
    .bus_fault = &bus_fault_handler,
    .usage_fault = &usage_fault_handler,
    .sv_call = NULL,
    .debug_monitor = &debug_monitor_handler,
    .pend_sv = NULL,
    .systick = &systick_handler,
    // ... //
    .adc_irq_handler = NULL,
    .usart1_irq_handler = &usart1_irq_handler,
};