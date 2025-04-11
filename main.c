#include <stdio.h>
#include <stdint.h>

#include "utils.h"
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

    configure_usart1(115200);

    char * test = "testing";

    if (usart_send_buffer((uint8_t *)test, strlen(test)) == -1)
    {
        fast_blink();
    };

    while (1)
    {
        toggle_user_led();
        sys_sleep(500);
    };

    return 0;
}