#include <stdint.h>

#include "io.h"
#include "gpio.h"
#include "utils.h"
#include "systick.h"

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
	switch (ot)
	{
	case DATA:
		io_set_bit(GPIOB_ODR, RS);
	case INSTRUCTION:
		io_clear_bit(GPIOB_ODR, RS);
	}

	io_match_bit(GPIOC_ODR, get_bit(data, 0), D0); // 0x40020814
	io_match_bit(GPIOC_ODR, get_bit(data, 1), D1);
	io_match_bit(GPIOB_ODR, get_bit(data, 2), D2);
	io_match_bit(GPIOC_ODR, get_bit(data, 3), D3);
	io_match_bit(GPIOC_ODR, get_bit(data, 4), D4);
	io_match_bit(GPIOC_ODR, get_bit(data, 5), D5);
	io_match_bit(GPIOC_ODR, get_bit(data, 6), D6);
	io_match_bit(GPIOD_ODR, get_bit(data, 7), D7);

	// toggle enable bit to send data
	io_set_bit(GPIOB_ODR, E); // 0x40020414
	io_clear_bit(GPIOB_ODR, E);
}

void configure_lcd(void)
{
	// set GPIO pins mode to output and pull up

	// PC9 -> D0
	io_set_bit(GPIOC_MODER, 18);
	io_clear_bit(GPIOC_MODER, 19);

	io_set_bit(GPIOC_PUPDR, 18);
	io_clear_bit(GPIOC_PUPDR, 19);

	io_clear_bit(GPIOC_OSPEEDR, 18);
	io_clear_bit(GPIOC_OSPEEDR, 19);

	// PC8 -> D1
	io_set_bit(GPIOC_MODER, 16);
	io_clear_bit(GPIOC_MODER, 17);

	io_set_bit(GPIOC_PUPDR, 16);
	io_clear_bit(GPIOC_PUPDR, 17);

	io_clear_bit(GPIOC_OSPEEDR, 16);
	io_clear_bit(GPIOC_OSPEEDR, 17);

	// PB8 -> D2
	io_set_bit(GPIOB_MODER, 16);
	io_clear_bit(GPIOB_MODER, 17);

	io_set_bit(GPIOB_PUPDR, 16);
	io_clear_bit(GPIOB_PUPDR, 17);

	io_clear_bit(GPIOB_OSPEEDR, 16);
	io_clear_bit(GPIOB_OSPEEDR, 17);

	// PC6 -> D3
	io_set_bit(GPIOC_MODER, 12);
	io_clear_bit(GPIOC_MODER, 13);

	io_set_bit(GPIOC_PUPDR, 12);
	io_clear_bit(GPIOC_PUPDR, 13);

	io_clear_bit(GPIOC_OSPEEDR, 12);
	io_clear_bit(GPIOC_OSPEEDR, 13);

	// PC10 -> D4
	io_set_bit(GPIOC_MODER, 20);
	io_clear_bit(GPIOC_MODER, 21);

	io_set_bit(GPIOC_PUPDR, 20);
	io_clear_bit(GPIOC_PUPDR, 21);

	io_clear_bit(GPIOC_OSPEEDR, 20);
	io_clear_bit(GPIOC_OSPEEDR, 21);

	// PC11 -> D5
	io_set_bit(GPIOC_MODER, 22);
	io_clear_bit(GPIOC_MODER, 23);

	io_set_bit(GPIOC_PUPDR, 22);
	io_clear_bit(GPIOC_PUPDR, 23);

	io_clear_bit(GPIOC_OSPEEDR, 22);
	io_clear_bit(GPIOC_OSPEEDR, 23);

	// PC12 -> D6
	io_set_bit(GPIOC_MODER, 24);
	io_clear_bit(GPIOC_MODER, 25);

	io_set_bit(GPIOC_PUPDR, 24);
	io_clear_bit(GPIOC_PUPDR, 25);

	io_clear_bit(GPIOC_OSPEEDR, 24);
	io_clear_bit(GPIOC_OSPEEDR, 25);

	// PD2 -> D7
	io_set_bit(GPIOD_MODER, 4);
	io_clear_bit(GPIOD_MODER, 5);

	io_set_bit(GPIOD_PUPDR, 4);
	io_clear_bit(GPIOD_PUPDR, 5);

	io_clear_bit(GPIOD_OSPEEDR, 4);
	io_clear_bit(GPIOD_OSPEEDR, 5);

	// PB13 -> RS
	io_set_bit(GPIOB_MODER, 26);
	io_clear_bit(GPIOB_MODER, 27);

	io_set_bit(GPIOB_PUPDR, 26);
	io_clear_bit(GPIOB_PUPDR, 27);

	io_clear_bit(GPIOB_OSPEEDR, 26);
	io_clear_bit(GPIOB_OSPEEDR, 27);

	// PB15 -> E
	io_set_bit(GPIOB_MODER, 30);
	io_clear_bit(GPIOB_MODER, 31);

	io_set_bit(GPIOB_PUPDR, 30);
	io_clear_bit(GPIOB_PUPDR, 31);

	io_clear_bit(GPIOB_OSPEEDR, 30);
	io_clear_bit(GPIOB_OSPEEDR, 31);

	// set interface type, font, and lines
	lcd_write_byte((uint8_t)0x30, INSTRUCTION);
	sys_sleep(1);

	// turn on display
	lcd_write_byte((uint8_t)0xF, INSTRUCTION);
	sys_sleep(1);

	// clear display
	lcd_write_byte((uint8_t)0x1, INSTRUCTION);
	sys_sleep(2);

	// set entry mode
	lcd_write_byte((uint8_t)0x6, INSTRUCTION);
	sys_sleep(1);

	lcd_write_byte((uint8_t)0x35, DATA);
}