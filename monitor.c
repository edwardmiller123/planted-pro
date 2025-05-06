#include <stdint.h>
#include <stdbool.h>

#include "logger.h"
#include "systick.h"
#include "lcd.h"
#include "utils.h"
#include "gpio.h"
#include "monitor.h"

#define DISPLAY_SWITCH_TIME 3000

void init_monitor(monitor *m, light_monitor *lm, queue *light_readings)
{
	init_light_monitor(lm, light_readings);

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
	// TODO: measure soil moisture
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

	if (lm->percentage == UNDEFINED_PERCENTAGE)
	{
		line2 = "Percent: Initialising...";
	}
	else
	{
		char percentage[3];
		char percentage_hr[4];
		str_cat(int_to_string(lm->percentage, percentage), "%", percentage_hr);
		str_cat("Percent: ", percentage_hr, line2);
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