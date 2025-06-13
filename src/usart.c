#include <stdbool.h>

#include "usart.h"
#include "gpio.h"
#include "io.h"
#include "systick.h"
#include "utils.h"
#include "interrupts.h"

// Initialise USART1 (PA9 TX, PA10 RX) with the given baud rate. Requires the GPIO bus to be enabled first
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

    // enable gpio alternate function mode (USART) for PA10
    io_clear_bit(GPIOA_MODER, 20);
    io_set_bit(GPIOA_MODER, 21);

    // set the alternate function for PA10 to AF7 (4 bits, 0111)
    io_set_bit(GPIOA_AFRH, 8);
    io_set_bit(GPIOA_AFRH, 9);
    io_set_bit(GPIOA_AFRH, 10);
    io_clear_bit(GPIOA_AFRH, 11);

    // set word length for transmit to 8 bits
    io_clear_bit(USART1_CR1, 12);

    // set stop bit count to 1
    io_clear_bit(USART1_CR2, 12);
    io_clear_bit(USART1_CR2, 13);

    IO_ACCESS(USART1_BRR) = (DEFAULT_CLK_FREQ + baud / 2) / baud;

    // enable transmit
    io_set_bit(USART1_CR1, 3);

    // // enable transmission complete interrupt
    // io_set_bit(USART1_CR1, 6);

    // // enable TXE interrupt
    // io_set_bit(USART1_CR1, 7);

    // enable receive
    io_set_bit(USART1_CR1, 2);

    // enable received data ready interrupt
    io_set_bit(USART1_CR1, 5);

    // enable usart
    io_set_bit(USART1_CR1, 13);

    nvic_enable_irq(IRQ_USART1);

    // allow time for usart initialise otherwise the first byte sent gets mangled
    sys_sleep(5);
}

// Initialise USART3 (PB10 TX) with the given baud rate. Requires the GPIO bus to be enabled first
void configure_usart3(uint32_t baud)
{

    // Enable APB1 bus for USART3
    io_set_bit(RCC_APB1ENR, 18);

    // enable gpio alternate function mode (USART) for PB10
    io_clear_bit(GPIOB_MODER, 20);
    io_set_bit(GPIOB_MODER, 21);

    // set the alternate function for PB10 to AF7 (4 bits, 0111)
    io_set_bit(GPIOB_AFRH, 8);
    io_set_bit(GPIOB_AFRH, 9);
    io_set_bit(GPIOB_AFRH, 10);
    io_clear_bit(GPIOB_AFRH, 11);

    // set word length to transmit to 8 bits
    io_clear_bit(USART3_CR1, 12);

    // set stop bit count to 1
    io_clear_bit(USART3_CR2, 12);
    io_clear_bit(USART3_CR2, 13);

    IO_ACCESS(USART3_BRR) = (DEFAULT_CLK_FREQ + baud / 2) / baud;

    // // enable transmission complete interrupt
    // io_set_bit(USART3_CR1, 6);

    // // enable TXE interrupt
    // io_set_bit(USART3_CR1, 7);

    // enable transmit
    io_set_bit(USART3_CR1, 3);

    // enable usart
    io_set_bit(USART3_CR1, 13);

    nvic_enable_irq(IRQ_USART3);

    // allow time for usart initialise otherwise the first byte sent gets mangled
    sys_sleep(5);
}

bool usart1_transmission_successful()
{
    return (bool)io_get_bit(USART1_SR, 6);
}

// Checks if the TDR register is empty and hence we are ready to send data
bool usart1_ready()
{
    return (bool)io_get_bit(USART1_SR, 7);
}

int usart1_send_byte(uint8_t data)
{
    if (wait_for_condition(&usart1_ready, 500) == -1)
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

bool usart3_transmission_successful()
{
    return (bool)io_get_bit(USART3_SR, 6);
}

// Checks if the TDR register is empty and hence we are ready to send data
bool usart3_ready()
{
    return (bool)io_get_bit(USART3_SR, 7);
}

int usart3_send_byte(uint8_t data)
{
    if (wait_for_condition(&usart3_ready, 500) == -1)
    {
        return -1;
    }

    IO_ACCESS(USART3_DR) = data;

    // wait for the transmit to complete (500ms second timeout)
    if (wait_for_condition(&usart3_transmission_successful, 500) == -1)
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
    case USART3:
        send_func = &usart3_send_byte;
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

void usart1_irq_handler()
{
    while (1) {;};
}

void usart3_irq_handler()
{

}