#include <stdio.h>
#include <stdint.h>

#include "interrupts.h"
#include "io.h"
#include "gpio.h"
#include "systick.h"

#include "interrupt_table.c"

int main(void)
{
    init_systick();

    init_irqs(&interrupt_table);

    init_user_led();

    init_usart1();

    while (1)
    {
        toggle_user_led();
        if (usart_send_byte('h') == -1)
        {
            fast_blink();
        };
        sys_sleep(500);
    };

    return 0;
}