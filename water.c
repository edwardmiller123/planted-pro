#include <stdint.h>

#include "adc.h"
#include "logger.h"
#include "light.h"
#include "utils.h"
#include "sensor.h"

#define SATURATED_VAL 1000
#define MEDIUM_VAL 2500
#define DRY_VAL 3600

const char *soil_saturated = "Saturated";
const char *soil_medium = "Medium";
const char *soil_dry = "Dry";

// After 5 measurements have been taken we calculate the average water level and saturation percentage and write it
// back to the monitor struct.

int set_water_level(monitor *wm)
{
	if (sensor_calculate_average(wm->snr) == -1)
	{
		logger(ERROR, "Failed to get average water reading");
		return -1;
	}

	if (wm->snr->raw_average > DRY_VAL)
	{
		wm->level = (char *)soil_dry;
	}
	else if (wm->snr->raw_average > MEDIUM_VAL)
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
	if (sensor_process_samples(wm->snr, wm, &set_water_level) == -1)
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