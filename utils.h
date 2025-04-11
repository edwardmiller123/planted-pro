#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

uint32_t strlen(char *str);

static inline uint32_t toggle_bit(uint32_t data, uint32_t n)
{
    data ^= ((uint32_t)1 << n);
    return data;
}

#endif