#ifndef MEM_MAP_H
#define MEM_MAP_H

// cpu core functions
#define CORE_PERIPH_BASE (0xE0000000U)

// board peripherals
#define PERIPH_BASE (0x40000000U)

// AHB1 // 
#define AHB1_BASE (PERIPH_BASE + 0x20000U)

// APB1 //
#define APB1_BASE (PERIPH_BASE + 0x0U)

// APB2 //
#define APB2_BASE (PERIPH_BASE + 0x10000U)

#define DEFAULT_CLK_FREQ 16000000

// RCC //
// base address of RCC module
#define RCC_BASE (AHB1_BASE + 0x3800U)

#define RCC_AHB1RSTR (RCC_BASE + 0x10U)
#define RCC_AHB1ENR (RCC_BASE + 0x30U)

#define RCC_APB1RSTR (RCC_BASE + 0x20U)
#define RCC_APB1ENR (RCC_BASE + 0x40)

#define RCC_APB2RSTR (RCC_BASE + 0x24U)
#define RCC_APB2ENR (RCC_BASE + 0x44)

#endif