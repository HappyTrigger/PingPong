#ifndef _USART_DRIVER_ 
#define _USART_DRIVER_ 

#include <stdint.h>

#define FOSC 4915200UL
#define BAUD 9600
#define MYUBBR FOSC/16/BAUD-1

void USART_init(uint16_t ubrr);
uint8_t USART_putchar(char c);
char USART_getchar();



#endif /*_USART_DRIVER_*/