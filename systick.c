#include "systick.h"
#include "interrupts.h"
#include "io.h"
#include "gpio.h"

// This is initialised to zero by the reset handler
static uint32_t system_counter;

void systick_handler(void)
{
    system_counter += 1;
}

// initialise thew system clock to tick every millisecond
void init_systick(void)
{
    // set the clock reload value (tick every millisecond)
    IO_ACCESS(STK_LOAD_R) = 18750 - 1; // processor speed/1000 - 1
    // clear the current value (just to be sure)
    IO_ACCESS(STK_VAL_R) = 0x0U;
    // set the source to processor clock
    io_set_bit(STK_CTRL_R, 2);
    // enable the counter
    io_set_bit(STK_CTRL_R, 0);
    // enable systick exceptions
    io_set_bit(STK_CTRL_R, 1);
}

void sys_sleep(uint32_t ms)
{
    uint32_t exit_count = system_counter + ms;
    while (system_counter < exit_count)
    {
        ;
    };
}

uint32_t get_system_uptime(void) {
    return system_counter;
}

// Waits for a condition function to be satisfied until the specified timeout
int wait_for_condition(bool (*cond)(void), uint32_t ms) {
    uint32_t timeout = system_counter + ms;
    while (system_counter < timeout) {
        if ((*cond)()) {
            return 0;
        }
    }
    return -1;
}
