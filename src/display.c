#include <stdint.h>
#include <stdbool.h>

#include "display.h"
#include "logger.h"
#include "lcd.h"
#include "utils.h"
#include "adc.h"
#include "time.h"
#include "sensor.h"
#include "monitor.h"
#include "gpio.h"

#define DISPLAY_SWITCH_TIME 3000

typedef enum info_type
{
	LIGHT,
	MOISTURE,
	BATTERY
} info_type;

// use a set order for displaying info
#define DISPLAY_SEQ_LEN 5

const info_type display_order[DISPLAY_SEQ_LEN] = {LIGHT, MOISTURE, LIGHT, MOISTURE, BATTERY};
static uint8_t currently_showing_idx = 0;

static uint32_t display_last_changed = 0;

void display_percent(monitor *m, uint8_t row)
{
	lcd_set_cursor(0, row);

	char line_buf[LCD_LINE_LENGTH];
	char *line = line_buf;

	if (m->percent > 100)
	{
		line = "Percent: ERROR";
	}
	else
	{
		char percentage_str[3];
		char percentage_pretty[4];
		str_cat(int_to_string(m->percent, percentage_str), "%", percentage_pretty);
		str_cat("Percent: ", percentage_pretty, line);
	}

	lcd_write_string(line);
}

void display_light_info(monitor *m)
{
	logger(INFO, "Displaying light info");

	// display the percent first so its not held up from being visible by the screen scrolling
	// which blocks
	display_percent(m, 1);

	lcd_set_cursor(0, 0);

	char line1_buf[LCD_MAX_MSG_LEN];
	char *line1 = line1_buf;

	if (m->level == NULL)
	{
		line1 = "Light: ERROR";
	}
	else
	{
		str_cat("Light: ", (char *)m->level, line1);
	}

	lcd_write_string_and_scroll(line1, 0, 0);
}

void display_moisture_info(monitor *m)
{
	logger(INFO, "Displaying moisture info");

	display_percent(m, 1);

	lcd_set_cursor(0, 0);

	char line_buf[LCD_MAX_MSG_LEN];
	char *line = line_buf;

	if (m->level == NULL)
	{
		lcd_write_string("Soil: ERROR");
	}
	else
	{
		str_cat("Soil: ", (char *)m->level, line);
	}

	lcd_write_string_and_scroll(line, 0, 0);
}

void display_battery_info(monitor *m)
{
	logger(INFO, "Displaying battery info");

	if (m->level != NULL)
	{
		lcd_set_cursor(0, 1);
		lcd_write_string((char *)m->level);
		return;
	}

	lcd_set_cursor(0, 0);

	char line1_buf[LCD_MAX_MSG_LEN];
	char *line1 = line1_buf;

	if (m->percent > 100)
	{
		line1 = "Charge: ERROR";
	}
	else
	{
		char percentage_str[3];
		char percentage_pretty[4];
		str_cat(int_to_string(m->percent, percentage_str), "%", percentage_pretty);
		str_cat("Charge: ", percentage_pretty, line1);
	}

	lcd_write_string_and_scroll(line1, 0, 0);
}

void display_info(monitor * light_mon, monitor * water_mon, monitor * bat_mon)
{
	uint32_t now = get_system_counter();

	if (now < (display_last_changed + DISPLAY_SWITCH_TIME))
	{
		return;
	}

	#ifdef MODE
		toggle_user_led();
	#endif

	if (currently_showing_idx > DISPLAY_SEQ_LEN) {
		currently_showing_idx = 0;
	}

	display_last_changed = now;

	lcd_clear_display();

	switch (display_order[currently_showing_idx])
	{
	case LIGHT:
		display_light_info(light_mon);
		break;
	case MOISTURE:
		display_moisture_info(water_mon);
		break;
	case BATTERY:
		display_battery_info(bat_mon);
		break;
	}

	currently_showing_idx += 1;
}