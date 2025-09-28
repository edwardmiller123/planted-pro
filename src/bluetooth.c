#include <stdint.h>

#include "bluetooth.h"
#include "io.h"
#include "export.h"
#include "time.h"
#include "utils.h"
#include "logger.h"
#include "usart.h"
#include "interrupts.h"
#include "gpio.h"

#define SYNC_CODE 'T'

static bt_status status = DISCONNECTED;
static bool has_changed = false;

void configure_bluetooth()
{
	// Configure PC7 as an EXTI interrupt line

	// set PC7 to input mode
	io_clear_bit(GPIOC_MODER, 14);
	io_clear_bit(GPIOC_MODER, 15);

	// unmask the interrupt line
	io_set_bit(EXTI_IMR, EXTI7);

	// set to trigger on rising and falling edge
	io_set_bit(EXTI_RTSR, EXTI7);
	io_set_bit(EXTI_FTSR, EXTI7);

	// Map pin PC7 to the EXTI
	io_clear_bit(SYSCFG_EXTICR2, 12);
	io_set_bit(SYSCFG_EXTICR2, 13);
	io_clear_bit(SYSCFG_EXTICR2, 14);
	io_clear_bit(SYSCFG_EXTICR2, 15);

	// enable irq line in the nvic
	nvic_enable_irq(EXTI9_5);

	// TODO: use usart to set the bluetooth module name
}

void bluetooth_irq_handler()
{
	LOG(DEBUG, "EXTI9_5 interrupt triggered");

	status = !status;
	has_changed = true;

	exti_clear_pending_irq(EXTI7);
}

bt_status get_bluetooth_status()
{
	return status;
}

bool bt_status_changed()
{
	if (has_changed)
	{
		has_changed = false;
		return true;
	}
	return has_changed;
}

int read_and_update_time()
{
	uint8_t time_buf[UNIX_DIGIT_COUNT];
	result_code usart_read_status;
	uint8_t value;
	for (int i = 0; i < UNIX_DIGIT_COUNT; i++)
	{
		value = usart1_read_byte(&usart_read_status);
		if (usart_read_status == EMPTY)
		{
			LOG(ERROR, "Too few bytes read from USART1");
			return -1;
		}
		time_buf[i] = value;
	}

	result_code conversion_status;
	uint32_t unix_time = string_to_uint_base_10((char *)time_buf, UNIX_DIGIT_COUNT, &conversion_status);
	if (conversion_status == BAD_INPUT)
	{
		LOG(ERROR, "Time in wrong format");
		return -1;
	}

	uint32_t log_args[] = {unix_time};
	LOGF(INFO, "Syncing unix time to $", log_args, 1, NULL, 0);

	update_unix_time(unix_time);

	return 0;
}

void poll_bluetooth(exporter *e)
{
	uint8_t usart1_received_byte = usart1_read_byte(NULL);

	uint32_t log_args[] = {usart1_received_byte};
	LOGF(DEBUG, "Read byte $ from USART1 receive buffer", log_args, 1, NULL, 0);

	if (usart1_received_byte == (uint8_t)SYNC_CODE)
	{
		if (read_and_update_time() == -1)
		{
			LOG(ERROR, "Failed read sync time");
		}

		e->send_data = true;
	}
}