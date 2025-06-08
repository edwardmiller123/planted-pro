#include <stdint.h>
#include <stdbool.h>

#include "plantmonitor.h"
#include "logger.h"
#include "systick.h"
#include "lcd.h"
#include "utils.h"
#include "usart.h"
#include "adc.h"
#include "sensor.h"
#include "monitor.h"
#include "export.h"
#include "heap.h"

#define DISPLAY_SWITCH_TIME 3000

#define LIGHT_MAX_VALUE 4095

#define WATER_MAX_VALUE 2760
#define WATER_MIN_VALUE 1200

#define SAMPLE_SIZE 5

#define EXPORT_POLL_INTERVAL 500
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
	// back to the state struct. The light monitor uses ADC1 to read from a ldr in a potential divider with an 82k resistor
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

	pm->currently_showing = LIGHT;
	pm->display_change_interval = 0;
	pm->lm = lm;
	pm->wm = wm;

	// TODO: change this to poll every 15 minutes for 12 hours after we are done testing
	exporter *e = init_exporter(EXPORT_POLL_INTERVAL, EXPORT_POINT_COUNT);
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
}

void display_percent(monitor *m)
{
	lcd_set_cursor(0, 1);

	char line2_buf[LCD_LINE_LENGTH];
	char *line2 = line2_buf;

	if (m->percent == UNDEFINED_PERCENTAGE)
	{
		line2 = "Percent: reading...";
	}
	else
	{
		uint32_t displayed_percent = m->percent;
		if (displayed_percent == 0)
		{
			// show 1 percent rather than 0 since its very rare that there is 0 percent of the read attribute available
			displayed_percent = 1;
		}

		char percentage_str[3];
		char percentage_pretty[4];
		str_cat(int_to_string(displayed_percent, percentage_str), "%", percentage_pretty);
		str_cat("Percent: ", percentage_pretty, line2);
	}

	lcd_write_string(line2);
}

void display_light_info(monitor *m)
{
	lcd_set_cursor(0, 0);

	// display the percent first so its not held up from being visible by the screen scrolling
	// which blocks
	display_percent(m);

	char line1_buf[LCD_MAX_MSG_LEN];
	char *line1 = line1_buf;

	if (m->level == NULL)
	{
		line1 = "Light: reading...";
	}
	else
	{
		str_cat("Light: ", (char *)m->level, line1);
	}

	lcd_write_string_and_scroll(line1, 0, 0);
}

void display_moisture_info(monitor *m)
{
	lcd_set_cursor(0, 0);

	display_percent(m);

	char line1_buf[LCD_MAX_MSG_LEN];
	char *line1 = line1_buf;

	if (m->level == NULL)
	{
		lcd_write_string("Soil: reading...");
	}
	else
	{
		str_cat("Soil: ", (char *)m->level, line1);
	}

	lcd_write_string_and_scroll(line1, 0, 0);
}

void display_info(plant_monitor *pm)
{
	uint32_t now = get_system_uptime();
	if (now < (pm->display_change_interval + DISPLAY_SWITCH_TIME))
	{
		return;
	}

	pm->currently_showing = (info_type) !(bool)pm->currently_showing;
	pm->display_change_interval = now;

	lcd_clear_display();

	switch (pm->currently_showing)
	{
	case LIGHT:
		display_light_info(pm->lm);
		break;
	case MOISTURE:
		display_moisture_info(pm->wm);
		break;
	}
}

void run_monitor(plant_monitor *pm)
{
	display_info(pm);

	poll_sensors(pm);

	if (store_data_for_export(pm->e, pm->lm->percent, pm->wm->percent)) {
		logger(WARNING, "Failed to store data for export");
	}
}