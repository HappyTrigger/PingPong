/*
 * OLED_DRIVER.c
 *
 * Created: 17.09.2015 10:09:58
 *  Author: michalma
 */ 

#include <avr/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include "OLED_DRIVER.h"
#include "PING_PONG_LIB.h"
#include "JOYSTICK_DRIVER.h"

static unsigned int position = 0;

const unsigned char PROGMEM font[95][5] = {
	{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}, //
	{0b00000000,0b00000000,0b01011110,0b00000000,0b00000000}, // !
	{0b00000000,0b00001110,0b00000000,0b00001110,0b00000000}, // "
	{0b00101000,0b01111100,0b00101000,0b01111100,0b00101000}, // #
	{0b00001000,0b01010100,0b01111100,0b01010100,0b00100000}, // $
	{0b00100110,0b00010000,0b00001000,0b01100100,0b00000000}, // %
	{0b00101000,0b01010100,0b00101000,0b01000000,0b00000000}, // &
	{0b00000000,0b00000000,0b00001110,0b00000000,0b00000000}, // '
	{0b00000000,0b00111100,0b01000010,0b00000000,0b00000000}, // (
	{0b00000000,0b01000010,0b00111100,0b00000000,0b00000000}, // )
	{0b00000000,0b01010100,0b00111000,0b01010100,0b00000000}, // *
	{0b00010000,0b00010000,0b01111100,0b00010000,0b00010000}, // +
	{0b00000000,0b10000000,0b01100000,0b00100000,0b00000000}, // ,
	{0b00010000,0b00010000,0b00010000,0b00010000,0b00000000}, // -
	{0b00000000,0b01100000,0b01100000,0b00000000,0b00000000}, // .
	{0b00100000,0b00010000,0b00001000,0b00000100,0b00000000}, // /
	{0b00000000,0b00111100,0b01000010,0b00111100,0b00000000}, // 0
	{0b00000000,0b01000100,0b01111110,0b01000000,0b00000000}, // 1
	{0b01000100,0b01100010,0b01010010,0b01001100,0b00000000}, // 2
	{0b00100010,0b01001010,0b01001010,0b00110110,0b00000000}, // 3
	{0b00011000,0b00010100,0b01111110,0b00010000,0b00000000}, // 4
	{0b00101110,0b01001010,0b01001010,0b00110010,0b00000000}, // 5
	{0b00111100,0b01001010,0b01001010,0b00110000,0b00000000}, // 6
	{0b00000010,0b01100010,0b00011010,0b00000110,0b00000000}, // 7
	{0b00110100,0b01001010,0b01001010,0b00110100,0b00000000}, // 8
	{0b00001100,0b01010010,0b01010010,0b00111100,0b00000000}, // 9
	{0b00000000,0b01101100,0b01101100,0b00000000,0b00000000}, // :
	{0b10000000,0b01101100,0b00101100,0b00000000,0b00000000}, // ;
	{0b00000000,0b00010000,0b00101000,0b01000100,0b00000000}, // <
	{0b00101000,0b00101000,0b00101000,0b00101000,0b00000000}, // =
	{0b00000000,0b01000100,0b00101000,0b00010000,0b00000000}, // >
	{0b00000000,0b00000100,0b01010010,0b00001100,0b00000000}, // ?
	{0b00111100,0b01000010,0b01011010,0b00011100,0b00000000}, // @
	{0b01111100,0b00010010,0b00010010,0b01111100,0b00000000}, // A
	{0b01111110,0b01001010,0b01001010,0b00110100,0b00000000}, // B
	{0b00111100,0b01000010,0b01000010,0b00100100,0b00000000}, // C
	{0b01111110,0b01000010,0b01000010,0b00111100,0b00000000}, // D
	{0b01111110,0b01001010,0b01001010,0b01000010,0b00000000}, // E
	{0b01111110,0b00001010,0b00001010,0b00000010,0b00000000}, // F
	{0b00111100,0b01000010,0b01010010,0b01110100,0b00000000}, // G
	{0b01111110,0b00001000,0b00001000,0b01111110,0b00000000}, // H
	{0b00000000,0b01000010,0b01111110,0b01000010,0b00000000}, // I
	{0b00100000,0b01000000,0b01000000,0b00111110,0b00000000}, // J
	{0b01111110,0b00011000,0b00100100,0b01000010,0b00000000}, // K
	{0b01111110,0b01000000,0b01000000,0b01000000,0b00000000}, // L
	{0b01111110,0b00001100,0b00001100,0b01111110,0b00000000}, // M
	{0b01111110,0b00001100,0b00110000,0b01111110,0b00000000}, // N
	{0b00111100,0b01000010,0b01000010,0b00111100,0b00000000}, // O
	{0b01111110,0b00010010,0b00010010,0b00001100,0b00000000}, // P
	{0b00111100,0b01100010,0b01000010,0b10111100,0b00000000}, // Q
	{0b01111110,0b00010010,0b00110010,0b01001100,0b00000000}, // R
	{0b00100100,0b01001010,0b01010010,0b00100100,0b00000000}, // S
	{0b00000000,0b00000010,0b01111110,0b00000010,0b00000000}, // T
	{0b00111110,0b01000000,0b01000000,0b00111110,0b00000000}, // U
	{0b00011110,0b01100000,0b01100000,0b00011110,0b00000000}, // V
	{0b01111110,0b00110000,0b00110000,0b01111110,0b00000000}, // W
	{0b01100110,0b00011000,0b00011000,0b01100110,0b00000000}, // X
	{0b00000000,0b00001110,0b01110000,0b00001110,0b00000000}, // Y
	{0b01100010,0b01010010,0b01001010,0b01000110,0b00000000}, // Z
	{0b00000000,0b01111110,0b01000010,0b01000010,0b00000000}, // [
	{0b00000100,0b00001000,0b00010000,0b00100000,0b00000000}, // "\"
	{0b00000000,0b01000010,0b01000010,0b01111110,0b00000000}, // ]
	{0b00000000,0b00000100,0b00000010,0b00000100,0b00000000}, // ^
	{0b01000000,0b01000000,0b01000000,0b01000000,0b00000000}, // _
	{0b00000000,0b00000010,0b00000100,0b00000000,0b00000000}, // `
	{0b00110000,0b01001000,0b00101000,0b01111000,0b00000000}, // a
	{0b01111110,0b01001000,0b01001000,0b00110000,0b00000000}, // b
	{0b00110000,0b01001000,0b01001000,0b00000000,0b00000000}, // c
	{0b00110000,0b01001000,0b01001000,0b01111110,0b00000000}, // d
	{0b00110000,0b01101000,0b01011000,0b00010000,0b00000000}, // e
	{0b00010000,0b01111100,0b00010010,0b00000100,0b00000000}, // f
	{0b01010000,0b10101000,0b10101000,0b10011000,0b00000000}, // g
	{0b01111110,0b00001000,0b00001000,0b01110000,0b00000000}, // h
	{0b00000000,0b01001000,0b01111010,0b01000000,0b00000000}, // i
	{0b00000000,0b01000000,0b10000000,0b01111010,0b00000000}, // j
	{0b01111110,0b00010000,0b00101000,0b01000000,0b00000000}, // k
	{0b00000000,0b01000010,0b01111110,0b01000000,0b00000000}, // l
	{0b01111000,0b00010000,0b00011000,0b01110000,0b00000000}, // m
	{0b01111000,0b00001000,0b00001000,0b01110000,0b00000000}, // n
	{0b00110000,0b01001000,0b01001000,0b00110000,0b00000000}, // o
	{0b11111000,0b01001000,0b01001000,0b00110000,0b00000000}, // p
	{0b00110000,0b01001000,0b01001000,0b11111000,0b00000000}, // q
	{0b01111000,0b00001000,0b00001000,0b00010000,0b00000000}, // r
	{0b01010000,0b01011000,0b01101000,0b00101000,0b00000000}, // s
	{0b00001000,0b00111110,0b01001000,0b01000000,0b00000000}, // t
	{0b00111000,0b01000000,0b01000000,0b01111000,0b00000000}, // u
	{0b00000000,0b00111000,0b01000000,0b00111000,0b00000000}, // v
	{0b01111000,0b01100000,0b01100000,0b01111000,0b00000000}, // w
	{0b01001000,0b00110000,0b00110000,0b01001000,0b00000000}, // x
	{0b00011000,0b10100000,0b01000000,0b00111000,0b00000000}, // y
	{0b01001000,0b01101000,0b01011000,0b01001000,0b00000000}, // z
	{0b00000000,0b00001000,0b00111100,0b01000010,0b00000000}, // {
	{0b00000000,0b00000000,0b01111110,0b00000000,0b00000000}, // |
	{0b00000000,0b01000010,0b00111100,0b00001000,0b00000000}, // }
	{0b00000100,0b00000010,0b00000100,0b00000010,0b00000000}, // ~
};

/****************************************************************************
* \brief Initialize OLED display
*
****************************************************************************/
void init_oled()
{
	write_c(0xae);    // display off
	write_c(0xa1);    //segment remap
	write_c(0xda);    //common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8);    //common output scan direction:com63~com0
	write_c(0xa8);    //multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5);    //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81);    //contrast control
	write_c(0x50);
	write_c(0xd9);    //set pre-charge period
	write_c(0x21);
	write_c(0x20);    //Set Memory Addressing Mode
	write_c(0x00);
	write_c(0xdb);    //VCOM deselect level mode
	write_c(0x30);
	write_c(0xad);    //master configuration
	write_c(0x00);
	write_c(0xa4);    //out follows RAM content
	write_c(0xa6);    //set normal display
	write_c(0xaf);    // display on
}

/****************************************************************************
* \brief Write command to OLED display
*
****************************************************************************/
void write_c(uint8_t command)
{
	volatile uint8_t *ext_oled = (uint8_t *) OLED_COMMAND_BASE_ADDR;
	ext_oled[0] = command;
}


/****************************************************************************
* \brief Write data to OLED display
*
****************************************************************************/
void write_d(uint8_t data)
{
	volatile uint8_t *ext_oled = (uint8_t *) OLED_DATA_BASE_ADDR;
	ext_oled[0] = data;
}

/****************************************************************************
* \brief Reset position to [0,0]
*
****************************************************************************/
void reset_position()
{
	write_c(0x21); // set column to 0
	write_c(0x00);
	write_c(0x7F);
	
	write_c(0x22); // set page to 0
	write_c(0x00);
	write_c(0x07);
}

/****************************************************************************
* \brief Refresh OLED by rewriting all data from SRAM to OLED
*
****************************************************************************/
void refresh_oled()
{
	uint16_t i;
	reset_position();
	volatile uint8_t *ext_ram = (uint8_t *) OLED_BASE_SRAM_ADDRESS;
	uint8_t oled_value;
	
	for (i = 0; i < OLED_ADDR_SPACE_LEN; i++)
	{
		oled_value = ext_ram[i];
		write_d(oled_value);
	}
}

/****************************************************************************
* \brief Clear OLED display
*
****************************************************************************/
void clear_oled()
{
	uint16_t i;
	volatile uint8_t *ext_ram = (uint8_t *) OLED_BASE_SRAM_ADDRESS;
	
	for (i = 0; i < OLED_ADDR_SPACE_LEN; i++)
	{
		ext_ram[i] = 0x00;
	}
}

/****************************************************************************
* \brief Set pixel on position [x,y] on OLED display
*
* \param in x Position of pixel that should be set
* \param in y Position of pixel that should be set
* \return Success/Failure
****************************************************************************/
uint8_t set_pixel(uint8_t x,uint8_t y)
{
	volatile uint8_t *ext_ram = (uint8_t *) OLED_BASE_SRAM_ADDRESS;
	
	if (x > 127)
	{
		return 1;
	}
	else if (y > 64)
	{
		return 1;
	}
	else
	{
		uint16_t page = y/8;
		uint16_t coloumn = page*128 + x;
		ext_ram[coloumn] |= (1 << (y % 8));
	}
	return 0;
}

/****************************************************************************
* \brief Unset pixel on position [x,y] on OLED display
*
* \param in x Position of pixel that should be unset
* \param in y Position of pixel that should be unset
* \return Success/Failure
****************************************************************************/
uint8_t unset_pixel(uint8_t x, uint8_t y)
{
	volatile char *ext_ram = (char *) OLED_BASE_SRAM_ADDRESS;
	
	if (x > 127)
	{
		return 1;
	}
	else if (y > 64)
	{
		return 1;
	}
	else
	{
		uint16_t page = y/8;
		uint16_t column = page * 128 + x;
		ext_ram[column] &= ~(1 << (y % 8));
	}
	return 0;
}


/****************************************************************************
* \brief Print character on current position on OLED display
*
* \param in x Position of pixel that should be set
****************************************************************************/
void print_char(char character)
{
	uint8_t i;
	volatile char* ext_ram = (char*) OLED_BASE_SRAM_ADDRESS;
	
	for(i = 0; i < 4; i++)
	{
		ext_ram[position] = pgm_read_word_near(&(font[character-32][i]));
		position++;
	}
}

/****************************************************************************
* \brief Print inverted character on current position on OLED display
*
* \param in x Position of pixel that should be set
****************************************************************************/
void print_inverted_char(char character)
{
	uint8_t i;
	volatile char* ext_ram = (char*) OLED_BASE_SRAM_ADDRESS;
	
	for(i = 0; i < 4; i++)
	{
		ext_ram[position] = ~pgm_read_word_near(&(font[character-32][i]));
		position++;
	}
}

/****************************************************************************
* \brief Set position of the next character
*
* \param in column Column on the X-axis
* \param in page Row on Y-axis
* \return Success/Failure
****************************************************************************/
uint8_t set_position(uint8_t column, uint8_t page)
{
	if (column > 32)
	{
		return 1;
	}
	else if (page > 8)
	{
		return 1;
	}
	
	position = page * 128 + column * 4;
	
	return 0;
}

/****************************************************************************
* \brief Print string to the OLED display
*
* \param in String that shall be printed
* \return Success/Failure
****************************************************************************/
uint8_t print_string(char* string)
{
	uint8_t i;
	uint8_t lenght  = strlen(string);
	for(i = 0; i < lenght; i++)
	{
		print_char(string[i]);
	}
	return 0;
}

/****************************************************************************
* \brief Revert the colour of the particular line
*
****************************************************************************/
void revert_colour_line(uint8_t line)
{
	uint8_t i;
	volatile uint8_t* ext_ram = (uint8_t*) OLED_BASE_SRAM_ADDRESS;
	
	for(i = (line * OLED_LINE_LENGTH_COLUMNS) + 2; i < ((line + 1) * OLED_LINE_LENGTH_COLUMNS); i++)
	{
		ext_ram[i-1] = ~ext_ram[i-1];
	}
}


/****************************************************************************
* \brief Print line to the OLED display
*
* \param in x1 X-axis position of the first point
* \param in y1 Y-axis position of the first point
* \param in x2 X-axis position of the second point
* \param in y2 Y-axis position of the second point
* \return Success/Failure
****************************************************************************/
uint16_t print_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t dx,dy,sx,sy,i;
	float k,p;
	
	if (x1 > OLED_WIDTH || x2 > OLED_WIDTH || y1 > OLED_HEIGHT || y2 > OLED_HEIGHT || x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0)
	{
		return 1;
	}
	
	if (x1 == x2 && y1 == y2)
	{
		set_pixel(x1,y1);
		return 0;
	}
	
	dx = abs(x2-x1);
	dy = abs(y2-y1);
	
	sx = x1 < x2 ? 1 : -1;
	sy = y1 < y2 ? 1 : -1;
	
	if (dx > dy)
	{
		k = (float) dy / dx;
		p = y1;
		for (i = x1; i != x2; i += sx)
		{
			set_pixel(x1, (uint16_t) (p+0.5));
			x1 += sx;
			p += k * sy;
		}
	}
	else
	{
		k = (float) dx / dy;
		p = x1;
		for (i = y1; i != y2; i += sy)
		{
			set_pixel((uint16_t)(p+0.5),y1);
			y1 += sy;
			p += k * sx;
		}
	}
	set_pixel(x2, y2);
	
	return 0;
}

/****************************************************************************
* \brief Print polygon to the OLED display
*
* \param in n Number of edges of the polygon
* \param in Coordinates of the polygon points
* \return Success/Failure
****************************************************************************/
uint16_t print_polygon(uint16_t n, uint16_t* coord)
{
	uint16_t i;
	if (n < 2)
	{
		return 1;
	}
	
	for (i = 0; i < 2 * (n-1); i += 2)
	{
		print_line(coord[i],coord[i+1],coord[i+2],coord[i+3]);
	}
	print_line(coord[i],coord[i+1],coord[0],coord[1]);
	
	return 0;
}


/****************************************************************************
* \brief Print circle to the OLED display using Bresenham's Circle Algorithm
*
* \param in X coordinate of circle center
* \param in Y coordinate of circle center
* \param in Radius of the circle
****************************************************************************/
void draw_circle(uint16_t xc, uint16_t yc, uint16_t r)
{
	uint16_t x, y = r;
	uint16_t d = 3 - 2 * r;
	
	for (x = 0; x < y; x++)
	{
		set_pixel(xc + x, yc + y);
		set_pixel(xc - x, yc + y);
		set_pixel(xc + x, yc - y);
		set_pixel(xc - x, yc - y);
		set_pixel(xc + y, yc + x);
		set_pixel(xc - y, yc + x);
		set_pixel(xc + y, yc - x);
		set_pixel(xc - y, yc - x);

		if (d < 0)
		{
			d = d + 4 * x + 6;
		}
		else
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
	}
}

/****************************************************************************
* \brief SCREENSAVER
*
****************************************************************************/
void screensaver()
{
	static uint8_t x = 0;
	static uint8_t dir = 0;
	uint8_t i;
	if (dir == 0)
	{
		x++;
		dir = x == (OLED_WIDTH - 1) ? 1 : 0;
	}
	else
	{
		x--;
		dir = x == 0 ? 0 : 1;
	}

	
	for (i = 0; i < OLED_HEIGHT; i++)
	{
		set_pixel(x, i);
	}
	refresh_oled();
	for (i = 0; i < OLED_HEIGHT; i++)
	{
		unset_pixel(x, i);
	}
	_delay_ms(10);
}

/****************************************************************************
* \brief Make a end game animation
*
* \param in direction of the animation
****************************************************************************/
void end_game_animation(uint8_t dir)
{
	uint8_t i;
	uint8_t start, end;
	start = dir == 0 ? 0 : OLED_WIDTH;
	end = dir == 0 ? OLED_WIDTH : 0;
	
	while (start != end)
	{
		for (i = 0; i < OLED_HEIGHT; i++)
		{
			set_pixel(start, i);
		}
		refresh_oled();
		for (i = 0; i < OLED_HEIGHT; i++)
		{
			unset_pixel(start, i);
		}
		start = dir == 0 ? start + 1 : start - 1;
		_delay_ms(10);
	}
}