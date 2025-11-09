#ifndef FPMATH_H
#define FPMATH_H

#include <stdint.h>

#define SHIFT 8

// fixed point division ignoring the fractional parts
static inline uint32_t fp_divide(uint32_t numerator, uint32_t denominator) {
	return ((numerator << SHIFT) / denominator) >> SHIFT;
}

// finds the percentage of the given integers uisng fixed point arithmetic
static inline uint32_t fp_percentage(uint32_t numerator, uint32_t denominator) {
	return (((numerator << SHIFT) * 100) / denominator) >> SHIFT;
}


#endif