
#include "interrupts.h"
#include "io.h"
#include "utils.h"
#include "logger.h"

extern void main(void);
extern uint8_t _data;
extern uint8_t _data_end;
extern uint8_t _fdata;
extern uint8_t _bss;
extern uint8_t _bss_end;

void nvic_enable_irq(uint32_t n)
{
    uint32_t port;
    if (n <= 31)
    {
        port = NVIC_ISER0;
    }
    if ((32 <= n) && (n <= 63))
    {
        port = NVIC_ISER1;
        n -= 32;
    }

    io_set_bit(port, n);
}

void nvic_disable_irq(uint32_t n)
{
    uint32_t port;
    if (n <= 31)
    {
        port = NVIC_ICER0;
    }
    io_set_bit(port, n);
}

void unmask_exti(uint32_t n) {
    io_clear_bit(EXTI_IMR, n);
}

// clears the pending status of an interrupt so it can fire again
void nvic_clear_pending_irq(uint32_t n)
{
    uint32_t port;
    if (n <= 31)
    {
        port = NVIC_ICPR0;
    }

    if ((32 <= n) && (n <= 63))
    {
        port = NVIC_ICPR1;
        n -= 32;
    }
    io_set_bit(port, n);
}

void exti_clear_pending_irq(uint32_t n) {
    // writing a 1 to this register clears the bit
    io_set_bit(EXTI_PR, n);
}

void reset_handler(void)
{
    // copy .data section to sram
    uint8_t *pos = &_fdata;
    uint8_t *data_dst = &_data;
    while (data_dst != &_data_end)
    {
        *(data_dst++) = *(pos++);
    }

    // zero the .bss section
    uint8_t *bss_dst = &_bss;
    while (bss_dst != &_bss_end)
    {
        *(bss_dst++) = 0;
    }

    main();
}

// for now just hang
void nmi_handler(void)
{
    while (1)
    {
        ;
    };
}

void hard_fault_handler(void)
{
    logger(FATAL, "Hard Fault");
    while (1)
    {
        ;
    };
}

void usage_fault_handler(void)
{
    while (1)
    {
        ;
    };
}

void mem_manage_fault_handler(void)
{
    while (1)
    {
        ;
    };
}

void bus_fault_handler(void)
{
    while (1)
    {
        ;
    };
}

void debug_monitor_handler(void)
{
    while (1)
    {
        ;
    };
}

void register_irq_handler(int_table * interrupt_table, uint32_t irq, void * handler) {
    interrupt_table->irq_handlers[irq] = handler;
}