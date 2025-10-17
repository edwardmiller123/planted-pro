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
    #ifdef LOGLEVEL
        set_log_level((log_level)LOGLEVEL);
    #endif

    // enable gpio ports A - D in AHB1 bus
    io_set_bit(RCC_AHB1ENR, 0);
    io_set_bit(RCC_AHB1ENR, 1);
    io_set_bit(RCC_AHB1ENR, 2);
    io_set_bit(RCC_AHB1ENR, 3);

    init_systick();

    init_debug_led();

    // used for logging
    configure_usart3(DEFAULT_BAUD);

    uint32_t args[1] = {get_system_counter()};
    LOGF(INFO, "Systick, LED2 and USART3 initialisation completed in $ ms", args, 1, NULL, 0);

    configure_bluetooth();

    configure_lcd();

    configure_adc();

    // configured for the HC-05 bluetooth module
    configure_usart1(BLUETOOTH_BAUD);
    LOG(INFO, "Initialised USART1");

    init_heap();

    plant_monitor *pm = init_plant_monitor();
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