#include <stdint.h>
#include <stdio.h>

#include "systick.h"

extern uint32_t _stack;
extern void reset_handler(void);

__attribute__ ((packed))
typedef struct int_table {
    uint32_t * initial_stack;
    void * reset;
    void * nmi;
    void * hard_fault;
    void * mem_manage_fault;
    void * bus_fault;
    void * usage_fault;
    void * reserved1;
    void * sv_call;
    void * debug_monitor;
    void * reserved2;
    void * pend_sv;
    void * systick;

} int_table;

__attribute__ ((section(".interrupts")))
int_table interrupts = {
    .initial_stack = &_stack,
    .reset = &reset_handler,
    .nmi = NULL,
    .hard_fault = NULL,
    .mem_manage_fault = NULL,
    .bus_fault = NULL,
    .usage_fault = NULL,
    .reserved1 = NULL,
    .sv_call = NULL,
    .debug_monitor = NULL,
    .reserved2 = NULL,
    .pend_sv = NULL,
    .systick = systick_handler,
};