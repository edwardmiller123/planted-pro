
#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "fpmath.h"
#include "logger.h"
#include "adc.h"
#include "sensor.h"
#include "heap.h"


sensor *init_sensor(uint32_t sample_size, adc adc_num)
{
	sensor *s = malloc(sizeof(sensor));
	if (s == NULL)
	{
		LOG(ERROR, "Failed to allocate memory for sensor");
		return NULL;
	}

	ring_buffer *readings = init_ring_buffer();
	if (readings == NULL)
	{
		LOG(ERROR, "Failed to initialise sensor readings buffer");
		return NULL;
	}

	s->readings_buffer = readings;

	s->sensor_percent = 0;
	s->sample_size = sample_size;
	s->adc_num = adc_num;
	return s;
}

int sensor_calculate_average(sensor *s)
{
	uint32_t total_val = 0;
	int32_t reading;
	for (int i = 0; i < s->sample_size; i++)
	{
		result_code read_result;
		reading = ring_buffer_read_word(s->readings_buffer, &read_result);
		if (read_result == EMPTY)
		{
			LOG(WARNING, "Sensor tried to read from empty readings buffer");
			return 0;
		}

		[[maybe_unused]] uint32_t args[] = {reading};
		LOGF(DEBUG, "Removed $ from sensor readings queue", args, 1, NULL, 0);

		total_val += reading;
	}

	uint32_t average = fp_divide(total_val, s->sample_size);
	s->raw_average = average;

	[[maybe_unused]] uint32_t args_average[] = {average};
	LOGF(DEBUG, "Set average sensor ADC value to $", args_average, 1, NULL, 0);

	return 0;
}

int sensor_read_adc(sensor *s)
{
	result_code adc_result;
	uint32_t reading = adc_manual_conversion(s->adc_num, &adc_result);
	if (adc_result == FAILURE)
	{
		LOG(ERROR, "Failed to read adc conversion");
		return -1;
	}

	ring_buffer_write_word(s->readings_buffer, reading);
	
	[[maybe_unused]] uint32_t args[] = {reading};
	LOGF(DEBUG, "Added $ to sensor readings buffer", args, 1, NULL, 0);
	return 0;
}