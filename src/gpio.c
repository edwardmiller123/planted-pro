#include <stdbool.h>

#include "gpio.h"
#include "io.h"
#include "systick.h"
#include "interrupts.h"

// alters the state of the given pin in the given gpio port using the atomic BSSR register
void gpio_write_pin_atomic(gpio port, uint32_t pin, gpio_action action)
{
    if (action == CLEAR)
    {
        pin += 16;
    }
    switch (port)
    {
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
    io_toggle_bit(GPIOA_ODR, LED2);
}

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

// Initialise USART1 (PA9) with the given baud rate. Requires the GPIO bus to be enabled first
void configure_usart1(uint32_t baud)
{
    // Enable APB2 bus for USART1
    io_set_bit(RCC_APB2ENR, 4);

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

    IO_ACCESS(USART1_BRR) = (DEFAULT_CLK_FREQ + baud / 2) / baud;

    // enable transmit
    io_set_bit(USART1_CR1, 3);

    // enable transmission complete interrupt
    io_set_bit(USART1_CR1, 6);

    // enable TXE interrupt
    io_set_bit(USART1_CR1, 7);

    // enable usart
    io_set_bit(USART1_CR1, 13);

    nvic_enable_irq(IRQ_USART1);

    // allow time for usart initialise otherwise the first byte sent gets mangled
    sys_sleep(5);
}

// Initialise USART2 (PA2 TX, PA3 RX) with the given baud rate. Requires the GPIO bus to be enabled first
void configure_usart2(uint32_t baud)
{
    // Enable APB1 bus for USART2
    io_set_bit(RCC_APB1ENR, 17);

    // enable gpio alternate function mode (USART) for PA2
    io_clear_bit(GPIOA_MODER, 4);
    io_set_bit(GPIOA_MODER, 5);

    // set the alternate function for PA2 to AF7 (4 bits, 0111)
    io_set_bit(GPIOA_AFRL, 8);
    io_set_bit(GPIOA_AFRL, 9);
    io_set_bit(GPIOA_AFRL, 10);
    io_clear_bit(GPIOA_AFRL, 11);

    // enable gpio alternate function mode (USART) for PA3
    io_clear_bit(GPIOA_MODER, 6);
    io_set_bit(GPIOA_MODER, 7);

    // set the alternate function for PA3 to AF7 (4 bits, 0111)
    io_set_bit(GPIOA_AFRL, 12);
    io_set_bit(GPIOA_AFRL, 13);
    io_set_bit(GPIOA_AFRL, 14);
    io_clear_bit(GPIOA_AFRL, 15);

    // set word length to transmit to 8 bits
    io_clear_bit(USART2_CR1, 12);

    // set stop bit count to 1
    io_clear_bit(USART2_CR2, 12);
    io_clear_bit(USART2_CR2, 13);

    IO_ACCESS(USART2_BRR) = (DEFAULT_CLK_FREQ + baud / 2) / baud;

    // enable transmit
    io_set_bit(USART2_CR1, 3);

    // enable transmission complete interrupt
    io_set_bit(USART2_CR1, 6);

    // enable TXE interrupt
    io_set_bit(USART2_CR1, 7);

    // enable receive
    io_set_bit(USART2_CR1, 2);

    // enable receive complete interrupt
    io_set_bit(USART2_CR1, 5);

    // enable usart
    io_set_bit(USART2_CR1, 13);

    nvic_enable_irq(IRQ_USART2);

    // allow time for usart initialise otherwise the first byte sent gets mangled
    sys_sleep(5);
}

bool usart1_transmission_successful()
{
    return (bool)io_get_bit(USART1_SR, 6);
}

// Checks if the TDR register is empty and hence we are ready to send data
bool usart1_data_empty()
{
    return (bool)io_get_bit(USART1_SR, 7);
}

int usart1_send_byte(uint8_t data)
{
    if (wait_for_condition(&usart1_data_empty, 500) == -1)
    {
        return -1;
    }

    IO_ACCESS(USART1_DR) = data;

    // wait for the transmit to complete (1 second timeout)
    if (wait_for_condition(&usart1_transmission_successful, 500) == -1)
    {
        return -1;
    }
    return 0;
}

bool usart2_transmission_successful()
{
    return (bool)io_get_bit(USART2_SR, 6);
}

// Checks if the TDR register is empty and hence we are ready to send data
bool usart2_data_empty()
{
    return (bool)io_get_bit(USART2_SR, 7);
}

int usart2_send_byte(uint8_t data)
{
    if (wait_for_condition(&usart2_data_empty, 500) == -1)
    {
        return -1;
    }

    IO_ACCESS(USART2_DR) = data;

    // wait for the transmit to complete (1 second timeout)
    if (wait_for_condition(&usart2_transmission_successful, 500) == -1)
    {
        return -1;
    }
    return 0;
}

int usart_send_buffer(usart num, uint8_t *buf, uint32_t size)
{
    int (*send_func)(uint8_t);
    switch (num)
    {
    case USART1:
        send_func = &usart1_send_byte;
        break;
    case USART2:
        send_func = &usart2_send_byte;
        break;
    }

    for (int i = 0; i < size; i++)
    {
        if (send_func(buf[i]) == -1)
        {
            return -1;
        }
    }
    return 0;
}

void usart1_handler()
{
}