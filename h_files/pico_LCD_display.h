#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

struct lcd_display {
	uint e_pin;
	uint rs_pin;
	uint rw_pin;
	uint db_pins[8];
};

void set_pins(struct lcd_display display, uint rs, uint rw, char pin_state);
void init_display(struct lcd_display display);
void init_pins(struct lcd_display display);
void wait_for_busy_flag(struct lcd_display display);
void display_msg(struct lcd_display display, char *message);
void jump_to_line_2(struct lcd_display display);
void jump_to_pos(struct lcd_display display, uint pos);


void set_pins(struct lcd_display display, uint rs, uint rw, char pin_state)
// set the pins for the LCD Display
// char pin_state is a byte to hold the D7 to D0 pins
// rs - register select pin
// rw - read/write pin
{
	
	wait_for_busy_flag(display);
	gpio_put(display.e_pin, 0);
	sleep_us(1);
	gpio_put(display.rs_pin, rs);
	gpio_put(display.rw_pin, rw);
	for (int bit_idx = 0; bit_idx < 8; bit_idx++){
		gpio_put(display.db_pins[bit_idx],  pin_state & (0x80 >> bit_idx));
		
	}
	sleep_us(1);
	gpio_put(display.e_pin,1);
	sleep_us(1);
	gpio_put(display.e_pin,0);
	
}


void init_display(struct lcd_display display)
{
	// Page 45 of HD44780 Datasheet as reference for following 8 bit initialization
	sleep_ms(45);
	set_pins(display, 0, 0, 0x30);
	sleep_ms(5);
	set_pins(display, 0, 0, 0x30);
	sleep_us(120);
	set_pins(display, 0, 0, 0x30);
	// Function set to 8-bit operation; 2 line display
	set_pins(display, 0, 0, 0x38);
	// Turn on the display and cursor
	set_pins(display, 0, 0, 0x0e);
	// Set entry mode to increment and not shift display
	set_pins(display, 0, 0, 0x06);
	// Clear screen and move to home
	set_pins(display, 0, 0, 0x01);

}

void init_pins(struct lcd_display display)
{
	gpio_init(display.e_pin);
	gpio_set_dir(display.e_pin, GPIO_OUT);
	gpio_init(display.rs_pin);
	gpio_set_dir(display.rs_pin, GPIO_OUT);
	gpio_init(display.rw_pin);
	gpio_set_dir(display.rw_pin, GPIO_OUT);
	for (int i = 0; i < 8; i++)
	{
		gpio_init(display.db_pins[i]);
		gpio_set_dir(display.db_pins[i], GPIO_OUT);
	}

}

void display_msg(struct lcd_display display, char *message)
{
	for (int byte_idx = 0; byte_idx < strlen(message); byte_idx++) {
		char d_byte = message[byte_idx];
		set_pins(display, 1, 0, d_byte);
	}

}

void wait_for_busy_flag(struct lcd_display display)
// Waits for the LCD's busy flag to clear.

{
	//get the current state of the pins to return to once busy flag if off
	uint start_state_DB[8];
	bool start_state_RS;
	bool start_state_RW;

	start_state_RS = gpio_get(display.rs_pin);
	start_state_RW = gpio_get(display.rw_pin);

	for (int i = 0; i < 8; i++){
		start_state_DB[i] = gpio_get(display.db_pins[i]);
	}

	//start checking the busy flag
	uint busy = 1;
	
	gpio_put(display.e_pin, 0);
	gpio_put(display.rs_pin, 0);
	gpio_put(display.rw_pin, 1);
	
	gpio_set_dir(display.db_pins[0], GPIO_IN);
	while (busy != 0) {

		sleep_us(1);
		gpio_put(display.e_pin, 1);
		sleep_us(1);
		busy = gpio_get(display.db_pins[0]);
		sleep_us(1);
		gpio_put(display.e_pin, 0);
	
	
	}
	gpio_set_dir(display.db_pins[0], GPIO_OUT);

	// Reset the pins to their orgional state
	gpio_put(display.rw_pin, start_state_RW);
	gpio_put(display.rs_pin, start_state_RS);
	for (int i = 0; i < 8; i++){
		gpio_put(display.db_pins[i], start_state_DB[i]);
	}
}

void jump_to_line_2(struct lcd_display display)
{
    	jump_to_pos(display, 40);
}

void jump_to_pos(struct lcd_display display, uint pos) {
	set_pins(display, 0,0, (0x80 + pos));
}
