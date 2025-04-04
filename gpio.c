#include <stdbool.h>

#include "gpio.h"
#include "io.h"
#include "systick.h"

void toggle_user_led()
{
    io_toggle_bit(GPIOAOD_R, LED2);
}

// Crude error signalling for now
void fast_blink() {
    for (int i = 0; i < 4; i++) {
        toggle_user_led();
        sys_sleep(50);
    }
}

void init_user_led() {
    // enable GPIOA
    IO_ACCESS(RCC_AHB1ENR) = 0x01U;

    // set GPIO mode to output. Since this option is encoded as two bits we shouldnt assume
    // one hasnt been set
    io_clear_bit(GPIOA_MODER, 9);
    io_set_bit(GPIOA_MODER, 10);

    // set GPIO mode to pull up
    io_set_bit(GPIOA_PUPDR, 10);

    // set GPIO type to push pull
    IO_ACCESS(GPIOA_OTYPER) = 0x0U;

    // set GPIO output speed to medium
    IO_ACCESS(GPIOA_OSPEEDR) = 0x400U;
}

// Initialise the USART to use PA9 for TX and PA10 for RX
// TODO: Enable the usart interrupts to signal the transmission is complete
// TODO: Specify the alternative function number to use
void init_usart() {
    // enable gpio alternate function mode (USART) for PA9
    io_clear_bit(GPIOA_MODER, 18);
    io_set_bit(GPIOA_MODER, 19);

    // enable usart
    io_set_bit(USART1_CR1, 13);

    // set word length to transmit to 8 bits
    io_clear_bit(USART1_CR1, 12);

    // set stop bit count to 1
    io_clear_bit(USART1_CR2, 12);
    io_clear_bit(USART1_CR2, 13);

    // set baud rate to 115200
    // usartdiv = max_clock_freq / (16 * baud_rate) = 90000000 / (16 * 115200)
    // = 48.8281
    // Mentissa = 48, Fraction = 0.8281 * 16 = 13.2496 ~ 13
    // USARTDIV = DIV_Mantisa[11:0] DIV_Fraction[3:0] = [000000110000][1101] = 0x30D
    IO_ACCESS(USART1_BRR) = 0x30D;

    // enable transmit
    io_set_bit(USART1_CR1, 3);
}

bool uart_transmission_succesfful() {
    return io_is_bit_set(USART1_SR, 6);
}


void usart_send_byte(uint8_t data) {
    IO_ACCESS(USART1_DR) = data;
    // wait for transmission to complete (2 second timeout)
    int ok = wait_for_condition(&uart_transmission_succesfful, 2000);
    if (ok == -1) {
        fast_blink();
    }
}