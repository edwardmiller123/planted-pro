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

#define NVIC_OFFSET 0xE100U
#define NVIC_BASE (CORE_PERIPH_BASE + NVIC_OFFSET)

#define NVIC_ISER_OFFSET 0x100
#define NVIC_ISER1_R (NVIC_BASE + NVIC_ISER_OFFSET + 0x0U)

#define NVIC_ISER2_R (NVIC_BASE + NVIC_ISER_OFFSET + 0x04U)

// ...

#define NVIC_ICER_OFFSET 0x180U
#define NVIC_ICER1_R (NVIC_BASE + NVIC_ICER_OFFSET + 0x0U)

// ...

#define NVIC_ICPR_OFFSET 0x280
#define NVIC_ICPR1_R (NVIC_BASE + NVIC_ICPR_OFFSET + 0x00)

void enable_interrupt(uint32_t n);

void disable_interrupt(uint32_t n);

// clears the pedning status of an interrupt so it can fire again
void clear_pending_interrupt(uint32_t n);

void reset_handler(void);

void nmi_handler(void);

void hard_fault_handler(void);

void usage_fault_handler(void);

void mem_manage_fault_handler(void);

void bus_fault_handler(void);

void debug_monitor_handler(void);

typedef struct __attribute__ ((packed)) int_table {
    uint32_t * initial_stack;
    void * reset;
    void * nmi;
    void * hard_fault;
    void * mem_manage_fault;
    void * bus_fault;
    void * usage_fault;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t reserved3;
    uint32_t reserved4;
    void * sv_call;
    void * debug_monitor;
    uint32_t reserved5;
    void * pend_sv;
    void * systick;
} int_table;

#endif