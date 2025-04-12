#ifndef LED_H
#define LED_H

#include <stdint.h>

#include "memmap.h"

// base address of RCC module
#define RCC_BASE (AHB1_BASE + 0x3800U)

// AHB1 peripheral reset
#define RCC_AHB1RSTR (RCC_BASE + 0x10U)

// AHB1 enable register
#define RCC_AHB1ENR (RCC_BASE + 0x30U)

#define RCC_APB2ENR (RCC_BASE + 0x44)

// AHB1 enable in low power register
#define AHB1LPEN_R_OFFSET (0x50U)
#define RCC_AHB1LPEN_R (RCC_BASE + AHB1LPEN_R_OFFSET)

// base address of GPIOA port
#define GPIOA_BASE (AHB1_BASE + 0x0U)

// base address of GPIOA mode register
#define GPIOA_MODER (GPIOA_BASE + 0x0U)

#define GPIOA_OTYPER (GPIOA_BASE + 0x4U)

#define GPIOA_OSPEEDR (GPIOA_BASE + 0x8U)

#define GPIOA_PUPDR (GPIOA_BASE + 0x0CU)

// Pin alternate functions 
#define GPIOA_AFRH (GPIOA_BASE + 0x24U)

// GPIOA output data register
#define GPIOAOD_R (GPIOA_BASE + 0x14U)

#define PA5 5
#define LED2 PA5

void toggle_user_led();

void init_user_led();

void fast_blink();

#define USART1_OFFSET (0x1000U)
#define USART1_BASE (APB2_BASE + USART1_OFFSET)

#define USART1_SR (USART1_BASE + 0x0U)
#define USART1_DR (USART1_BASE + 0x04U)
#define USART1_BRR (USART1_BASE + 0x08U)
#define USART1_CR1 (USART1_BASE + 0x0CU)
#define USART1_CR2 (USART1_BASE + 0x10)

void configure_usart1(uint32_t baud);

int usart1_send_byte(uint8_t data);

int usart1_send_buffer(uint8_t * buf, uint32_t size);

void usart1_handler();

#endif