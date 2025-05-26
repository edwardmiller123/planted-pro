#include <stdint.h>

#include "adc.h"
#include "logger.h"
#include "monitor.h"
#include "utils.h"
#include "sensor.h"

// values calibrated using an 82k resistor in the potential divider
#define LIGHT_BRIGHT_DIRECT_VAL 1000
#define LIGHT_BRIGHT_INDIRECT_VAL 1500
#define LIGHT_MEDIUM_VAL 2000
#define LIGHT_LOW_VAL 3600

const char *light_bright_direct = "Bright Direct";
const char *light_bright_indirect = "Bright Indirect";
const char *light_medium = "Medium";
const char *light_low = "Low";

void init_monitor(monitor *m, sensor * s)
{
	m->snr = s;
	m->level = NULL;
	m->percent = UNDEFINED_PERCENTAGE;
}

int monitor_process_samples(monitor *m, int (*action)(struct monitor *))
{
	if (m->snr->readings_queue->size == m->snr->sample_size)
	{
		if (action(m) == -1)
		{
			logger(ERROR, "Failed to process adc samples");
			return -1;
		}

		reset_queue(m->snr->readings_queue);
	}
	return 0;
}

int set_light_level(monitor *lm)
{
	if (sensor_calculate_average(lm->snr) == -1)
	{
		logger(ERROR, "Failed to get average light reading");
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

	char *args_level[] = {lm->level};
	loggerf(INFO, "Light level set to &", NULL, 0, args_level, 1);

	// we want the "light" percentage and darkness gives the max adc value
	lm->percent = 100 - lm->snr->sensor_percent;

	uint32_t args_intensity[] = {lm->percent};
	loggerf(INFO, "Light intensity set to $", args_intensity, 1, NULL, 0);

	return 0;
}

int measure_light(monitor *lm)
{
	if (monitor_process_samples(lm, &set_light_level) == -1)
	{
		logger(ERROR, "Failed to calculate light level");
		return -1;
	}

	if (sensor_read_adc(lm->snr) == -1)
	{
		logger(ERROR, "Failed to take light sensor reading");
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
		logger(ERROR, "Failed to get average water reading");
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

	char *args_level[] = {wm->level};
	loggerf(INFO, "Water level set to &", NULL, 0, args_level, 1);

	// we want the water "percentage" and dry soil gives the max adc value
	wm->percent = 100 - wm->snr->sensor_percent;

	uint32_t args_intensity[] = {wm->percent};
	loggerf(INFO, "Soil saturation set to $", args_intensity, 1, NULL, 0);

	return 0;
}

int measure_water(monitor *wm)
{
	if (monitor_process_samples(wm, &set_water_level) == -1)
	{
		logger(ERROR, "Failed to calculate water level");
		return -1;
	}

	if (sensor_read_adc(wm->snr) == -1)
	{
		logger(ERROR, "Failed to take water sensor reading");
		return -1;
	}

	return 0;
}
