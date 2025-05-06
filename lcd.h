#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#define LCD_LINE_LENGTH 16

void configure_lcd(void);

void lcd_write_string(char * str);

void lcd_clear_display();

// set the cursor to the given coordinates. x gives the column and y specifies the row
// e.g (3, 1) third colum, second row.
void lcd_set_cursor(uint32_t x, uint32_t y);

#endif