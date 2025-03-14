#ifndef IO_H
#define IO_H

#include "stdint.h"

typedef volatile uint32_t *io_port;

#define IO_ACCESS(port) (*(io_port)(port))

static inline uint32_t toggle_bit(uint32_t data, uint32_t n) {
    data ^= ((uint32_t)1 << n);
    return data;
}

static inline void io_toggle_bit(uint32_t port, uint32_t n)
{
    IO_ACCESS(port) ^= ((uint32_t)1 << n);
}

#endif