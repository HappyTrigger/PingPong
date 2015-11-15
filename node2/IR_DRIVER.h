

#ifndef _IR_DRIVER_h
#define _IR_DRIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

uint8_t is_IR_interrupted();

#endif

