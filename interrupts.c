#include <stdio.h>

#include "interrupts.h"
#include "io.h"
#include "systick.h"

extern void main(void);
extern uint8_t _data;
extern uint8_t _data_end;
extern uint8_t _fdata;
extern uint8_t _bss;
extern uint8_t _bss_end;

void enable_interrupt(uint32_t n)
{
    uint32_t port;
    if (n <= 31)
    {
        port = NVIC_ISER1_R;
    }
    io_set_bit(port, n);
}

void disable_interrupt(uint32_t n)
{
    uint32_t port;
    if (n <= 31)
    {
        port = NVIC_ICER1_R;
    }
    io_set_bit(port, n);
}

// clears the pending status of an interrupt so it can fire again
void clear_pending_interrupt(uint32_t n)
{
    uint32_t port;
    if (n <= 31)
    {
        port = NVIC_ICPR1_R;
    }
    io_set_bit(port, n);
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