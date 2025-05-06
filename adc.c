#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "adc.h"
#include "systick.h"
#include "io.h"
#include "gpio.h"
#include "logger.h"
#include "interrupts.h"

// TODO: maybe make adc interrupt driven

// configure the ADC to use PA1 as the input pin
void configure_adc1() {

	// enable adc clock on APB2 bus
	io_set_bit(RCC_APB2ENR, 8);

	// set PA1 to analog mode
	io_set_bit(GPIOA_MODER, 2);
	io_set_bit(GPIOA_MODER, 3);

	// enable channel 1
	io_set_bit(ADC1_SQR3, 0);
	io_clear_bit(ADC1_SQR3, 1);
	io_clear_bit(ADC1_SQR3, 2);
	io_clear_bit(ADC1_SQR3, 3);
	io_clear_bit(ADC1_SQR3, 4);

	// enable scan mode
	io_set_bit(ADC1_CR1, 8);

	// ensure right alignment of data
	io_clear_bit(ADC1_CR2, 11);

	// trigger EOCS bit to be set at the end of each conversion
	io_set_bit(ADC1_CR2, 10); // 0x40012008

	// enable EOC_interrupt
	io_set_bit(ADC1_CR1, 0);

	nvic_enable_irq(ADC);

	// enable adc
	io_set_bit(ADC1_CR2, 0);

	logger(INFO, "Initilaised ADC on PA1");
}

bool adc_conversion_complete() {
	return (bool)io_get_bit(ADC1_SR, 1);
}

int32_t adc1_manual_conversion() {
	io_set_bit(ADC1_CR2, 30);

	if (wait_for_condition(&adc_conversion_complete, 500) == -1)
    {
		logger(ERROR, "ADC conversion timed out");
        return -1;
    }
	
	uint32_t adc_val = IO_ACCESS(ADC1_DR);

	uint32_t args[] = {adc_val};
	loggerf(DEBUG, "ADC read conversion value: $", args, 1, NULL, 0);

	return adc_val;
}


void adc_handler() {
	logger(DEBUG, "ADC interrupt triggered");
}