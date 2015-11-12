/*
 * CAN_DRIVER.c
 *
 * Created: 01.10.2015 13:50:02
 *  Author: haakongn
 */ 

#include "CAN_DRIVER.h"
#include "MCP_DRIVER.h"
#include <avr/io.h>
#include "PING_PONG_LIB.h"
#include <stdint.h>
//#include <avr/interrupt.h>
//#include <avr/delay.h>

/****************************************************************************
* \brief Initialize CAN communication with node 2
*
****************************************************************************/
void CAN_init()
{
	mcp_init();
	/* Turn off mask/filters, receive any message, rollover enabled */
	mcp_modify_bit(MCP_RXB0CTRL, 0b01100100, 0xFF);
	/* Loopback mode enabled */
	mcp_modify_bit(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);
	/* Interrupt after received buffer 0 */
	//mcp_modify_bit(MCP_CANINTE, 0x01, 0x01);
	/*
	// Enable CAN interrupt 
	clear_bit(DDRD, PD2);
	
	set_bit(GIFR, INT1);
	set_bit(MCUCR, ISC11);
	clear_bit(MCUCR, ISC10);
	set_bit(GICR, INT1);
	*/
}

/****************************************************************************
* \brief Send a message through CAN to node 2
*
* \param in message
* \return result of the CAN operation
****************************************************************************/
uint8_t CAN_send_message(CANMessage message)
{
	uint8_t ctrl = mcp_read(MCP_TXB0CTRL);
	
	if (test_bit(ctrl, TXREQ))
	{
		return MESSAGE_PENDING;
	}
	/* Use lower 8bits of ID */
	mcp_write_byte(MCP_TXB0SIDL, (uint8_t)(message.ID << 5));
	mcp_write_byte(MCP_TXB0SIDH, (uint8_t)(message.ID >> 3));
	
	mcp_write_byte(MCP_TXB0DLC, (message.length) & 0x0F);
	
	for (int i = 0; i < message.length; i++)
	{
		mcp_write_byte(MCP_TXB0D0 + i, message.data_array[i]);
	}
	
	mcp_request_to_send(1);

	return SUCCESS;
}

/****************************************************************************
* \brief Receive a message from node 2 through CAN
*
* \param out message
* \return result of the CAN operation
****************************************************************************/
uint8_t CAN_receive_message(CANMessage* message)
{
	int i = 0;
	uint8_t received = mcp_read(MCP_CANINTF) & (0x01);
	if (received)
	{
		mcp_modify_bit(MCP_CANINTF, 0x01, 0);
		message->ID = (mcp_read(MCP_RXB0SIDH) << 3);
		message->ID |= (mcp_read(MCP_RXB0SIDL) >> 5);
		
		message->length = mcp_read(MCP_RXB0DLC) & (0x0F);
		
		for (i = 0; i < message->length; i++)
		{
			message->data_array[i] = mcp_read(MCP_RXB0D0 + i);
		}
	}
	else
	{
		return NO_NEW_MESSAGE;
	}
	
	return SUCCESS;
}

/*
ISR(INT1_vect)
{
	cli();
	printf("2\n");
	//GIFR &= ~(1 << INT1);
	mcp_modify_bit(MCP_CANINTF, 0x01, 0);
	//printf("Interrupt\n");
	sei();
}
*/