#ifndef MEM_MAP_H
#define MEM_MAP_H

// cpu core functions
#define CORE_PERIPH_BASE (0xE0000000U)

// board peripherals
#define PERIPH_BASE (0x40000000U)

// base address of bus AHB1
#define AHB1_OFFSET (0x20000U)
#define AHB1_BASE (PERIPH_BASE + AHB1_OFFSET)

#define APB2_OFFSET (0x10000U)
#define APB2_BASE (PERIPH_BASE + APB2_OFFSET)

#define APB2_DEFAULT_CLK_FREQ 16000000

#endif