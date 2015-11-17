/*
 * PingPongApplication.c
 *
 * Created: 27.08.2015 15:29:02
 *  Author: haakongn
 */ 

#define F_CPU 4915200UL

#include <stdio.h>
#include "USER_INTERFACE.h"

int main(void)
{	
	init();
	printf("Main system up and functional\n");
	
	while(1)
	{
		interface_state_machine();
	}
}