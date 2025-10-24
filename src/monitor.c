#include <stdint.h>

#include "adc.h"
#include "logger.h"
#include "monitor.h"
#include "utils.h"
#include "sensor.h"
#include "heap.h"

// values calibrated using an 82k resistor in the potential divider
#define LIGHT_BRIGHT_DIRECT_VAL 1000
#define LIGHT_BRIGHT_INDIRECT_VAL 1500
#define LIGHT_MEDIUM_VAL 2000
#define LIGHT_LOW_VAL 3600

const char *light_bright_direct = "Bright Direct";
const char *light_bright_indirect = "Bright Indirect";
const char *light_medium = "Medium";
const char *light_low = "Low";

monitor *init_monitor(sensor *s)
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

int set_light_level(monitor *lm)
{
	if (sensor_calculate_average(lm->snr) == -1)
	{
		LOG(ERROR, "Failed to get average light reading");
		return -1;
	};

	if (lm->snr->raw_average > LIGHT_LOW_VAL)
	{
		lm->level = (char *)light_low;
	}
	else if (lm->snr->raw_average > LIGHT_MEDIUM_VAL)
	{
		lm->level = (char *)light_medium;
	}
	else if (lm->snr->raw_average > LIGHT_BRIGHT_INDIRECT_VAL)
	{
		lm->level = (char *)light_bright_indirect;
	}
	else
	{
		lm->level = (char *)light_bright_direct;
	}

	[[maybe_unused]] char *args_level[] = {(char *)lm->level};
	LOGF(DEBUG, "Light level set to &", NULL, 0, args_level, 1);

	// we want the "light" percentage and darkness gives the max adc value
	lm->percent = 100 - lm->snr->sensor_percent;

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

#define WATER_HIGH_VAL 1400
#define WATER_MEDIUM_VAL 2000
#define WATER_DRY_VAL 2400

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

	if (wm->snr->raw_average > WATER_DRY_VAL)
	{
		wm->level = (char *)soil_dry;
	}
	else if (wm->snr->raw_average > WATER_MEDIUM_VAL)
	{
		wm->level = (char *)soil_medium;
	}
	else
	{
		wm->level = (char *)soil_saturated;
	}

	[[maybe_unused]] char *args_level[] = {(char *)wm->level};
	LOGF(DEBUG, "Water level set to &", NULL, 0, args_level, 1);

	// we want the water "percentage" and dry soil gives the max adc value
	wm->percent = 100 - wm->snr->sensor_percent;

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

int set_battery_charge(monitor *bm)
{
	if (sensor_calculate_average(bm->snr) == -1)
	{
		LOG(ERROR, "Failed to get average battery charge reading");
		return -1;
	}

	bm->level = NULL;

	if (bm->snr->sensor_percent < BAT_LOW_PERCENT) {
		bm->level = battery_low;

		[[maybe_unused]] char *args_level[] = {(char *)bm->level};
		LOGF(DEBUG, "Battery level set to &", NULL, 0, args_level, 1);
	}

	bm->percent = bm->snr->sensor_percent;

	[[maybe_unused]] uint32_t args_intensity[] = {bm->percent};
	LOGF(DEBUG, "Charge percent set to $", args_intensity, 1, NULL, 0);

	return 0;

}

int measure_battery_charge(monitor *bm)
{
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
