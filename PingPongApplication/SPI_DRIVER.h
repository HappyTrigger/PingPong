/*
 * SPI_DRIVER.h
 *
 * Created: 01.10.2015 10:06:00
 *  Author: haakongn
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_
#include <stdint.h>


#define DDR_SPI DDRB
#define DD_CS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7

void SPI_init();
void SPI_write(uint8_t data);
uint8_t SPI_read();


#endif /* SPI_DRIVER_H_ */