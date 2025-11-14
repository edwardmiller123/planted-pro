#include <stdint.h>

#include "adc.h"
#include "logger.h"
#include "monitor.h"
#include "utils.h"
#include "sensor.h"
#include "heap.h"
#include "fpmath.h"

// values calibrated using a 200k resistor in the potential divider
#define LIGHT_BRIGHT_DIRECT_VAL 80
#define LIGHT_BRIGHT_INDIRECT_VAL 60
#define LIGHT_LOW_VAL 25

const char *light_bright_direct = "Bright Direct";
const char *light_bright_indirect = "Bright Indirect";
const char *light_medium = "Medium";
const char *light_low = "Low";

monitor *init_monitor(sensor *s, uint16_t min_adc_reading, uint16_t max_adc_reading)
{
	monitor *m = malloc(sizeof(monitor));
	if (m == NULL)
	{
		LOG(ERROR, "Failed to allocate memory for monitor");
		return NULL;
	}

	m->snr = s;
	m->level = NULL;
	m->percent = 0;
	m->min_adc_reading = min_adc_reading;
	m->max_adc_reading = max_adc_reading;
	return m;
}

int monitor_process_samples(monitor *m, int (*action)(struct monitor *))
{
	if (m->snr->readings_buffer->word_count == m->snr->sample_size)
	{
		if (action(m) == -1)
		{
			LOG(ERROR, "Failed to process adc samples");
			return -1;
		}
	}
	return 0;
}

uint8_t monitor_calculate_percent(monitor *m)
{
	uint32_t value_range = m->max_adc_reading - m->min_adc_reading;
	uint32_t adjusted_val = m->snr->raw_average - m->min_adc_reading;
	uint32_t percentage = fp_percentage(adjusted_val, value_range);
	return percentage;
}

int set_light_level(monitor *lm)
{
	if (sensor_calculate_average(lm->snr) == -1)
	{
		LOG(ERROR, "Failed to get average light reading");
		return -1;
	};

	[[maybe_unused]] char *args_level[] = {(char *)lm->level};
	LOGF(DEBUG, "Light level set to &", NULL, 0, args_level, 1);

	// we want the "light" percentage and darkness gives the max adc value
	lm->percent = 100 - monitor_calculate_percent(lm);

	if (lm->percent < LIGHT_LOW_VAL)
	{
		lm->level = (char *)light_low;
	}
	else if (lm->percent < LIGHT_BRIGHT_INDIRECT_VAL && lm->percent > LIGHT_LOW_VAL)
	{
		lm->level = (char *)light_medium;
	}
	else if (lm->percent > LIGHT_BRIGHT_INDIRECT_VAL && lm->percent < LIGHT_BRIGHT_DIRECT_VAL)
	{
		lm->level = (char *)light_bright_indirect;
	}
	else if (lm->percent > LIGHT_BRIGHT_DIRECT_VAL)
	{
		lm->level = (char *)light_bright_direct;
	}

	[[maybe_unused]] uint32_t args_intensity[] = {lm->percent};
	LOGF(DEBUG, "Light intensity set to $", args_intensity, 1, NULL, 0);

	return 0;
}

int measure_light(monitor *lm)
{
	if (monitor_process_samples(lm, &set_light_level) == -1)
	{
		LOG(ERROR, "Failed to calculate light level");
		return -1;
	}

	if (sensor_read_adc(lm->snr) == -1)
	{
		LOG(ERROR, "Failed to take light sensor reading");
		return -1;
	}

	return 0;
}

#define WATER_HIGH_VAL 80
#define WATER_DRY_VAL 20

const char *soil_saturated = "Saturated";
const char *soil_medium = "Medium";
const char *soil_dry = "Dry";

int set_water_level(monitor *wm)
{
	if (sensor_calculate_average(wm->snr) == -1)
	{
		LOG(ERROR, "Failed to get average water reading");
		return -1;
	}

	[[maybe_unused]] char *args_level[] = {(char *)wm->level};
	LOGF(DEBUG, "Water level set to &", NULL, 0, args_level, 1);

	// we want the water "percentage" and dry soil gives the max adc value
	wm->percent = 100 - monitor_calculate_percent(wm);

	if (wm->percent < WATER_DRY_VAL)
	{
		wm->level = (char *)soil_dry;
	}
	else if (wm->percent > WATER_DRY_VAL && wm->percent < WATER_HIGH_VAL)
	{
		wm->level = (char *)soil_medium;
	}
	else if (wm->percent > WATER_HIGH_VAL)
	{
		wm->level = (char *)soil_saturated;
	}

	[[maybe_unused]] uint32_t args_intensity[] = {wm->percent};
	LOGF(DEBUG, "Soil saturation set to $", args_intensity, 1, NULL, 0);

	return 0;
}

int measure_water(monitor *wm)
{
	if (monitor_process_samples(wm, &set_water_level) == -1)
	{
		LOG(ERROR, "Failed to calculate water level");
		return -1;
	}

	if (sensor_read_adc(wm->snr) == -1)
	{
		LOG(ERROR, "Failed to take water sensor reading");
		return -1;
	}

	return 0;
}

const char *battery_low = "Low Battery";

// The percent of the maximum measured battery voltage that Vref is equivalent to
#define VREF_MAX_BAT_PERCENT 58

// The percent of the minimum measured battery voltage that Vref is equivalent to
#define VREF_MIN_BAT_PERCENT 72

// Digital value of Vref which is always equivalent to ~1.21V.
// This value will always be 58% of the max battery charge (2.1V from divider)
// This value will be 72% of the min battery value (1.67V from divider)
static uint16_t v_ref = 0;

int set_battery_charge(monitor *bm)
{
	if (sensor_calculate_average(bm->snr) == -1)
	{
		LOG(ERROR, "Failed to get average battery charge reading");
		return -1;
	}

	// calculate the new max adc value based off Vref
	bm->max_adc_reading = fp_divide(v_ref * 100, VREF_MAX_BAT_PERCENT);

	[[maybe_unused]] uint32_t args_max_adc_reading[] = {bm->max_adc_reading};
	LOGF(DEBUG, "Calibrated max charge ADC value to $", args_max_adc_reading, 1, NULL, 0);

	// calculate the new min adc value based off Vref
	bm->min_adc_reading = fp_divide(v_ref * 100, VREF_MIN_BAT_PERCENT);

	[[maybe_unused]] uint32_t args_min_adc_reading[] = {bm->min_adc_reading};
	LOGF(DEBUG, "Calibrated min charge ADC value to $", args_min_adc_reading, 1, NULL, 0);

	// calculate the charge percent uisng the scaled values
	bm->percent = monitor_calculate_percent(bm);

	bm->level = NULL;

	if (bm->percent < BAT_LOW_PERCENT) {
		bm->level = battery_low;

		[[maybe_unused]] char *args_level[] = {(char *)bm->level};
		LOGF(DEBUG, "Battery level set to &", NULL, 0, args_level, 1);
	}

	[[maybe_unused]] uint32_t args_intensity[] = {bm->percent};
	LOGF(DEBUG, "Charge percent set to $", args_intensity, 1, NULL, 0);

	return 0;

}

int measure_battery_charge(monitor *bm)
{
	result_code result;
	v_ref = adc_read_vrefint(&result);
	if (result == FAILURE) {
		LOG(ERROR, "Failed to read internal reference voltage");
		return -1;
	}

	[[maybe_unused]] uint32_t args_vref[] = {v_ref};
	LOGF(DEBUG, "Vref ADC value measured as $", args_vref, 1, NULL, 0);

	if (monitor_process_samples(bm, &set_battery_charge) == -1)
	{
		LOG(ERROR, "Failed to calculate battery charge");
		return -1;
	}

	if (sensor_read_adc(bm->snr) == -1)
	{
		LOG(ERROR, "Failed to take battery sensor reading");
		return -1;
	}

	return 0;
}
