#include <stdint.h>

#include "utils.h"
#include "interrupts.h"
#include "io.h"
#include "gpio.h"
#include "usart.h"
#include "time.h"
#include "logger.h"
#include "lcd.h"
#include "adc.h"
#include "sensor.h"
#include "heap.h"
#include "plantmonitor.h"

#include "interrupt_table.c"

int main(void)
{
    bool debug_led = false;
    log_level ll = ERROR;

    #ifdef MODE
        switch (MODE)
        {
        case 1:
            ll = INFO;
            break;
        case 2:
            ll = DEBUG;
            debug_led = true;
            break;
        }
    #endif

    // enable gpio ports A - D in AHB1 bus
    io_set_bit(RCC_AHB1ENR, 0);
    io_set_bit(RCC_AHB1ENR, 1);
    io_set_bit(RCC_AHB1ENR, 2);
    io_set_bit(RCC_AHB1ENR, 3);

    init_systick();

    init_user_led();

    // used for logging
    configure_usart3(DEFAULT_BAUD);

    set_log_level(ll);

    uint32_t args[1] = {get_system_counter()};
    loggerf(INFO, "Systick, LED2 and USART3 initialisation completed in $ ms", args, 1, NULL, 0);

    configure_lcd();

    configure_adc();

    // configured for the HC-05 bluetooth module
    configure_usart1(HC05_BAUD);
    logger(INFO, "Initialised USART1");

    init_heap();

    plant_monitor *pm = init_plant_monitor(debug_led);
    if (pm == NULL)
    {
        init_fault_handler();
    }

    while (1)
    {
        run_monitor(pm);
    };

    return 0;
}