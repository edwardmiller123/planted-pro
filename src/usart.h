#ifndef USART_H
#define USART_H

#include <stdint.h>

#include "memmap.h"

#define HC05_BAUD 9600
#define DEFAULT_BAUD 115200

typedef enum usart {
	USART1,
	USART3
} usart;

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

// Initialise USART3 (PB10 TX) with the given baud rate. Requires the GPIO bus to be enabled first
void configure_usart3(uint32_t baud);


#endif