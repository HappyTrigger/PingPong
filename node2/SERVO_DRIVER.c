// 
// 
// 

#include "SERVO_DRIVER.h"

#define REFRESH_INTERVAL 20000

uint8_t is_IR_interrupted()
{
	uint16_t sensorValue = 0;
	uint8_t retVal = 0;
	sensorValue = analogRead(A1);
	if (sensorValue < 100)
	{
		retVal = 1; //Something is blocking the IR
	}
	else
	{
		retVal = 0; //Nothing is blocking the Ir-beam
	}
	
	return retVal;
}

