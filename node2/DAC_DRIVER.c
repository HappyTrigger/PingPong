// 
// 
// 

#include "DAC_DRIVER.h"

void dac_init()
{
	pinMode(A3, OUTPUT);
	pinMode(53, OUTPUT);
	pinMode(A5, OUTPUT);
	pinMode(A6, OUTPUT);

	digitalWrite(A3, LOW);
	digitalWrite(A5, HIGH);
	digitalWrite(A6, HIGH);
	
}

