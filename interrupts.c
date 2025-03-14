#include <stdio.h>

#include "interrupts.h"
#include "io.h"
#include "systick.h"

void enable_interrupt(uint32_t n) {
    uint32_t port;
    if (n <= 31) {
        port = NVIC_ISER1_R;
    }
    io_toggle_bit(port, n);
}

void disable_interrupt(uint32_t n) {
    uint32_t port;
    if (n <= 31) {
        port = NVIC_ICER1_R;
    }
    io_toggle_bit(port, n);
}

// for now just hang
void nmi_handler(void) {
    while (1) {;}
}

void hard_fault_handler(void) {
    while (1) {;}
}

void usage_fault_handler(void) {
    while (1) {;}
}

void mem_manage_fault_handler(void) {
    while (1) {;}
}

void bus_fault_handler(void) {
    while(1){;}
}

void debug_monitor_handler(void) {
    while(1){;}
}