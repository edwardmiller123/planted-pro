#include "led.h"
#include "io.h"

void toggle_led(uint32_t led)
{
    io_toggle_bit(GPIOAOD_R, led);
}

void configure_led() {
    
}