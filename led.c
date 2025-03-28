#include "led.h"
#include "io.h"

void toggle_user_led()
{
    io_toggle_bit(GPIOAOD_R, LED2);
}

void init_user_led() {
    // enable GPIOA
    IO_ACCESS(RCC_AHB1EN_R) = 0x01U;

    // set GPIO mode to output
    io_set_bit(GPIOAMODE_R, 10);

    // set GPIO mode to pull up
    io_set_bit(GPIOAPUPD_R, 10);

    // set GPIO type to push pull
    IO_ACCESS(GPIOAOTYPE_R) = 0x0U;

    // set GPIO output speed to medium
    IO_ACCESS(GPIOAOSPEED_R) = 0x400U;
}