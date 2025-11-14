#ifndef ADC_H
#define ADC_H

#include "memmap.h"
#include "utils.h"

#define ADC_BASE (APB2_BASE + 0x2000U)

#define ADC1_BASE (ADC_BASE + 0x0U)

#define ADC1_SR (ADC1_BASE + 0x0U)
#define ADC1_CR1 (ADC1_BASE + 0x4U)
#define ADC1_CR2 (ADC1_BASE + 0x8U) 
#define ADC1_SQR3 (ADC1_BASE + 0x34U) // channel select
#define ADC1_SMPR1 (ADC1_BASE + 0xCU)
#define ADC1_DR (ADC1_BASE + 0x4CU)

#define ADC2_BASE (ADC_BASE + 0x100U)

#define ADC2_SR (ADC2_BASE + 0x0U)
#define ADC2_CR1 (ADC2_BASE + 0x4U)
#define ADC2_CR2 (ADC2_BASE + 0x8U) 
#define ADC2_SQR3 (ADC2_BASE + 0x34U) // channel select
#define ADC2_SMPR1 (ADC2_BASE + 0xCU)
#define ADC2_DR (ADC2_BASE + 0x4CU)

#define ADC3_BASE (ADC_BASE + 0x200U)

#define ADC3_SR (ADC3_BASE + 0x0U)
#define ADC3_CR1 (ADC3_BASE + 0x4U)
#define ADC3_CR2 (ADC3_BASE + 0x8U) 
#define ADC3_SQR3 (ADC3_BASE + 0x34U) // channel select
#define ADC3_SMPR1 (ADC3_BASE + 0xCU)
#define ADC3_DR (ADC3_BASE + 0x4CU)

typedef enum adc {
	ADC1,
	ADC2,
	ADC3
} adc;

void configure_adc();

// manually trigger a conversion from the given ADC and read the result
uint32_t adc_manual_conversion(adc adc_num, result_code * result);

// Read the digital value of Vref which is always equivalent to ~1.21V
uint16_t adc_read_vrefint(result_code *result);

#endif