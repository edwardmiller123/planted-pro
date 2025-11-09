#ifndef INTERUPTS_H
#define INTERUPTS_H

#include <stdint.h>

#include "memmap.h"

#define IRQ_COUNT 97

// IRQ numbers
#define IRQ_ADC 18
#define EXTI9_5 23
#define IRQ_USART1 37
#define IRQ_USART3 39

// EXTI Interrupt numbers

#define EXTI7 7

// NVIC registers
#define NVIC_BASE (CORE_PERIPH_BASE + 0xE000U)

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

// Rising edge detection
#define EXTI_RTSR (EXTI_BASE + 0x08)

// Falling edge detetcion
#define EXTI_FTSR (EXTI_BASE + 0x0C)

// Interrupt pending register
#define EXTI_PR (EXTI_BASE + 0x14U)

#define SYSCFG_BASE (APB2_BASE + 0x3800)

// External interrupt configuration
#define SYSCFG_EXTICR2 (SYSCFG_BASE + 0x0C)

void nvic_enable_irq(uint32_t n);

void nvic_disable_irq(uint32_t n);

void unmask_exti(uint32_t n);

// clears the pending status of an irq in so it can fire again
void nvic_clear_pending_irq(uint32_t n);

// clears the pending status of an IRQ in the EXTI so it can fire again
void exti_clear_pending_irq(uint32_t n);

void reset_handler(void);

void nmi_handler(void);

void hard_fault_handler(void);

void usage_fault_handler(void);

void mem_manage_fault_handler(void);

void bus_fault_handler(void);

void debug_monitor_handler(void);

void init_fault_handler(void);

#define IRQ_HANDLER_BYTES (IRQ_COUNT * sizeof(void *))

typedef struct __attribute__((packed)) int_table
{
    uint32_t *initial_stack;
    void *reset;
    void *nmi;
    void *hard_fault;
    void *mem_manage_fault;
    void *bus_fault;
    void *usage_fault;
    uint8_t reserved[16];
    void *sv_call;
    void *debug_monitor;
    uint8_t reserved2[4];
    void *pend_sv;
    void *systick;
    void * watchdog_irq_handler;
    void * pvd_irq_handler;
    void * tamp_stamp_irq_handler;
    void * rtc_wkup_irq_handler;
    void * flash_irq_handler;
    void * rcc_irq_handler;
    void * exti0_irq_handler;
    void * exti1_irq_handler;
    void * exti2_irq_handler;
    void * exti3_irq_handler;
    void * exti4_irq_handler;
    void * dma1_stream0_irq_handler;
    void * dma1_stream1_irq_handler;
    void * dma1_stream2_irq_handler;
    void * dma1_stream3_irq_handler;
    void * dma1_stream4_irq_handler;
    void * dma1_stream5_irq_handler;
    void * dma1_stream6_irq_handler;
    void * adc_irq_handler;
    void * can1_tx_irq_handler;
    void * can1_rx0_irq_handler;
    void * can1_rx1_irq_handler;
    void * can1_sce_irq_handler;
    void * exti9_5_irq_handler;
    void * tim1_brk_tim2_irq_handler;
    void * tim1_up_tim10_irq_handler;
    void * tim1_trg_com_tim10_irq_handler;
    void * tim1_cc_irq_handler;
    void * tim2_irq_handler;
    void * tim3_irq_handler;
    void * tim4_irq_handler;
    void * i2c1_ev_irq_handler;
    void * i2c1_er_irq_handler;
    void * i2c2_ev_irq_handler;
    void * i2c2_er_irq_handler;
    void * spi1_irq_handler;
    void * spi2_irq_handler;
    void * usart1_irq_handler;
    void * usart2_irq_handler;
    void * usart3_irq_handler;

} int_table;

#endif