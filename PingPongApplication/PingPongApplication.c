/*
 * PingPongApplication.c
 *
 * Created: 27.08.2015 15:29:02
 *  Author: haakongn
 */ 


//#include <delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 4915200UL
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBBR FOSC/16/BAUD-1

unsigned char cReceived;
unsigned char cTransmit;
unsigned char[] = {'a','d','r'};
int nTransmitLength;

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

void USART_Transmit(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while(!( UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
	//PORTC = 0x00;
}

unsigned char USART_Receive(void)
{
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}



ISR(USART0_RXC_vect)
{
	cReceived = UDR0;
//	PORTC = 0xFF;
	USART_Transmit(cReceived);
}

ISR(USART0_TXC_vect)
{
	if (nTransmitLength)
	{
		UDR0 = cTransmit;	
	}
	else
	{
		UCSR0B &= ~(1<<TXCIE0);
	}
	PORTC = 0xFF;
	//USART_Transmit(cReceived);
}

int main(void)
{	
	DDRC = 0xFF;
	PORTC = 0x00;
	unsigned char c = 0;
	
	sei();
	USART_Init(MYUBBR);

	
	while(1)
	{
		//c = USART_Receive();
		//USART_Transmit(cReceived);
		//USART_Transmit('\n');
		//PORTC = 0x00;
		_delay_ms(500);
		//c = 'b';
		//USART_Transmit(c);
		//PORTC = 0xFF;
		_delay_ms(500);
	}
}



