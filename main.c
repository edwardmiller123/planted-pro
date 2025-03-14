#include <stdio.h>
#include <stdint.h>

#include "interrupts.h"
#include "io.h"
#include "led.h"
#include "systick.h"

#include "interrupt_table.c"

void main(void)
{

    // enable GPIOA
    IO_ACCESS(RCC_AHB1EN_R) = 0x01U;

    // set GPIO mode to output
    io_toggle_bit(GPIOAMODE_R, 10);

    // set GPIO mode to pull up
    io_toggle_bit(GPIOAPUPD_R, 10);

    // set GPIO type to push pull
    IO_ACCESS(GPIOAOTYPE_R) = 0x0U;

    // set GPIO output speed to medium
    IO_ACCESS(GPIOAOSPEED_R) = 0x400U;

    // configure_systick();

    while (1)
    {
        // blink the led by flipping the correct bit
        toggle_led((uint32_t)5);
        for (uint32_t i = 0; i < 400000; i++){;}
    };
}