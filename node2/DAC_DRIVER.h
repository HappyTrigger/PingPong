// DAC_DRIVER.h

#ifndef _DAC_DRIVER_h
#define _DAC_DRIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
void dac_init();
void dac_write(int8_t value);
#endif

