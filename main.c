#include "io.h"

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

extern unsigned int _stack;
extern void reset_handler(void);

__attribute__ ((packed))
typedef struct int_table {
    unsigned int * initial_stack;
    void * reset;
} int_table;

__attribute__ ((section(".interrupts")))
int_table interrupts = {
    .initial_stack = &_stack,
    .reset = &reset_handler,
};

void toggle_led(unsigned int led)
{
    unsigned int led_state = read_word((void *)GPIOAOD_R);
    led_state = toggle_bit(led_state, led);
    write_word((void *)GPIOAOD_R, led_state);
}

int main()
{

    // // enable RCC clock for GPIOA
    // // set bit 0 to enable the clock
    write_word((unsigned int *)RCC_AHB1EN_R, 0x01U);

    // // set the pin mode to output for PA5 
    // // (flipping bit 10 as each pin has 2 corresponding bits)
    write_word((unsigned int *)GPIOAMODE_R, 0x400U);

    // *(unsigned int *)GPIOAMODE_R |= (1 << 10);

    while (1)
    {
        // blink the led by flipping the correct bit
        toggle_led((unsigned int)5);
        for (int i = 0; i < 100000; i++){;}
    };
}