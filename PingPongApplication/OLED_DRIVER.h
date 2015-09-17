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


void init_oled();
void write_c(char command);
void write_d(char data);
void refresh_oled();
void clear_oled();
void reset_position();
int set_pixel(unsigned int x,unsigned int y);
int unset_pixel(unsigned int x,unsigned int y);

void print_char(char character);
int print_string(char* string);
int set_position(unsigned int x, unsigned int y);


void screensaver();



#endif /* OLED_DRIVER_H_ */