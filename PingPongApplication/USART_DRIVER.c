#include "USART_DRIVER.h"
#include <avr/io.h>


/****************************************************************************
 * \brief Initialize USART unit to desired baud rate.
 * 
 * \param ubrr Desired baud rate.
 ****************************************************************************/
void USART_Init(unsigned int ubrr)
{
	//Set baud rate
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<< RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1<<TXCIE0);
	// Set frame format: 8data, 2 stop bit
	UCSR0C = (1<<URSEL0) | (3<<UCSZ00) ;
}

/****************************************************************************
 * \brief Sends one char through USART.
 * 
 * \param c Char to be sent.
 * \return Always 0.
 ****************************************************************************/
int USART_putchar(char c)
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
int USART_getchar()
{
	char c;
	loop_until_bit_is_set(UCSR0A, RXC0);
	c = UDR0;
	return c;
}


//ISR(USART0_RXC_vect)
//{
	//cReceived = UDR0;
	////	PORTC = 0xFF;
	//USART_Transmit(cReceived);
//}
//
//ISR(USART0_TXC_vect)
//{
	//if (nTransmitLength)
	//{
		//UDR0 = cTransmit;
	//}
	//else
	//{
		//UCSR0B &= ~(1<<TXCIE0);
	//}
	//PORTC = 0xFF;
	////USART_Transmit(cReceived);
//}

