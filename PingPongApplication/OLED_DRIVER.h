/*
 * OLED_DRIVER.h
 *
 * Created: 17.09.2015 10:09:41
 *  Author: michalma
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_
#define OLED_BASE_SRAM_ADDRESS SRAM_BASE_ADDR
#define OLED_ADDR_SPACE_LEN 1024
#define OLED_LINE_LENGTH_COLUMNS 128
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#include "JOYSTICK_DRIVER.h"

typedef enum
{
	Username,
	NewGame,
	Options,
	HighScores,
	Sound,
	Mode
} ScreenName;

void init_oled();
void write_c(char command);
void write_d(char data);

void refresh_oled();
void clear_oled();
void reset_position();
int set_position(unsigned int column, unsigned int page);

int set_pixel(unsigned int x,unsigned int y);
int unset_pixel(unsigned int x,unsigned int y);

void print_char(char character);
void print_inverted_char(char character);
int print_string(char* string);
void revert_colour_line(unsigned char line);

int print_line(int x1, int y1, int x2, int y2);
int print_polygon(int n, int* coord);

void draw_circle(int xc, int yc, int r);


void draw_screen(ScreenName name, JoystickDirection direction);

void screensaver();

#endif /* OLED_DRIVER_H_ */