#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#define LCD_LINE_LENGTH 16
#define LCD_MAX_MSG_LEN 64

void configure_lcd(void);

void lcd_write_string(char * str);

// writes the given string to the lcd and scrolls to the end if it doesnt fit. This blocks during the scroll
// as we have no threads as of yet.
void lcd_write_string_and_scroll(char *str, uint32_t start_x, uint32_t start_y);

void lcd_clear_display();

// set the cursor to the given coordinates. x gives the column and y specifies the row
// e.g (3, 1) third colum, second row.
void lcd_set_cursor(uint32_t x, uint32_t y);

#endif