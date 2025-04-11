#ifndef IO_H
#define IO_H

#include "stdint.h"
#include "stdbool.h"

typedef volatile uint32_t *io_port;

#define IO_ACCESS(port) (*(io_port)(port))

static inline void io_toggle_bit(uint32_t port, uint32_t n)
{
    IO_ACCESS(port) ^= ((uint32_t)1 << n);
}

// sets the given bit to 1 in the given IO port. Does nothing if the bit is already set
static inline void io_set_bit(uint32_t port, uint32_t n)
{
    IO_ACCESS(port) |= ((uint32_t)1 << n);
}

// sets the given bit to 0 in the given IO port. Does nothing if the bit is already 0
static inline void io_clear_bit(uint32_t port, uint32_t n)
{
    IO_ACCESS(port) &= ~((uint32_t)1 << n);
}

// Check if the given bit is set
static inline bool io_is_bit_set(uint32_t port, uint32_t n) {
    uint32_t isolated_bit = IO_ACCESS(port) & (1 << n);
    if (isolated_bit == 0) {
        return false;
    }
    return true;
}

#endif