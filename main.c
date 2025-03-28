#include <stdio.h>
#include <stdint.h>

#include "interrupts.h"
#include "io.h"
#include "led.h"
#include "systick.h"

#include "interrupt_table.c"

void main(void)
{
    init_user_led();

    init_systick();

    while (1)
    {
        toggle_user_led();
        sys_sleep(500);
    };
}