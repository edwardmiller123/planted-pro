#ifndef LED_H
#define LED_H

#include <stdint.h>

#include "memmap.h"

// base address of GPIOA port
#define GPIOA_OFFSET (0x0U)
#define GPIOA_BASE (AHB1_BASE + GPIOA_OFFSET)

// base address of RCC module
#define RCC_OFFSET (0x00003800U)
#define RCC_BASE (AHB1_BASE + RCC_OFFSET)

// AHB1 peripheral reset
#define AHB1RST_R_OFFSET (0x00000010U)
#define RCC_AHB1RST_R ((RCC_BASE + AHB1RST_R_OFFSET))

// AHB1 enable register
#define AHB1EN_R_OFFSET (0x00000030U)
#define RCC_AHB1EN_R ((RCC_BASE + AHB1EN_R_OFFSET))

// AHB1 enable in low power register
#define AHB1LPEN_R_OFFSET (0x50U)
#define RCC_AHB1LPEN_R ((RCC_BASE + AHB1LPEN_R_OFFSET))

// base address of GPIOA mode register
#define GPIOAMODE_R_OFFSET (0x0U)
#define GPIOAMODE_R (GPIOA_BASE + GPIOAMODE_R_OFFSET)

#define GPIOAOTYPE_R_OFFSET (0x4U)
#define GPIOAOTYPE_R (GPIOA_BASE + GPIOAOTYPE_R_OFFSET)

#define GPIOAOSPEED_R_OFFSET (0x8U)
#define GPIOAOSPEED_R (GPIOA_BASE + GPIOAOSPEED_R_OFFSET)

#define GPIOAPUPD_R_OFFSET (0x0CU)
#define GPIOAPUPD_R (GPIOA_BASE + GPIOAPUPD_R_OFFSET)

// base address of GPIOA output data register
#define GPIOAOD_R_OFFSET (0x00000014U)
#define GPIOAOD_R (GPIOA_BASE + GPIOAOD_R_OFFSET)

#define LED2 5

void toggle_user_led();

void init_user_led();

#endif