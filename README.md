# pico_LCD_Display
First attempt to run an LCD Display from the pico using c.

This will be a header file that can be used to run the HD44780U (LCD-II) based LCD Display from the Raspberry Pi Pico.

The lcd_display struct holds the number of the GPIO pin that is connected to each of the LCD Display's pins.

<h3>LCD pin numbers assumed:</h3>
    <b>1</b>       - GND : Tied to ground         -- not included in lcd_display struct<br />
    <b>2</b>       - VDD : Tied to 5v             -- not included in lcd_display struct<br />
    <b>3</b>       - VO  : for brigtness control  -- not included in lcd_display<br />
    <b>4</b>       - RS  : Register Select        -- Required - char                      -- supply GPIO pin number this is connected to<br />
    <b>5</b>       - RW  : Read/Write Select      -- Required - char                      -- supply GPIO pin number this is selected to or 'N' if not connected to LCD Display<br />
    <b>6</b>       - E   : Enable Pin             -- Required - char                      -- supply GPIO pin number this is connected to<br />
    <b>7 to 14</b> - DB  : Data Pins              -- Required - array of 8 char           -- supply GPIO pins that this is connected to in an array from DB0 pin to DB7 pin<br />
    <b>15</b>      - BL1 : Back light 5v          -- not included in lcd_display struct<br />
    <b>16</b>      - BL2 : Back light GND         -- not included in lcd_display struct<br />

Recommended initlisation:
    
    const struct lcd_display display={RS_PIN, RW_PIN, E_PIN, {DB7_PIN, DB6_PIN, DB5_PIN, DB4_PIN, DB3_PIN, DB2_PIN, DB1_PIN, DB0_PIN}};

Functions available:
    
   Intended for use outside of header:
    
        void lcd_init_pins(struct lcd_display display);
        void lcd_init_display(struct lcd_display display);
        void lcd_display_msg(struct lcd_display display, char *message);
        void lcd_jump_to_line_2(struct lcd_display display);
        void lcd_jump_to_pos(struct lcd_display display, uint pos);
        void lcd_set_cursor(struct lcd_display display, bool cursor);

   Intended for use internally by the header:
    
        void lcd_set_pins(struct lcd_display display, uint rs, uint rw, char pin_state);
        void lcd_wait_for_busy_flag(struct lcd_display display);


To use:

1. Define pin numbers<br />
    for example:
    
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

2. Set up a const sturuct LCD-display<br />
    for example:
        
		const struct lcd_display display={RS_PIN, RW_PIN, E_PIN, {DB7_PIN, DB6_PIN, DB5_PIN, DB4_PIN, DB3_PIN, DB2_PIN, DB1_PIN, DB0_PIN}};

3. Initlise pins on pico<br />
    for example:
    
    	lcd_init_pins(display);

4. Initlise display<br />
    for example:
    
    	lcd_init_display(display);

5. Write message<br />
    for example:
    
    	lcd_display_msg(display, "Hello, World!");

Example use of header file:

    #include <stdio.h>
    #include "pico/stdlib.h"
    #include "hardware/gpio.h"
    #include "pico/binary_info.h"
    #include "../pico_LCD_Display/h_files/pico_LCD_display.h"

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

        // test code for rw_pin set to 0
        //gpio_init(1);
        //gpio_set_dir(1, GPIO_OUT);
        //gpio_put(1,0);

        lcd_init_pins(display);
        lcd_init_display(display);
        lcd_display_msg(display, "Hello, World!");
        lcd_jump_to_line_2(display);
        lcd_display_msg(display, "Hello again!");
        lcd_set_cursor(display, false);
    }
