#include <stdint.h>
#include <stdbool.h>

#include "plantmonitor.h"
#include "logger.h"
#include "time.h"
#include "lcd.h"
#include "utils.h"
#include "usart.h"
#include "adc.h"
#include "sensor.h"
#include "monitor.h"
#include "export.h"
#include "gpio.h"
#include "heap.h"
#include "bluetooth.h"
#include "display.h"

#define LIGHT_MAX_VALUE 4095

#define WATER_MAX_VALUE 2850
#define WATER_MIN_VALUE 1200

// The battery has a max charge of 4.2v and can be considered "dead" by the time it
// reaches 3.3v (any lower and we wont get accurate readings so consider 3.3 to be 0 percent battery).
// The sensor gives half the battery voltage so we consider 2.1v full and 1.65 "empty".
// Since the max ADC value 4095 corresponds to the reference voltage 3.3v we get ADC values of,
// Full: 2620, empty: 2047
#define BAT_CHARGE_MIN_VALUE 2047
#define BAT_CHARGE_MAX_VALUE 2620

#define SAMPLE_SIZE 5

// Use a large sample size for the battery as the signal is expected to be noisey
#define BAT_CHARGE_SAMPLE_SIZE 50

#define EXPORT_POLL_INTERVAL_SECONDS 1
#define EXPORT_POINT_COUNT 12

plant_monitor *init_plant_monitor()
{
	plant_monitor *pm = malloc(sizeof(plant_monitor));
	if (pm == NULL)
	{
		logger(ERROR, "Failed to allocate memory for plant monitor");
		return NULL;
	}

	// After 5 measurements have been taken we calculate the average light level and "percentage" and write it
	// back to the state struct. The light monitor uses ADC1 to read from an ldr in a potential divider
	sensor *light_sen = init_sensor(0, LIGHT_MAX_VALUE, SAMPLE_SIZE, ADC1);
	if (light_sen == NULL)
	{
		logger(ERROR, "Failed to allocate memory for light sensor");
		return NULL;
	}

	logger(INFO, "Light sensor initialised on ADC1");

	// The water monitor uses a capacitive soil moisture sensor
	sensor *water_sen = init_sensor(WATER_MIN_VALUE, WATER_MAX_VALUE, SAMPLE_SIZE, ADC2);
	if (water_sen == NULL)
	{
		logger(ERROR, "Failed to allocate memory for water sensor");
		return NULL;
	}

	logger(INFO, "Water sensor initialised on ADC2");

	// The battery sensor measures the current battery voltage halfed so that it always remains under the reference voltage
	sensor *battery_sen = init_sensor(BAT_CHARGE_MIN_VALUE, BAT_CHARGE_MAX_VALUE, BAT_CHARGE_SAMPLE_SIZE, ADC3);
	if (water_sen == NULL)
	{
		logger(ERROR, "Failed to allocate memory for battery sensor");
		return NULL;
	}

	logger(INFO, "Battery sensor initialised on ADC3");

	monitor *lm = init_monitor(light_sen);
	if (lm == NULL)
	{
		logger(ERROR, "Failed to initialise light monitor");
		return NULL;
	}

	monitor *wm = init_monitor(water_sen);
	if (wm == NULL)
	{
		logger(ERROR, "Failed to initialise water monitor");
		return NULL;
	}

	monitor *bm = init_monitor(battery_sen);
	if (wm == NULL)
	{
		logger(ERROR, "Failed to initialise battery monitor");
		return NULL;
	}

	pm->lm = lm;
	pm->wm = wm;
	pm->bm = bm;

	// TODO: change this to poll every 15 minutes for 12 hours after we are done testing
	exporter *e = init_exporter(EXPORT_POLL_INTERVAL_SECONDS, EXPORT_POINT_COUNT);
	if (e == NULL)
	{
		logger(ERROR, "Failed to initialise exporter");
		return NULL;
	}

	pm->e = e;

	return pm;
}

void poll_sensors(plant_monitor *pm)
{
	if (measure_light(pm->lm) == -1)
	{
		logger(ERROR, "Failed to measure light level");
	}

	if (measure_water(pm->wm) == -1)
	{
		logger(ERROR, "Failed to measure water level");
	}

	if (measure_battery_charge(pm->bm) == -1)
	{
		logger(ERROR, "Failed to measure battery charge");
	}
}

void run_monitor(plant_monitor *pm)
{
	display_info(pm->lm, pm->wm, pm->bm);

	poll_sensors(pm);

	poll_bluetooth(pm->e);

	run_exporter(pm->e, pm->lm->percent, pm->wm->percent, pm->bm->percent);

}