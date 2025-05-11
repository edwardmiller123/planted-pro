#include <stdint.h>
#include <stdbool.h>

#include "logger.h"
#include "systick.h"
#include "lcd.h"
#include "utils.h"
#include "gpio.h"
#include "adc.h"
#include "sensor.h"
#include "monitor.h"

#define DISPLAY_SWITCH_TIME 3000

// max adc output using a 82k resistor in the potential divider
#define ADC_LIGHT_MAX_OUTPUT 4095

#define SAMPLE_SIZE 5

void init_monitor(monitor *m, light_monitor *lm, sensor * light_sensor, queue *light_readings)
{
	init_sensor(light_sensor, light_readings, ADC_LIGHT_MAX_OUTPUT, SAMPLE_SIZE);
	init_light_monitor(lm, light_sensor);

	m->currently_showing = LIGHT;
	m->sys_uptime = 0;
	m->lm = lm;
}

void poll_sensors(monitor *m)
{
	if (measure_light(m->lm) == -1)
	{
		logger(ERROR, "Failed to measure light level");
	}
	adc_manual_conversion(ADC2);
}

void display_light_info(light_monitor *lm)
{
	lcd_set_cursor(0, 0);
	char line1_buf[LCD_LINE_LENGTH];
	char *line1 = line1_buf;

	if (lm->level == NULL)
	{
		line1 = "Sun: Initialising...";
	}
	else
	{
		str_cat("Sun: ", (char *)lm->level, line1);
	}

	lcd_write_string(line1);

	lcd_set_cursor(0, 1);

	char line2_buf[LCD_LINE_LENGTH];
	char *line2 = line2_buf;

	if (lm->intensity_percent == UNDEFINED_PERCENTAGE)
	{
		line2 = "Intensity: Initialising...";
	}
	else
	{
		uint32_t intensity_percent = lm->intensity_percent;
		if (intensity_percent == 0)
		{
			// show 1 percent rather than 0 since its very rare that there is 0 light available
			intensity_percent = 1;
		}

		char percentage_str[3];
		char percentage_pretty[4];
		str_cat(int_to_string(intensity_percent, percentage_str), "%", percentage_pretty);
		str_cat("Intensity: ", percentage_pretty, line2);
	}

	lcd_write_string(line2);
}

void display_moisture_info()
{
	lcd_set_cursor(0, 0);
	lcd_write_string("Soil: initialising...");
}

void display_info(monitor *m)
{
	lcd_clear_display();

	switch (m->currently_showing)
	{
	case LIGHT:
		display_light_info(m->lm);
		break;
	case MOISTURE:
		display_moisture_info();
		break;
	}
}

void run_monitor(monitor *m)
{
	uint32_t now = get_system_uptime();
	if (now > (m->sys_uptime + DISPLAY_SWITCH_TIME))
	{
		m->currently_showing = (info_type) !(bool)m->currently_showing;
		m->sys_uptime = now;
		display_info(m);
		toggle_user_led();
	}

	poll_sensors(m);
}