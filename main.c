#include <stdint.h>

#include "utils.h"
#include "interrupts.h"
#include "io.h"
#include "gpio.h"
#include "systick.h"
#include "logger.h"
#include "lcd.h"
#include "adc.h"
#include "monitor.h"

#include "interrupt_table.c"

int main(void)
{
    // enable gpio ports A - D in AHB1 bus
    io_set_bit(RCC_AHB1ENR, 0);
    io_set_bit(RCC_AHB1ENR, 1);
    io_set_bit(RCC_AHB1ENR, 2);
    io_set_bit(RCC_AHB1ENR, 3);

    init_systick();

    init_irqs(&interrupt_table);

    init_user_led();

    configure_usart1(115200);

    set_log_level(DEBUG);

    uint32_t args[1] = {get_system_uptime()};
    loggerf(INFO, "Systick, LED2 and USART1 initialisation completed in $ ms", args, 1, NULL, 0);

    configure_lcd();

    configure_adc1();

    queue light_readings_queue;
    light_monitor lm;
    monitor m;

    init_monitor(&m, &lm, &light_readings_queue);

    while (1)
    {
        run_monitor(&m);
    };

    return 0;
}