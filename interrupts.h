#ifndef INTERUPTS_H
#define INTERUPTS_H

#include <stdint.h>

#include "memmap.h"

// Exception interrupt numbers
#define RESET 1
#define NMI 2
#define HARD_FAULT 3
#define MEM_MANAGE 4
#define BUS_FAULT 5
#define USAGE_FAULT 6
#define SV_CALL 11
#define DEBUG_MONITOR 12
#define PEND_SV 14
#define SYSTICK 15

// nvic register addresses

#define NVIC_OFFSET 0xE000U
#define NVIC_BASE (CORE_PERIPH_BASE + NVIC_OFFSET)

#define NVIC_ISER_OFFSET 0x100U
#define NVIC_ISER_BASE (NVIC_BASE + NVIC_ISER_OFFSET)

#define NVIC_ISER1_OFFSET 0
#define NVIC_ISER1_R (NVIC_ISER_BASE + NVIC_ISER1_OFFSET)

#define NVIC_ISER2_OFFSET 0x04
#define NVIC_ISER2_R (NVIC_ISER_BASE + NVIC_ISER2_OFFSET)

// ...

#define NVIC_ICER_OFFSET 0x180U
#define NVIC_ICER_BASE (NVIC_BASE + NVIC_ICER_OFFSET)

#define NVIC_ICER1_OFFSET 0
#define NVIC_ICER1_R (NVIC_ICER_BASE + NVIC_ICER_OFFSET)

void enable_interrupt(uint32_t n);

void disable_interrupt(uint32_t n);

void nmi_handler(void);

void hard_fault_handler(void);

void usage_fault_handler(void);

void mem_manage_fault_handler(void);

void bus_fault_handler(void);

void debug_monitor_handler(void);

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

#endif