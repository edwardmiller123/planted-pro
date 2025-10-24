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

#define MAX_CMD_LEN 64
#define MAX_RESPONSE_LEN 64
#define OK_RESPONSE_LEN 4
#define BT_RESPONSE_TIMEOUT 500

#define AT_COMMAND_MODE "AT+ENAT\r\n"
#define AT_READ_BLE_NAME "AT+LEGN\r\n"
#define AT_SET_BLE_NAME "AT+LENAplanted-pro\r\n"
#define AT_RESET_MODULE "AT+REST\r\n"
#define AT_OK "OK\r\n"

static bt_status status = DISCONNECTED;
static bool has_changed = false;

// Check if USART1 has received a response.
// The accepted number of characters is the length of the OK response.
bool usart1_receive_buffer_holds_response()
{
	return usart1_receive_buffer_len() >= OK_RESPONSE_LEN;
}

// Sends the given command and waits for a response in BT receive buffer
int send_at_cmd_with_response(char *cmd)
{
	if (usart_send_buffer(USART1, (uint8_t *)cmd, str_len(cmd)) == -1)
	{
		LOG(ERROR, "Failed to send buffer over USART1");
		return -1;
	}

	if (wait_for_condition(&usart1_receive_buffer_holds_response, BT_RESPONSE_TIMEOUT) == -1)
	{
		LOG(ERROR, "Timed out waiting for BT module response");
		return -1;
	}

	return 0;
}

// Returns True if the USART1 buffer contains the BT response
bool read_at_ok()
{
	char response_buf[MAX_RESPONSE_LEN];

	uint16_t receive_buffer_len = usart1_receive_buffer_len();
	if (receive_buffer_len > MAX_RESPONSE_LEN)
	{
		LOG(WARNING, "Bluetooth response to long");
		return false;
	}

	for (int i = 0; i < receive_buffer_len; i++)
	{
		// dont need to check the response since we will read 0 bytes if the buffer is empty
		response_buf[i] = usart1_read_byte(NULL);
	}

	response_buf[receive_buffer_len] = '\0';

	return str_cmp(response_buf, AT_OK);
}

int set_ble_name()
{
	// first enter cmd mode
	if (send_at_cmd_with_response(AT_COMMAND_MODE) == -1)
	{
		LOG(ERROR, "BT failed to enter command mode");
		return -1;
	}

	if (!read_at_ok())
	{
		LOG(ERROR, "Didnt receive OK from entering command mode");
		return -1;
	}

	// set BLE name
	if (send_at_cmd_with_response(AT_SET_BLE_NAME) == -1)
	{
		LOG(ERROR, "Failed to send set BLE name command");
		return -1;
	}

	if (!read_at_ok())
	{
		LOG(ERROR, "Didnt receive OK from setting BLE name");
		return -1;
	}

	// reset the BT module
	if (usart_send_buffer(USART1, (uint8_t *)AT_RESET_MODULE, str_len(AT_RESET_MODULE)) == -1)
	{
		LOG(ERROR, "Failed to send buffer over USART1");
		return -1;
	}

	// allow time for the module to reset
	sys_sleep(200);

	LOG(INFO, "BLE name set");

	return 0;
}

void configure_bluetooth()
{
	// Configure PC7 as an EXTI interrupt line

	// set PC7 to input mode
	io_clear_bit(GPIOC_MODER, 14);
	io_clear_bit(GPIOC_MODER, 15);

	// set to trigger on rising and falling edge
	io_set_bit(EXTI_RTSR, EXTI7);
	io_set_bit(EXTI_FTSR, EXTI7);

	// enable SYSCFG clock om APB2 bus
	io_set_bit(RCC_APB2ENR, 14);

	// Map pin PC7 to the EXTI
	io_clear_bit(SYSCFG_EXTICR2, 12);
	io_set_bit(SYSCFG_EXTICR2, 13);
	io_clear_bit(SYSCFG_EXTICR2, 14);
	io_clear_bit(SYSCFG_EXTICR2, 15);

	// wait for the interrupt line to stabaize as we dont want to trigger the interrupt when we first turn
	// on unintentionally
	sys_sleep(500);

	// enable irq line in the nvic
	nvic_enable_irq(EXTI9_5);

	// unmask the interrupt line
	io_set_bit(EXTI_IMR, EXTI7);

	if (set_ble_name() == -1)
	{
		LOG(WARNING, "Failed to set BT module BLE name");
	}
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

	[[maybe_unused]] uint32_t log_args[] = {unix_time};
	LOGF(INFO, "Syncing unix time to $", log_args, 1, NULL, 0);

	update_unix_time(unix_time);

	return 0;
}

void poll_bluetooth(exporter *e)
{
	uint8_t usart1_received_byte = usart1_read_byte(NULL);

	[[maybe_unused]] uint32_t log_args[] = {usart1_received_byte};
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