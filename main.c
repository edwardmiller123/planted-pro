#include <stdio.h>
#include <stdint.h>

#include "interrupts.h"
#include "io.h"
#include "gpio.h"
#include "systick.h"

#include "interrupt_table.c"

void main(void)
{
    init_user_led();

    init_systick();

    init_usart();

    usart_send_byte('h');

    while (1)
    {
        toggle_user_led();
        sys_sleep(500);
    };
}