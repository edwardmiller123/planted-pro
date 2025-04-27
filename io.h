#ifndef IO_H
#define IO_H

#include <stdint.h>

#include "utils.h"

typedef volatile uint32_t *_io_port;

#define IO_ACCESS(port) (*(_io_port)(port))

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

// returns the given bit from the IO port
static inline uint32_t io_get_bit(uint32_t port, uint32_t n) {
    return ((IO_ACCESS(port) & (1 << n)) >> n);
}

// set or clear the given bit in the IO port based of the value of the 
// given condition bit
static inline void io_match_bit(uint32_t port, uint32_t condition_bit, uint32_t n)
{
    switch (condition_bit) {
        case 1:
            io_set_bit(port, n);
            break;
        case 0:
            io_clear_bit(port, n);
            break;
    }
}
#endif