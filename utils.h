#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

uint32_t str_len(char *str);

void byte_copy(uint8_t * src, uint8_t * dst, uint32_t n);

void mem_zero(uint8_t *dst, uint32_t n);

// concatenates the two given strings in the given buffer. The new buffer must
// have enough room for both strings and no args can be NULL.
void str_cat(char *str1, char *str2, char *buf);

// getDigitCount returns the number of digits in the given base 10 integer
int get_digit_count(int num);

// returns the reverse of the provided string.
char *reverse_string(char *str, char *new_str);

// converts an integer to a string.
char *int_to_string(uint32_t integer, char *new_str);

static inline uint32_t toggle_bit(uint32_t data, uint32_t n)
{
    data ^= ((uint32_t)1 << n);
    return data;
}

#endif