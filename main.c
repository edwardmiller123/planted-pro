#include <stdio.h>
#include <stdint.h>

#include "io.h"

#include "interrupts.c"

#define PERIPH_BASE (0x40000000U)

// base address of bus AHB1
#define AHB1_OFFSET (0x00020000U)
#define AHB1_BASE (PERIPH_BASE + AHB1_OFFSET)

// base address of GPIOA port
#define GPIOA_OFFSET (0x0U)
#define GPIOA_BASE (AHB1_BASE + GPIOA_OFFSET)

// base address of RCC module
#define RCC_OFFSET (0x00003800U)
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)

// base address of AHB1 enable register
#define AHB1EN_R_OFFSET (0x00000030U)
#define RCC_AHB1EN_R ((RCC_BASE + AHB1EN_R_OFFSET))

// base address of GPIOA mode register
#define GPIOAMODE_R_OFFSET (0x0U)
#define GPIOAMODE_R (GPIOA_BASE + GPIOAMODE_R_OFFSET)

// base address of GPIOA output data register
#define GPIOAOD_R_OFFSET (0x00000014U)
#define GPIOAOD_R (GPIOA_BASE + GPIOAOD_R_OFFSET)

void toggle_led(uint32_t led)
{
    io_toggle_bit(GPIOAOD_R, led);
}

int main()
{
    IO_ACCESS(RCC_AHB1EN_R) = 0x01U;

    IO_ACCESS(GPIOAMODE_R) = 0x400U;

    while (1)
    {
        // blink the led by flipping the correct bit
        toggle_led((uint32_t)5);
        for (uint32_t i = 0; i < 400000; i++){;}
    };
}