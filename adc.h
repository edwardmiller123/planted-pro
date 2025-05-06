#ifndef ADC_H
#define ADC_H

#include "memmap.h"

#define ADC_BASE (APB2_BASE + 0x2000U)

#define ADC1_BASE (ADC_BASE + 0x0U)

#define ADC1_SR (ADC1_BASE + 0x0U)
#define ADC1_CR1 (ADC1_BASE + 0x4U)
#define ADC1_CR2 (ADC1_BASE + 0x8U) 
#define ADC1_SQR3 (ADC1_BASE + 0x34U) // channel select
#define ADC1_SMPR1 (ADC1_BASE + 0xCU)
#define ADC1_DR (ADC1_BASE + 0x4CU)

void configure_adc1();

// manually trigger a conversion and read the result
int32_t adc1_manual_conversion();

void adc_handler();

#endif