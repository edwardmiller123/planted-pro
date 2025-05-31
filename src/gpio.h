#ifndef LED_H
#define LED_H

#include <stdint.h>

#include "memmap.h"

#define HC05_BAUD 38400

typedef enum gpio {
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOD
} gpio;

typedef enum gpio_action {
	CLEAR,
	SET
} gpio_action;

typedef enum usart {
	USART1,
	USART3
} usart;

// alters the state the given pin in the given gpio port using the atomic BSSR register
void gpio_write_pin_atomic(gpio port, uint32_t pin, gpio_action action);

// GPIOA //

// base address of GPIOA port
#define GPIOA_BASE (AHB1_BASE + 0x0U)

#define GPIOA_MODER (GPIOA_BASE + 0x0U) // Output mode
#define GPIOA_OTYPER (GPIOA_BASE + 0x4U)
#define GPIOA_OSPEEDR (GPIOA_BASE + 0x8U)
#define GPIOA_PUPDR (GPIOA_BASE + 0x0CU)
#define GPIOA_AFRL (GPIOA_BASE + 0x20U) // Pin alternate functions 
#define GPIOA_AFRH (GPIOA_BASE + 0x24U) // Pin alternate functions 
#define GPIOA_ODR (GPIOA_BASE + 0x14U) // Output data register
#define GPIOA_BSSR (GPIOA_BASE + 0x18U) // Output data register

#define PA1 1
#define PA5 5

// GPIOB //

#define PB8 8
#define PB13 13
#define PB14 14
#define PB15 15

#define GPIOB_BASE (AHB1_BASE + 0x400U)

#define GPIOB_MODER (GPIOB_BASE + 0x0U)
#define GPIOB_OTYPER (GPIOB_BASE + 0x4U)
#define GPIOB_OSPEEDR (GPIOB_BASE + 0x8U)
#define GPIOB_PUPDR (GPIOB_BASE + 0x0CU)
#define GPIOB_AFRH (GPIOB_BASE + 0x24U)
#define GPIOB_ODR (GPIOB_BASE + 0x14U)
#define GPIOB_BSSR (GPIOB_BASE + 0x18U)

// GPIOC //

#define GPIOC_BASE (AHB1_BASE + 0x800U)

#define GPIOC_MODER (GPIOC_BASE + 0x0U)
#define GPIOC_OTYPER (GPIOC_BASE + 0x4U)
#define GPIOC_OSPEEDR (GPIOC_BASE + 0x8U)
#define GPIOC_PUPDR (GPIOC_BASE + 0x0CU)
#define GPIOC_AFRH (GPIOC_BASE + 0x24U)
#define GPIOC_ODR (GPIOC_BASE + 0x14U)
#define GPIOC_BSSR (GPIOC_BASE + 0x18U)

#define PC6 6
#define PC8 8
#define PC9 9
#define PC10 10
#define PC11 11
#define PC12 12

// GPIOD //

#define GPIOD_BASE (AHB1_BASE + 0xC00U)

#define GPIOD_MODER (GPIOD_BASE + 0x0U)
#define GPIOD_OTYPER (GPIOD_BASE + 0x4U)
#define GPIOD_OSPEEDR (GPIOD_BASE + 0x8U)
#define GPIOD_PUPDR (GPIOD_BASE + 0x0CU)
#define GPIOD_AFRH (GPIOD_BASE + 0x24U)
#define GPIOD_ODR (GPIOD_BASE + 0x14U)
#define GPIOD_BSSR (GPIOD_BASE + 0x18U)

#define PD2 2

// LED //

#define LED2 PA5

void toggle_user_led();

void init_user_led();

// Backup error signalling
void fast_blink();

int usart_send_buffer(usart num, uint8_t *buf, uint32_t size);

// USART1 //

#define USART1_BASE (APB2_BASE + 0x1000U)

#define USART1_SR (USART1_BASE + 0x0U)
#define USART1_DR (USART1_BASE + 0x04U)
#define USART1_BRR (USART1_BASE + 0x08U)
#define USART1_CR1 (USART1_BASE + 0x0CU)
#define USART1_CR2 (USART1_BASE + 0x10)

void configure_usart1(uint32_t baud);

void usart1_handler();

// USART3 //

#define USART3_BASE (APB1_BASE + 0x4800U)

#define USART3_SR (USART3_BASE + 0x0U)
#define USART3_DR (USART3_BASE + 0x04U)
#define USART3_BRR (USART3_BASE + 0x08U)
#define USART3_CR1 (USART3_BASE + 0x0CU)
#define USART3_CR2 (USART3_BASE + 0x10)

// Initialise USART2 (PA2 TX, PA3 RX) with the given baud rate. Requires the GPIO bus to be enabled first
void configure_usart3(uint32_t baud);

#endif