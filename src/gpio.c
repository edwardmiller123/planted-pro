#include <stdbool.h>

#include "gpio.h"
#include "io.h"
#include "time.h"
#include "interrupts.h"

// alters the state of the given pin in the given gpio port using the atomic BSSR register
void gpio_write_pin_atomic(gpio port, uint32_t pin, gpio_action action)
{
    if (action == CLEAR)
    {
        pin += 16;
    }
    switch (port)
    {
    case GPIOA:
        io_set_bit(GPIOA_BSSR, pin);
        break;
    case GPIOB:
        io_set_bit(GPIOB_BSSR, pin);
        break;
    case GPIOC:
        io_set_bit(GPIOC_BSSR, pin);
        break;
    case GPIOD:
        io_set_bit(GPIOD_BSSR, pin);
        break;
    }
}

void toggle_debug_led()
{
    io_toggle_bit(GPIOA_ODR, LED2);
}

void fast_blink()
{
    for (int i = 0; i < 4; i++)
    {
        toggle_debug_led();
        sys_sleep(50);
    }
}

// Initialise GPIOA for the user LED. Requires GPIOA to be enabled first
void init_debug_led()
{
    // set GPIO PA5 mode to output. Since this option is encoded as two bits we shouldnt assume
    // one hasnt been set
    io_set_bit(GPIOA_MODER, 10);
    io_clear_bit(GPIOA_MODER, 11);

    // set GPIO PA5 mode to pull up
    io_set_bit(GPIOA_PUPDR, 10);

    // set GPIO type to push pull
    IO_ACCESS(GPIOA_OTYPER) = 0x0U;

    // set GPIO output speed to medium
    IO_ACCESS(GPIOA_OSPEEDR) = 0x400U;
}