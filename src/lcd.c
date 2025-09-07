#include <stdint.h>

#include "io.h"
#include "gpio.h"
#include "utils.h"
#include "time.h"
#include "logger.h"
#include "lcd.h"

#define DISPLAY_LENGTH 32

// LCD pin to GPIO Mapping (data lines connect to the first 4 pins on each side at the top of the board)
// PC9 -> D0
// PC8 -> D1
// PB8 -> D2
// PC6 -> D3
// PC10 -> D4
// PC11 -> D5
// PC12 -> D6
// PD2 -> D7
// PB13 -> RS
// PB14 -> RW
// PB15 -> E

// Data lines
#define D0 PC9
#define D1 PC8
#define D2 PB8
#define D3 PC6
#define D4 PC10
#define D5 PC11
#define D6 PC12
#define D7 PD2

// Identify output as data or instruction
#define RS PB13

// Set read or write to lcd module
#define RW PB14

// Enable LCD
#define E PB15

typedef enum output_type
{
	INSTRUCTION,
	DATA
} output_type;

void lcd_write_byte(uint8_t data, output_type ot)
{
	gpio_write_pin_atomic(GPIOB, RS, (gpio_action)ot);

	gpio_write_pin_atomic(GPIOC, D0, (gpio_action)get_bit(data, 0));
	gpio_write_pin_atomic(GPIOC, D1, (gpio_action)get_bit(data, 1));
	gpio_write_pin_atomic(GPIOB, D2, (gpio_action)get_bit(data, 2));
	gpio_write_pin_atomic(GPIOC, D3, (gpio_action)get_bit(data, 3));
	gpio_write_pin_atomic(GPIOC, D4, (gpio_action)get_bit(data, 4));
	gpio_write_pin_atomic(GPIOC, D5, (gpio_action)get_bit(data, 5));
	gpio_write_pin_atomic(GPIOC, D6, (gpio_action)get_bit(data, 6));
	gpio_write_pin_atomic(GPIOD, D7, (gpio_action)get_bit(data, 7));

	// toggle enable bit to send data
	gpio_write_pin_atomic(GPIOB, E, SET);
	sys_sleep(1);
	gpio_write_pin_atomic(GPIOB, E, CLEAR);
}

void configure_lcd(void)
{
	// set GPIO pins mode to output and pull down

	// PC9 -> D0
	io_set_bit(GPIOC_MODER, 18);
	io_clear_bit(GPIOC_MODER, 19);

	// set high speed
	io_set_bit(GPIOC_OSPEEDR, 18);
	io_set_bit(GPIOC_OSPEEDR, 19);

	// PC8 -> D1
	io_set_bit(GPIOC_MODER, 16);
	io_clear_bit(GPIOC_MODER, 17);

	io_set_bit(GPIOC_OSPEEDR, 16);
	io_set_bit(GPIOC_OSPEEDR, 17);

	// PB8 -> D2
	io_set_bit(GPIOB_MODER, 16);
	io_clear_bit(GPIOB_MODER, 17);

	io_set_bit(GPIOB_OSPEEDR, 16);
	io_set_bit(GPIOB_OSPEEDR, 17);

	// PC6 -> D3
	io_set_bit(GPIOC_MODER, 12);
	io_clear_bit(GPIOC_MODER, 13);

	io_set_bit(GPIOC_OSPEEDR, 12);
	io_set_bit(GPIOC_OSPEEDR, 13);

	// PC10 -> D4
	io_set_bit(GPIOC_MODER, 20);
	io_clear_bit(GPIOC_MODER, 21);

	io_set_bit(GPIOC_OSPEEDR, 20);
	io_set_bit(GPIOC_OSPEEDR, 21);

	// PC11 -> D5
	io_set_bit(GPIOC_MODER, 22);
	io_clear_bit(GPIOC_MODER, 23);

	io_set_bit(GPIOC_OSPEEDR, 22);
	io_set_bit(GPIOC_OSPEEDR, 23);

	// PC12 -> D6
	io_set_bit(GPIOC_MODER, 24);
	io_clear_bit(GPIOC_MODER, 25);

	io_set_bit(GPIOC_OSPEEDR, 24);
	io_set_bit(GPIOC_OSPEEDR, 25);

	// PD2 -> D7
	io_set_bit(GPIOD_MODER, 4);
	io_clear_bit(GPIOD_MODER, 5);

	io_set_bit(GPIOD_OSPEEDR, 4);
	io_set_bit(GPIOD_OSPEEDR, 5);

	// PB13 -> RS
	io_set_bit(GPIOB_MODER, 26);
	io_clear_bit(GPIOB_MODER, 27);

	io_set_bit(GPIOB_OSPEEDR, 26);
	io_set_bit(GPIOB_OSPEEDR, 27);

	// PB15 -> E
	io_set_bit(GPIOB_MODER, 30);
	io_clear_bit(GPIOB_MODER, 31);

	io_set_bit(GPIOB_OSPEEDR, 30);
	io_set_bit(GPIOB_OSPEEDR, 31);

	// set interface type, font, and lines
	lcd_write_byte((uint8_t)0x38, INSTRUCTION);
	sys_sleep(1);

	// clear display and set cursor home
	lcd_write_byte((uint8_t)0x1, INSTRUCTION);
	sys_sleep(2);

	// set entry mode and increment cursor
	lcd_write_byte((uint8_t)0x6, INSTRUCTION);
	sys_sleep(1);

	// turn on display
	lcd_write_byte((uint8_t)0xC, INSTRUCTION);
	sys_sleep(1);

	logger(INFO, "Initialised LCD in 8-bit mode");
}

// write the given string from the current cursor position
void lcd_write_string(char *str)
{
	char *args[] = {str};
	loggerf(DEBUG, "Writing string to lcd: &", NULL, 0, args, 1);

	for (int i = 0; i < LCD_LINE_LENGTH; i++)
	{
		if (str[i] == '\0')
		{
			break;
		}
		lcd_write_byte(str[i], DATA);
	}
}

void lcd_write_string_and_scroll(char *str, uint32_t start_x, uint32_t start_y)
{
	lcd_set_cursor(start_x, start_y);

	lcd_write_string(str);
	uint32_t len = str_len(str);

	if (len <= LCD_LINE_LENGTH)
	{
		return;
	}

	// dont scroll too early
	sys_sleep(1000);

	uint32_t remaining_characters = len - LCD_LINE_LENGTH;

	char *str_trunc;
	for (int i = 0; i <= remaining_characters; i++)
	{
		str_trunc = &str[i];
		if (str_trunc[i] == '\0')
		{
			logger(DEBUG, "LCD exiting scroll");
			return;
		}
		lcd_set_cursor(start_x, start_y);
		lcd_write_string(str_trunc);
		sys_sleep(100);
	}
}

void lcd_clear_display()
{
	logger(DEBUG, "Clearing lcd display");
	lcd_write_byte((uint8_t)0x1, INSTRUCTION);
	sys_sleep(2);
}

void lcd_set_cursor(uint32_t x, uint32_t y)
{
	if (y > 1)
	{
		uint32_t args[] = {x, y};
		loggerf(ERROR, "Invalid cursor position: ($, $)", args, 2, NULL, 0);
		return;
	}

	uint32_t y_offset = y * 0x40;
	uint32_t pos = y_offset + x;

	uint32_t args2[] = {pos};
	loggerf(DEBUG, "Setting cursor address to $", args2, 1, NULL, 0);

	lcd_write_byte((uint8_t)set_bit(pos, 7), INSTRUCTION);
	sys_sleep(1);
}