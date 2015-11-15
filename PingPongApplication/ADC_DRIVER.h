/*
 * IncFile2.h
 *
 * Created: 10.09.2015 16:16:57
 *  Author: haakongn
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <stdint.h>

#define ADC_CH1 0b00000100 // X axis
#define ADC_CH2 0b00000101 // Y axis
#define ADC_CH3 0b00000110 // Slider right
#define ADC_CH4 0b00000111 // Slider left

uint8_t ADC_read(uint8_t channel);

#endif /* ADC_DRIVER_H_ */