#include <stdint.h>

#include "export.h"
#include "time.h"
#include "utils.h"
#include "logger.h"
#include "usart.h"

#define SYNC_CODE 'T'

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
			logger(ERROR, "Too few bytes read from USART1");
			return -1;
		}
		time_buf[i] = value;
	}

	result_code conversion_status;
	uint32_t unix_time = string_to_uint_base_10((char *)time_buf, UNIX_DIGIT_COUNT, &conversion_status);
	if (conversion_status == BAD_INPUT)
	{
		logger(ERROR, "Time in wrong format");
		return -1;
	}

	uint32_t log_args[] = {unix_time};
	loggerf(INFO, "Syncing unix time to $", log_args, 1, NULL, 0);

	update_unix_time(unix_time);

	return 0;
}

void poll_bluetooth(exporter *e)
{
	uint8_t usart1_received_byte = usart1_read_byte(NULL);

	uint32_t log_args[] = {usart1_received_byte};
	loggerf(DEBUG, "Read byte $ from USART1 receive buffer", log_args, 1, NULL, 0);

	if (usart1_received_byte == (uint8_t)SYNC_CODE)
	{
		if (read_and_update_time() == -1)
		{
			logger(ERROR, "Failed read sync time");
		}

		e->send_data = true;
	}
}