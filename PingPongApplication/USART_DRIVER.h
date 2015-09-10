#ifndef _USART_DRIVER_ 
#define _USART_DRIVER_ 

//#define F_CPU 4915200UL
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBBR FOSC/16/BAUD-1

void USART_Init(unsigned int ubrr); 
int USART_putchar(char c);
int USART_getchar();



#endif /*_USART_DRIVER_*/