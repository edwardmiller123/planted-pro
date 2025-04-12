#include <stdio.h>
#include <stdint.h>

#include "utils.h"
#include "interrupts.h"
#include "io.h"
#include "gpio.h"
#include "systick.h"
#include "logger.h"

#include "interrupt_table.c"

int main(void)
{
    init_systick();

    init_irqs(&interrupt_table);

    init_user_led();

    configure_usart1(115200);

    uint32_t args[1] = {get_system_uptime()};
    loggerf(INFO, "Initialised: systick, LED2, USART1 in $ ms", args, 1);

    while (1)
    {
        toggle_user_led();
        sys_sleep(500);
    };

    return 0;
}