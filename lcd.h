#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void configure_lcd(void);

void lcd_write_string(char * str);

void lcd_clear_display();

void lcd_set_cursor(uint32_t x, uint32_t y);

#endif