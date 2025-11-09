#include <stdint.h>

#include "export.h"
#include "ringbuf.h"
#include "time.h"
#include "heap.h"
#include "utils.h"
#include "logger.h"
#include "usart.h"

#define MAX_JSON_SIZE 1024
#define MAX_DIGITS 10
#define MAX_DATA_POINT_JSON_SIZE 64
#define SYNC_CODE 'T'
#define UNIX_DIGIT_COUNT 10

#ifndef GIT_SHA

#define GIT_SHA "0"

#endif

exporter *init_exporter(uint16_t poll_interval, uint16_t data_point_count)
{
	exporter *e = malloc(sizeof(exporter));
	if (e == NULL)
	{
		LOG(ERROR, "Failed to malloc exporter");
		return NULL;
	}

	ring_buffer *buf = init_ring_buffer();
	if (buf == NULL)
	{
		LOG(ERROR, "Failed to initialise export buffer");
		return NULL;
	}

	e->export_buf = buf;
	e->data_point_limit = data_point_count;
	e->poll_interval = poll_interval;
	e->last_read_ts = 0;
	e->send_data = false;

	LOG(INFO, "Initialised Exporter");

	return e;
}

int store_data_for_export(exporter *e, uint32_t ts, uint8_t light_percent, uint8_t water_percent, uint8_t battery_percent)
{
	// remove the oldest data point if the queue is full
	if (e->export_buf->word_count == e->data_point_limit)
	{
		result_code read_result;
		uint32_t oldest_data = ring_buffer_read_word(e->export_buf, &read_result);
		if (read_result == SUCCESS)
		{
			[[maybe_unused]] uint32_t log_args[] = {((data_point *)oldest_data)->ts, ((data_point *)oldest_data)->light_percent, ((data_point *)oldest_data)->water_percent, ((data_point *)oldest_data)->battery_percent};
			LOGF(INFO, "Discarded values from export list. TS: $, Light: $, Water: $, Battery: $", log_args, 4, NULL, 0);

			free((void *)oldest_data);
		}
	}

	data_point *dp = malloc(sizeof(data_point));
	if (dp == NULL)
	{
		LOG(ERROR, "Failed to malloc data point");
		return -1;
	}

	dp->ts = ts;
	dp->light_percent = light_percent;
	dp->water_percent = water_percent;
	dp->battery_percent = battery_percent;

	ring_buffer_write_word(e->export_buf, (uint32_t)dp);

	[[maybe_unused]] uint32_t log_args[] = {dp->ts, dp->light_percent, dp->water_percent, dp->battery_percent};
	LOGF(INFO, "Stored values for export. TS: $, Light: $, Water: $, Battery: $", log_args, 4, NULL, 0);

	return 0;
}

// converts a single data_point struct to a json string
char *data_point_to_json(data_point *dp, uint8_t *buf)
{
	uint8_t *buf_pos = byte_copy((uint8_t *)"{", buf, 1);

	char *field_divider = ", ";
	uint32_t field_divider_len = str_len(field_divider);

	char *ts_field = "\"ts\": ";
	uint32_t ts_field_len = str_len(ts_field);
	buf_pos = byte_copy((uint8_t *)ts_field, buf_pos, ts_field_len);

	char ts_buf[MAX_DIGITS] = {'\0'};
	char *ts_val = int_to_string(dp->ts, ts_buf);
	buf_pos = byte_copy((uint8_t *)ts_val, buf_pos, str_len(ts_val));

	buf_pos = byte_copy((uint8_t *)field_divider, buf_pos, field_divider_len);

	char *light_field = "\"light\": ";
	uint32_t light_field_len = str_len(light_field);
	buf_pos = byte_copy((uint8_t *)light_field, buf_pos, light_field_len);

	char light_buf[MAX_DIGITS] = {'\0'};
	char *light_val = int_to_string(dp->light_percent, light_buf);
	buf_pos = byte_copy((uint8_t *)light_val, buf_pos, str_len(light_val));

	buf_pos = byte_copy((uint8_t *)field_divider, buf_pos, field_divider_len);

	char *water_field = "\"water\": ";
	uint32_t water_field_len = str_len(water_field);
	buf_pos = byte_copy((uint8_t *)water_field, buf_pos, water_field_len);

	char water_buf[MAX_DIGITS] = {'\0'};
	char *water_val = int_to_string(dp->water_percent, water_buf);
	buf_pos = byte_copy((uint8_t *)water_val, buf_pos, str_len(water_val));

	buf_pos = byte_copy((uint8_t *)field_divider, buf_pos, field_divider_len);

	char *bat_field = "\"battery\": ";
	uint32_t bat_field_len = str_len(bat_field);
	buf_pos = byte_copy((uint8_t *)bat_field, buf_pos, bat_field_len);

	char battery_buf[MAX_DIGITS] = {'\0'};
	char *battery_val = int_to_string(dp->battery_percent, battery_buf);
	buf_pos = byte_copy((uint8_t *)battery_val, buf_pos, str_len(battery_val));

	char *json_end = "}";
	uint32_t json_end_len = str_len(json_end);
	byte_copy((uint8_t *)json_end, buf_pos, json_end_len);

	[[maybe_unused]] char *log_args[] = {(char *)buf};
	LOGF(DEBUG, "Created JSON string: &", NULL, 0, log_args, 1);

	return (char *)buf;
}

// converts the contense of the export queue, along with the current reading, to a json string
char *export_queue_to_json(exporter *e, data_point *current, uint8_t *buf)
{
	char *list_divider = ", ";
	uint32_t list_divider_len = str_len(list_divider);

	uint8_t *buf_pos = byte_copy((uint8_t *)"{\"version\": ", buf, 12);

	char *quote = "\"";
	uint16_t quote_len = str_len(quote);

	buf_pos = byte_copy((uint8_t *)quote, buf_pos, quote_len);

	uint16_t git_sha_len = str_len(GIT_SHA);

	buf_pos = byte_copy((uint8_t *)GIT_SHA, buf_pos, git_sha_len);

	buf_pos = byte_copy((uint8_t *)quote, buf_pos, quote_len);

	buf_pos = byte_copy((uint8_t *)list_divider, buf_pos, list_divider_len);

	buf_pos = byte_copy((uint8_t *)"\"current\": ", buf_pos, 11);

	uint8_t current_dp_buf[MAX_DATA_POINT_JSON_SIZE];
	mem_zero(current_dp_buf, MAX_DATA_POINT_JSON_SIZE);
	char *current_dp_json = data_point_to_json(current, current_dp_buf);

	buf_pos = byte_copy((uint8_t *)current_dp_json, buf_pos, str_len(current_dp_json));

	buf_pos = byte_copy((uint8_t *)list_divider, buf_pos, list_divider_len);

	buf_pos = byte_copy((uint8_t *)"\"graph\": [", buf_pos, 10);

	ring_buffer *rb = e->export_buf;
	for (int i = 0; i < rb->word_count; i++)
	{

		uint8_t dp_buf[MAX_DATA_POINT_JSON_SIZE];
		mem_zero(dp_buf, MAX_DATA_POINT_JSON_SIZE);

		char *dp_json = data_point_to_json((data_point *)ring_buffer_next_word(rb), dp_buf);
		buf_pos = byte_copy((uint8_t *)dp_json, buf_pos, str_len(dp_json));

		// dont add the separator to the list for the final element
		if (i == rb->word_count - 1)
		{
			break;
		}

		buf_pos = byte_copy((uint8_t *)list_divider, buf_pos, list_divider_len);
	}

	byte_copy((uint8_t *)"]}", buf_pos, 2);

	return (char *)buf;
}

int export_data(exporter *e, data_point *current)
{
	uint8_t buf[MAX_JSON_SIZE];
	char *json_output = export_queue_to_json(e, current, buf);

	[[maybe_unused]] char *log_args[] = {json_output};
	LOGF(INFO, "Exporting data: &", NULL, 0, log_args, 1);

	if (usart_send_buffer(USART1, (uint8_t *)json_output, str_len(json_output)) == -1)
	{
		LOG(ERROR, "Failed to export data over USART1");
		return -1;
	}

	return 0;
}

void run_exporter(exporter *e, uint8_t light_percent, uint8_t water_percent, uint8_t battery_percent)
{
	uint32_t ts = get_unix_time();
	data_point current_data = {.ts = ts, .light_percent = light_percent, .water_percent = water_percent, .battery_percent = battery_percent};

	if (e->send_data)
	{
		if (export_data(e, &current_data) == -1)
		{
			LOG(ERROR, "Failed to export data");
		}

		e->send_data = false;
	}

	if (ts < e->poll_interval || (ts - e->last_read_ts) < e->poll_interval)
	{
		return;
	}

	e->last_read_ts = ts;

	store_data_for_export(e, ts, light_percent, water_percent, battery_percent);
}