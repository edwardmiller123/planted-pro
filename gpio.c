#include <stdbool.h>

#include "gpio.h"
#include "io.h"
#include "systick.h"
#include "interrupts.h"

// alters the state the given pin in the given gpio port using the atomic BSSR register
void gpio_write_pin_atomic(gpio port, uint32_t pin, gpio_action action) {
    if (action == CLEAR) {
        pin += 16;
    }
    switch (port){
        case GPIOA:
            io_set_bit(GPIOA_BSSR, pin);
            break;
        case GPIOB:
            io_set_bit(GPIOB_BSSR, pin);
            break;
        case GPIOC:
            io_set_bit(GPIOC_BSSR, pin);
            break;
        case GPIOD:
            io_set_bit(GPIOD_BSSR, pin);
            break;
    }
}

void toggle_user_led()
{
    io_toggle_bit(GPIOA_ODR, LED2); //0x40020014
}

// Crude error signalling for now
void fast_blink()
{
    for (int i = 0; i < 4; i++)
    {
        toggle_user_led();
        sys_sleep(50);
    }
}

// Initialise GPIOA for the user LED. Requires GPIOA to be enabled first
void init_user_led()
{
    // set GPIO PA5 mode to output. Since this option is encoded as two bits we shouldnt assume
    // one hasnt been set
    io_set_bit(GPIOA_MODER, 10);
    io_clear_bit(GPIOA_MODER, 11);

    // set GPIO PA5 mode to pull up
    io_set_bit(GPIOA_PUPDR, 10);

    // set GPIO type to push pull
    IO_ACCESS(GPIOA_OTYPER) = 0x0U;

    // set GPIO output speed to medium
    IO_ACCESS(GPIOA_OSPEEDR) = 0x400U;
}

// Initialise USART1 with (PA9 and PA10) with the given baud rate. Requires the correct bus to be enabled first
void configure_usart1(uint32_t baud)
{
    // enable gpio alternate function mode (USART) for PA9
    io_clear_bit(GPIOA_MODER, 18);
    io_set_bit(GPIOA_MODER, 19);

    // set the alternate function for PA9 to AF7 (4 bits, 0111)
    io_set_bit(GPIOA_AFRH, 4);
    io_set_bit(GPIOA_AFRH, 5);
    io_set_bit(GPIOA_AFRH, 6);
    io_clear_bit(GPIOA_AFRH, 7);

    // set word length to transmit to 8 bits
    io_clear_bit(USART1_CR1, 12);

    // set stop bit count to 1
    io_clear_bit(USART1_CR2, 12);
    io_clear_bit(USART1_CR2, 13);

    IO_ACCESS(USART1_BRR) = (APB2_DEFAULT_CLK_FREQ + baud / 2) / baud;

    // enable transmit
    io_set_bit(USART1_CR1, 3);

    // enable transmission complete interrupt
    io_set_bit(USART1_CR1, 6);

    // enable TXE interrupt
    io_set_bit(USART1_CR1, 7);

    // enable usart
    io_set_bit(USART1_CR1, 13);

    nvic_enable_irq(USART1);

    // allow time for usart initialise otherwise the first byte sent gets mangled
    sys_sleep(5);
}

bool usart_transmission_successful()
{
    return (bool)io_get_bit(USART1_SR, 6);
}

// Checks if the TDR register is empty and hence we are ready to send data
bool usart_data_empty()
{
    return (bool)io_get_bit(USART1_SR, 7);
}

int usart1_send_byte(uint8_t data)
{
    if (wait_for_condition(&usart_data_empty, 500) == -1)
    {
        return -1;
    }

    IO_ACCESS(USART1_DR) = data;

    // wait for the transmit to complete (1 second timeout)
    if (wait_for_condition(&usart_transmission_successful, 500) == -1)
    {
        return -1;
    }
    return 0;
}

int usart1_send_buffer(uint8_t *buf, uint32_t size)
{
    for (int i = 0; i < size; i++)
    {
        if (usart1_send_byte(buf[i]) == -1)
        {
            return -1;
        }
    }
    return 0;
}

void usart1_handler()
{
}