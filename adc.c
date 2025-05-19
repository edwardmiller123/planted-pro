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

// configure the ADC1 to use channel 1 on PA1 as the input pin
void configure_adc1()
{

	// enable adc1 clock on APB2 bus
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
	io_set_bit(ADC1_CR2, 10);

	// enable EOC_interrupt
	io_set_bit(ADC1_CR1, 0);

	// enable adc
	io_set_bit(ADC1_CR2, 0);

	logger(INFO, "Initilaised ADC1 with PA1, channel 1 as input");
}

// configure ADC2 to use channel 2 on PA2 as the input pin
// TODO: adc2 isnt reading values correctly
void configure_adc2()
{
	// enable adc2 clock on APB2 bus
	io_set_bit(RCC_APB2ENR, 9);

	// set PA2 to analog mode
	io_set_bit(GPIOA_MODER, 4);
	io_set_bit(GPIOA_MODER, 5);

	// enable channel 2
	io_clear_bit(ADC2_SQR3, 0);
	io_set_bit(ADC2_SQR3, 1);
	io_clear_bit(ADC2_SQR3, 2);
	io_clear_bit(ADC2_SQR3, 3);
	io_clear_bit(ADC2_SQR3, 4);

	// enable scan mode
	io_set_bit(ADC2_CR1, 8);

	// ensure right alignment of data
	io_clear_bit(ADC2_CR2, 11);

	// trigger EOCS bit to be set at the end of each conversion
	io_set_bit(ADC2_CR2, 10); // 0x40012008

	// enable EOC_interrupt
	io_set_bit(ADC2_CR1, 0);

	// enable adc
	io_set_bit(ADC2_CR2, 0);

	logger(INFO, "Initilaised ADC2 with PA2, channel 2 as input");
}

// configure ADC1 and ADC2
void configure_adc()
{
	configure_adc1();

	configure_adc2();

	nvic_enable_irq(ADC);
}

bool adc1_conversion_complete()
{
	return (bool)io_get_bit(ADC1_SR, 1);
}

bool adc2_conversion_complete()
{
	return (bool)io_get_bit(ADC2_SR, 1);
}

int wait_for_adc(adc adc_num, uint32_t timeout)
{
	switch (adc_num)
	{
	case ADC1:
		if (wait_for_condition(&adc1_conversion_complete, timeout) == -1)
		{
			logger(ERROR, "ADC1 conversion timed out");
			return -1;
		}
		break;
	case ADC2:
		if (wait_for_condition(&adc2_conversion_complete, timeout) == -1)
		{
			logger(ERROR, "ADC2 conversion timed out");
			return -1;
		}
		break;
	}
	return 0;
}

int32_t adc_manual_conversion(adc adc_num)
{

	uint32_t ctrl_reg;
	uint32_t data_reg;
	char * log_str;
	switch (adc_num)
	{
	case ADC1:
		ctrl_reg = ADC1_CR2;
		data_reg = ADC1_DR;
		log_str = "ADC1";
		break;
	case ADC2:
		ctrl_reg = ADC2_CR2;
		data_reg = ADC2_DR;
		log_str = "ADC2";
		break;
	}

	io_set_bit(ctrl_reg, 30);

	char * logger_args_str[] = {log_str};

	if (wait_for_adc(adc_num, 500) == -1)
	{
		loggerf(ERROR, "& conversion failed", NULL, 0, logger_args_str, 1);
		return -1;
	}

	uint32_t adc_val = IO_ACCESS(data_reg);

	uint32_t logger_args_int[] = {adc_val};
	loggerf(DEBUG, "& read conversion value: $", logger_args_int, 1, logger_args_str, 1);

	return adc_val;
}

void adc_handler()
{
	logger(DEBUG, "ADC interrupt triggered");
}