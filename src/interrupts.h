#ifndef INTERUPTS_H
#define INTERUPTS_H

#include <stdint.h>

#include "memmap.h"

#define IRQ_COUNT 96

// IRQ numbers
#define ADC 18
#define EXTI9_5 23
#define USART1 37

// EXTI Interrupt numbers

#define EXTI9 9

// NVIC registers
#define NVIC_OFFSET 0xE100U
#define NVIC_BASE (CORE_PERIPH_BASE + NVIC_OFFSET)

#define NVIC_ISER_OFFSET 0x100
#define NVIC_ISER0 (NVIC_BASE + NVIC_ISER_OFFSET + 0x0U)

#define NVIC_ISER1 (NVIC_BASE + NVIC_ISER_OFFSET + 0x4U)

// ...

#define NVIC_ICER_OFFSET 0x180U
#define NVIC_ICER0 (NVIC_BASE + NVIC_ICER_OFFSET + 0x0U)

// ...

#define NVIC_ICPR_OFFSET 0x280U
#define NVIC_ICPR0 (NVIC_BASE + NVIC_ICPR_OFFSET + 0x0U)

#define NVIC_ICPR1 (NVIC_BASE + NVIC_ICPR_OFFSET + 0x4U)

// EXTI registers
#define EXTI_BASE (APB2_BASE + 0x3C00U)

// Interrupt mask register
#define EXTI_IMR (EXTI_BASE + 0x0U)

// Interrupt pending register
#define EXTI_PR (EXTI_BASE + 0x14U)

void nvic_enable_irq(uint32_t n);

void nvic_disable_irq(uint32_t n);

void unmask_exti(uint32_t n);

// clears the pending status of an irq in so it can fire again
void nvic_clear_pending_irq(uint32_t n);

// clears the pending status of an IRQ in the EXTI so it can fire again
void exti_clear_pending_irq(uint32_t n);

void reset_handler(void);

void software_reset(void);

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
    void * irq_handlers[IRQ_COUNT];
} int_table;

void register_irq_handler(int_table * interrupt_table, uint32_t irq, void * handler);

#endif