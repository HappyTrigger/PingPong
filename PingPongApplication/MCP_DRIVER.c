/*
 * MCP_DRIVER.c
 *
 * Created: 01.10.2015 12:51:39
 *  Author: haakongn
 */ 
#include "MCP_DRIVER.h"
#include "SPI_DRIVER.h"
#include <avr/io.h>
#include <stdio.h>


/****************************************************************************
* \brief Enable communication with CAN controller
*
****************************************************************************/
void mcp_turn_on()
{
	PORTB &= ~(1 << DD_CS);
}

/****************************************************************************
* \brief Disable communication with CAN controller
*
****************************************************************************/
void mcp_turn_off()
{
	PORTB |= (1 << DD_CS);
}

/****************************************************************************
* \brief Initialize communication with CAN controller
*
* \return SUCESS/FAILURE
****************************************************************************/
uint8_t mcp_init()
{
	uint8_t tmp;
	
	SPI_init();
	mcp_reset();
	
	tmp = mcp_read(MCP_CANSTAT);
	
	if ((tmp & MODE_MASK) != MODE_CONFIG)
	{
		printf("MCP2515 NOT configured properly\n");
		return 1;
	}
	
	return 0;
}

/****************************************************************************
* \brief Reset the CAN controller
*
****************************************************************************/
void mcp_reset()
{
	mcp_turn_on();
	
	SPI_write(MCP_RESET);
	
	mcp_turn_off();
}

/****************************************************************************
* \brief Read data from CAN controller
*
* \param in address from which shall be read
* \return data
****************************************************************************/
uint8_t mcp_read(uint8_t address)
{
	volatile uint8_t retVal;
	
	mcp_turn_on();
	
	SPI_write(MCP_READ);
	SPI_write(address);
	
	retVal = SPI_read();
	
	mcp_turn_off();
	
	return retVal;
}

/****************************************************************************
* \brief Write data to CAN controller
*
* \param in address to which shall be write
* \return data
****************************************************************************/
void mcp_write_byte(uint8_t address, uint8_t byte)
{
	mcp_turn_on();
	SPI_write(MCP_WRITE);
	SPI_write(address);
	SPI_write(byte);
	mcp_turn_off();
}

/****************************************************************************
* \brief Read status of CAN controller
*
* \return status
****************************************************************************/
uint8_t mcp_read_status()
{
	volatile uint8_t retVal;
	mcp_turn_on();
	SPI_write(MCP_READ_STATUS);
	retVal=SPI_read();
	mcp_turn_off();
	return retVal;
}

/****************************************************************************
* \brief Modify bits of CAN controller
*
* \param in address of the register
* \param in bit mask
* \param in new value of the register
****************************************************************************/
void mcp_modify_bit(uint8_t address, uint8_t mask, uint8_t byte)
{
	mcp_turn_on();
	SPI_write(MCP_BITMOD);
	SPI_write(address);
	SPI_write(mask);
	SPI_write(byte);
	mcp_turn_off();
}

/****************************************************************************
* \brief Request to send particular buffer
*
* \param in address of the buffer
****************************************************************************/
void mcp_request_to_send(uint8_t bits)
{
	mcp_turn_on();
	SPI_write(MCP_RTS | (bits & 0b00000111));
	mcp_turn_off();
}