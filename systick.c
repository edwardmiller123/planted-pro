#include "systick.h"
#include "interrupts.h"
#include "io.h"
#include "led.h"

static unsigned int system_counter = 0;

void systick_handler(void) {
    system_counter += 1;
    toggle_led((uint32_t)5);
    while(1) {;};
}

// TODO: why are we hard faulting
void configure_systick(void) {
    // set the clock reload value (tick every second)
    IO_ACCESS(STK_LOAD_R) = 0xF42400U;
    // clear the current value (just to be sure)
    IO_ACCESS(STK_VAL_R) = 0x0U;
    // set the source to processor clock
    io_toggle_bit(STK_CTRL_R, 2);
    // enable the counter
    io_toggle_bit(STK_CTRL_R, 0);
    // enable systick exceptions
    io_toggle_bit(STK_CTRL_R, 1);
    // // set clock source to processor clock, enable the counter and enable systick exceptions
    // IO_ACCESS(STK_CTRL_R) = 0x7U;

    // enable_interrupt(SYSTICK);
}

