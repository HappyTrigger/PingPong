/*
 * OLED_DRIVER.h
 *
 * Created: 17.09.2015 10:09:41
 *  Author: michalma
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

#include <stdint.h>

#define OLED_BASE_SRAM_ADDRESS SRAM_BASE_ADDR
#define OLED_ADDR_SPACE_LEN 1024
#define OLED_LINE_LENGTH_COLUMNS 128
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

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
void write_c(uint8_t command);
void write_d(uint8_t data);

void refresh_oled();
void clear_oled();
void reset_position();
uint8_t set_position(uint8_t column, uint8_t page);

uint8_t set_pixel(uint8_t x,uint8_t y);
uint8_t unset_pixel(uint8_t x,uint8_t y);

void print_char(char character);
void print_inverted_char(char character);
uint8_t print_string(char* string);
void revert_colour_line(uint8_t line);

uint16_t print_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
uint16_t print_polygon(uint16_t n, uint16_t* coord);

void draw_circle(uint16_t xc, uint16_t yc, uint16_t r);

void screensaver();
void end_game_animation(uint8_t dir);

#endif /* OLED_DRIVER_H_ */