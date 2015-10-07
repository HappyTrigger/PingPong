/*
 * SPI_DRIVER.c
 *
 * Created: 01.10.2015 10:06:18
 *  Author: haakongn
 */ 
#include "SPI_DRIVER.h"
#include <avr/io.h>

/****************************************************************************
* \brief Initialize the SPI communication
*
****************************************************************************/
void SPI_init()
{
	/* Drive MOSI, SCK and CS HIGH*/
	DDR_SPI |= (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_CS);
	/* Enable SPI, Master mode, clock rate fck/16*/
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

/****************************************************************************
* \brief Write data to slave unit
*
* \param in byte to be written
****************************************************************************/
void SPI_write(uint8_t data)
{
	SPDR = data;
	
	while(!(SPSR & (1 << SPIF)));
}

/****************************************************************************
* \brief Read data from slave unit
*
* \return Read data
****************************************************************************/
uint8_t SPI_read()
{
	volatile uint8_t retVal;
	SPDR = 0xFF;
	
	while(!(SPSR & (1 << SPIF)));
	
	retVal = SPDR;
	
	return retVal;
}