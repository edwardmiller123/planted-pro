#include <stdint.h>

#include "utils.h"
#include "interrupts.h"
#include "io.h"
#include "gpio.h"
#include "systick.h"
#include "logger.h"
#include "lcd.h"

#include "interrupt_table.c"

int main(void)
{
    // enable AHB1 bus
    IO_ACCESS(RCC_AHB1ENR) = 0x01U;

    // Enable APB2 bus for USART1
    io_set_bit(RCC_APB2ENR, 4);

    init_systick();

    init_irqs(&interrupt_table);

    init_user_led();

    configure_usart1(115200);

    set_log_level(INFO);

    uint32_t args[1] = {get_system_uptime()};
    loggerf(INFO, "Systick, LED2 and USART1 initialisation completed in $ ms", args, 1);

    configure_lcd();

    while (1)
    {
        toggle_user_led();
        sys_sleep(500);
    };

    return 0;
}