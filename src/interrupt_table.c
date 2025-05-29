#include <stdint.h>

#include "utils.h"
#include "interrupts.h"
#include "systick.h"
#include "gpio.h"
#include "adc.h"

extern uint32_t _stack;

__attribute__ ((section(".interrupts")))
int_table interrupt_table = {
    .initial_stack = &_stack,
    .reset = &reset_handler,
    .nmi = &nmi_handler,
    .hard_fault = &hard_fault_handler,
    .mem_manage_fault = &mem_manage_fault_handler,
    .bus_fault = &bus_fault_handler,
    .usage_fault = &usage_fault_handler,
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
    .reserved4 = 0,
    .sv_call = NULL,
    .debug_monitor = &debug_monitor_handler,
    .reserved5 = 0,
    .pend_sv = NULL,
    .systick = &systick_handler,
};

// TODO: IRQs dont actually work
void init_irqs(int_table * interrupt_table) {
    register_irq_handler(interrupt_table, IRQ_USART1, &usart1_handler);
    register_irq_handler(interrupt_table, ADC, &adc_handler);
}