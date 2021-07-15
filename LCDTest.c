#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"
#include "./h_files/pico_LCD_display.h"

#define E_PIN 2
#define RS_PIN 0
#define RW_PIN 1
#define DB7_PIN 10
#define DB6_PIN 9
#define DB5_PIN 8
#define DB4_PIN 7
#define DB3_PIN 6
#define DB2_PIN 5
#define DB1_PIN 4
#define DB0_PIN 3


void main() {

	const struct lcd_display display={RS_PIN, RW_PIN, E_PIN, {DB7_PIN, DB6_PIN, DB5_PIN, DB4_PIN, DB3_PIN, DB2_PIN, DB1_PIN, DB0_PIN}};

	stdio_init_all();

	lcd_init_pins(display);
	lcd_init_display(display);
	lcd_display_msg(display, "Hello, World!");
	lcd_jump_to_line_2(display);
	lcd_display_msg(display, "Hello again!");
	lcd_set_cursor(display, false);
}
