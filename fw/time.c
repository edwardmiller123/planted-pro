#include "time.h"
#include "interrupts.h"
#include "io.h"
#include "gpio.h"
#include "logger.h"

// We keep track of the system up time in millisseconds and the real unix time in seconds.
// The real time is the actual unix timestamp. It begins synced with the system counter until its updated
// externally

// This is initialised to zero by the reset handler
static uint32_t system_counter;

// number of ms since the last second
static uint16_t ms_counter;

// the real unix time which is synced from an external source. WIll need to change this by 2038
static uint32_t unix_time;

void systick_handler(void)
{
    system_counter += 1;
    if (ms_counter < 1000) {
        ms_counter +=1; 
    } else {
        ms_counter = 0;
        unix_time += 1;
    }
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

uint32_t get_system_counter(void)
{
    return system_counter;
}

uint32_t get_unix_time(void) {
    return unix_time;
}

void update_unix_time(uint32_t time) {
    asm __volatile__("CPSID i");
    unix_time = time;
    asm __volatile__("CPSIE i");
}
