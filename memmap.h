#ifndef MEM_MAP_H
#define MEM_MAP_H

// cpu core functions
#define CORE_PERIPH_BASE (0xE0000000U)

// board peripherals
#define PERIPH_BASE (0x40000000U)

// base address of bus AHB1
#define AHB1_OFFSET (0x00020000U)
#define AHB1_BASE (PERIPH_BASE + AHB1_OFFSET)

#endif