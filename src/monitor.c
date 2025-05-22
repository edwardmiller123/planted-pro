#include <stdint.h>
#include <stdbool.h>

#include "logger.h"
#include "systick.h"
#include "lcd.h"
#include "utils.h"
#include "gpio.h"
#include "adc.h"
#include "sensor.h"
#include "light.h"
#include "water.h"
#include "monitor.h"

#define DISPLAY_SWITCH_TIME 3000

// max adc output from ~5v supply
#define ADC_MAX_OUTPUT 4095

#define SAMPLE_SIZE 5

void init_plant_monitor(plant_monitor *pm, monitor *lm, monitor *wm, sensor *light_sensor, sensor *water_sensor, queue *light_readings, queue *water_readings)
{
	// After 5 measurements have been taken we calculate the average light level and "percentage" and write it
	// back to the state struct. The light monitor uses ADC1 to read from a ldr in a potential divider with an 82k resistor
	init_sensor(light_sensor, light_readings, ADC_MAX_OUTPUT, SAMPLE_SIZE, ADC1);
	logger(INFO, "Light sesnor initialised with ADC1");

	init_sensor(water_sensor, water_readings, ADC_MAX_OUTPUT, SAMPLE_SIZE, ADC2);
	logger(INFO, "Water sesnor initialised with ADC2");

	init_monitor(lm, light_sensor);
	init_monitor(wm, water_sensor);

	pm->currently_showing = LIGHT;
	pm->sys_uptime = 0;
	pm->lm = lm;
	pm->wm = wm;
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

	// display the percent first so its noit held up from beiung visible by the screen scrolling
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
	uint32_t now = get_system_uptime();
	if (now > (pm->sys_uptime + DISPLAY_SWITCH_TIME))
	{
		pm->currently_showing = (info_type) !(bool)pm->currently_showing;
		pm->sys_uptime = now;
		display_info(pm);
		toggle_user_led();
	}

	poll_sensors(pm);
}