#include <stdint.h>

#include "export.h"
#include "queue.h"
#include "systick.h"
#include "heap.h"
#include "utils.h"
#include "logger.h"
#include "usart.h"

// slightly bigger than the number of values we want to hold in the queue at any one time so we
// dont have to reset the queue too frequently
#define MAX_EXPORT_QUEUE_CAPACITY 128
#define MAX_JSON_SIZE 1024
#define MAX_DIGITS 10
#define MAX_DATA_POINT_JSON_SIZE 64
#define SEND_CODE '6'

exporter *init_exporter(uint16_t poll_interval, uint16_t data_point_count)
{
	exporter *e = malloc(sizeof(exporter));
	if (e == NULL)
	{
		logger(ERROR, "Failed to malloc exporter");
		return NULL;
	}

	queue *export_queue = init_queue(MAX_EXPORT_QUEUE_CAPACITY);
	if (export_queue == NULL)
	{
		logger(ERROR, "Failed to initialise export queue");
		return NULL;
	}

	e->export_queue = export_queue;
	e->data_point_limit = data_point_count;
	e->poll_interval = poll_interval;
	e->last_read_ts = 0;

	logger(INFO, "Initialised Exporter");

	return e;
}

// TODO: timestamps are in the wrong order soimetimes and there is some corruption
int store_data_for_export(exporter *e, uint32_t ts, uint8_t light_percent, uint8_t water_percent)
{
	// remove the oldest data point if the queue is full
	if (e->export_queue->size == e->data_point_limit)
	{
		uint32_t oldest_data = fifo_get(e->export_queue);
		if (oldest_data == -1)
		{
			logger(ERROR, "Failed to remove data from export queue");
			return -1;
		}

		uint32_t log_args[] = {((data_point *)oldest_data)->ts, ((data_point *)oldest_data)->light_percent, ((data_point *)oldest_data)->water_percent};
		loggerf(INFO, "Discarded values from export list. TS: $, Light: $, Water: $", log_args, 3, NULL, 0);

		free((void *)oldest_data);
	}

	data_point *dp = malloc(sizeof(data_point));
	if (dp == NULL)
	{
		logger(ERROR, "Failed to malloc data point");
		return -1;
	}

	dp->ts = ts;
	dp->light_percent = light_percent;
	dp->water_percent = water_percent;

	if (fifo_add(e->export_queue, (uint32_t)dp) == QUEUE_FULL)
	{
		logger(ERROR, "Failed to add data point to export queue");
		return -1;
	};

	uint32_t log_args[] = {ts, light_percent, water_percent};
	loggerf(INFO, "Stored values for export. TS: $, Light: $, Water: $", log_args, 3, NULL, 0);

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

	char *json_end = "}";
	uint32_t json_end_len = str_len(json_end);
	byte_copy((uint8_t *)json_end, buf_pos, json_end_len);

	char *log_args[] = {(char *)buf};
	loggerf(DEBUG, "Created JSON string: &", NULL, 0, log_args, 1);

	return (char *)buf;
}

// converts the contense of the export queue to a json list string
char *export_queue_to_json(exporter *e, uint8_t *buf)
{
	char *list_divider = ", ";
	uint32_t list_divider_len = str_len(list_divider);

	uint8_t *buf_pos = byte_copy((uint8_t *)"[", buf, 1);

	queue *q = e->export_queue;
	// The queue is very janky and we should really switch back to using statically allocated memory
	for (int i = 0; i <= q->size; i++)
	{

		uint8_t dp_buf[MAX_DATA_POINT_JSON_SIZE];
		char *dp_json = data_point_to_json((data_point *)queue_next(q), dp_buf);
		buf_pos = byte_copy((uint8_t *)dp_json, buf_pos, str_len(dp_json));

		// dont add the separator to the list for the final element
		if (i == q->size)
		{
			break;
		}

		buf_pos = byte_copy((uint8_t *)list_divider, buf_pos, list_divider_len);
	}

	byte_copy((uint8_t *)"]", buf_pos, 1);

	return (char *)buf;
}

int export_data(exporter *e)
{
	uint8_t buf[MAX_JSON_SIZE];
	char *json_output = export_queue_to_json(e, buf);

	char *log_args[] = {json_output};
	loggerf(INFO, "Exporting data: &", NULL, 0, log_args, 1);

	if (usart_send_buffer(USART1, (uint8_t *)json_output, str_len(json_output)) == -1)
	{
		logger(ERROR, "Failed to export data over USART1");
		return -1;
	}

	return 0;
}

void run_exporter(exporter *e, uint8_t light_percent, uint8_t water_percent)
{
	uint8_t usart1_received_byte = usart1_read_byte();

	uint32_t log_args[] = {usart1_received_byte};
	loggerf(DEBUG, "Read byte $ from USART1 receive buffer", log_args, 1, NULL, 0);

	if (usart1_received_byte == (uint8_t)SEND_CODE)
	{
		if (export_data(e) == -1)
		{
			logger(ERROR, "Failed to export data");
		}
	}

	uint32_t ts = get_system_uptime();
	if (ts < e->poll_interval || (ts - e->last_read_ts) < e->poll_interval)
	{
		return;
	}

	e->last_read_ts = ts;

	store_data_for_export(e, ts, light_percent, water_percent);
}