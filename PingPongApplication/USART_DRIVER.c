#include "USART_DRIVER.h"
#include <avr/io.h>


/****************************************************************************
 * \brief Initialize USART unit to desired baud rate.
 * 
 * \param ubrr Desired baud rate.
 ****************************************************************************/
void USART_init(uint16_t ubrr)
{
	//Set baud rate
	UBRR0H = (uint8_t) (ubrr>>8);
	UBRR0L = (uint8_t) ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<< RXEN0) | (1<<TXEN0);
	// Set frame format: 8data, 2 stop bit
	UCSR0C = (1<<URSEL0) | (3<<UCSZ00) ;
}

/****************************************************************************
 * \brief Sends one char through USART.
 * 
 * \param c Char to be sent.
 * \return Always 0.
 ****************************************************************************/
uint8_t USART_putchar(char c)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

/****************************************************************************
 * \brief Receives one char through USART.
 * 
 * \return Received char.
 ****************************************************************************/
char USART_getchar()
{
	char c;
	loop_until_bit_is_set(UCSR0A, RXC0);
	c = UDR0;
	return c;
}
