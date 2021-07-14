#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

const uint E_PIN = 2;
const uint RS_PIN = 0;
const uint RW_PIN = 1;
const uint DB7_PIN = 10;
const uint DB6_PIN = 9;
const uint DB5_PIN = 8;
const uint DB4_PIN = 7;
const uint DB3_PIN = 6;
const uint DB2_PIN = 5;
const uint DB1_PIN = 4;
const uint DB0_PIN = 3;

int set_pins(uint the_pins[8], uint rs, uint rw, char pin_state);
int init_display(uint the_pins[8]);
int init_pins(uint the_pins[8]);
int display_msg(uint the_pins[8], char *message);
int wait_for_busy_flag(uint the_pins[8]);
int jump_to_line_2(uint the_pins[8]);
int jump_to_pos(uint the_pins[8], uint pos);
/*
int main()
{
	uint pins[8];
	
	pins[0] = DB7_PIN;
	pins[1] = DB6_PIN;
	pins[2] = DB5_PIN;
	pins[3] = DB4_PIN;
	pins[4] = DB3_PIN;
	pins[5] = DB2_PIN;
	pins[6] = DB1_PIN;
	pins[7] = DB0_PIN;

	stdio_init_all();

	printf("Hello, world!");
	
	init_pins(pins);
	init_display(pins);
	jump_to_line_2(pins);
	display_msg(pins, "Hello, world!");
	jump_to_pos(pins, 0);
	display_msg(pins, "Hello, again!");

    	return 0;
}
*/
int set_pins(uint the_pins[8], uint rs, uint rw, char pin_state)
// set the pins for the LCD Display
// char pin_state is a byte to hold the D7 to D0 pins
// rs - register select pin
// rw - read/write pin
{
	
	wait_for_busy_flag(the_pins);
	gpio_put(E_PIN, 0);
	sleep_us(1);
	gpio_put(RS_PIN, rs);
	gpio_put(RW_PIN, rw);
	for (int bit_idx = 0; bit_idx < 8; bit_idx++){
		gpio_put(the_pins[bit_idx],  pin_state & (0x80 >> bit_idx));
		
	}
	sleep_us(1);
	gpio_put(E_PIN,1);
	sleep_us(1);
	gpio_put(E_PIN,0);
	
	return 0;
}

int init_display(uint the_pins[8])
{
	// Page 45 of HD44780 Datasheet as reference for following 8 bit initialization
	sleep_ms(45);
	set_pins(the_pins, 0, 0, 0x30);
	sleep_ms(5);
	set_pins(the_pins, 0, 0, 0x30);
	sleep_us(120);
	set_pins(the_pins, 0, 0, 0x30);
	// Function set to 8-bit operation; 2 line display
	set_pins(the_pins, 0, 0, 0x38);
	// Turn on the display and cursor
	set_pins(the_pins, 0, 0, 0x0e);
	// Set entry mode to increment and not shift display
	set_pins(the_pins, 0, 0, 0x06);
	// Clear screen and move to home
	set_pins(the_pins, 0, 0, 0x01);

	return 0;

}

int init_pins(uint the_pins[8])
{
	gpio_init(E_PIN);
	gpio_set_dir(E_PIN, GPIO_OUT);
	gpio_init(RS_PIN);
	gpio_set_dir(RS_PIN, GPIO_OUT);
	gpio_init(RW_PIN);
	gpio_set_dir(RW_PIN, GPIO_OUT);
	for (int i = 0; i < 8; i++)
	{
		gpio_init(the_pins[i]);
		gpio_set_dir(the_pins[i], GPIO_OUT);
	}

	return 0;
}

int display_msg(uint the_pins[8], char *message)
{
	for (int byte_idx = 0; byte_idx < strlen(message); byte_idx++) {
		char d_byte = message[byte_idx];
		set_pins(the_pins, 1, 0, d_byte);
	}

	return 0;
}

int wait_for_busy_flag(uint the_pins[8])
// Waits for the LCD's busy flag to clear.

{
	//get the current state of the pins to return to once busy flag if off
	uint start_state_DB[8];
	uint start_state_RS;
	uint start_state_RW;

	start_state_RS = gpio_get_out_level(RS_PIN);
	start_state_RW = gpio_get_out_level(RW_PIN);

	for (int i = 0; i < 8; i++){
		start_state_DB[i] = gpio_get_out_level(the_pins[i]);
	}

	//start checking the busy flag
	uint busy = 1;
	
	gpio_put(E_PIN, 0);
	gpio_put(RS_PIN, 0);
	gpio_put(RW_PIN, 1);
	
	gpio_set_dir(the_pins[0], GPIO_IN);
	while (busy != 0) {

		sleep_us(1);
		gpio_put(E_PIN, 1);
		sleep_us(1);
		busy = gpio_get(the_pins[0]);
		sleep_us(1);
		gpio_put(E_PIN, 0);
	
	
	}
	gpio_set_dir(the_pins[0], GPIO_OUT);

	// Reset the pins to their orgional state
	gpio_put(RW_PIN, start_state_RW);
	gpio_put(RS_PIN, start_state_RS);
	for (int i = 0; i < 8; i++){
		gpio_put(the_pins[i], start_state_DB[i]);
	}
	return 0;
}

int jump_to_line_2(uint the_pins[8])
{
    	jump_to_pos(the_pins, 40);
}

int jump_to_pos(uint the_pins[8], uint pos) {
	set_pins(the_pins, 0,0, (0x80 + pos));
}
