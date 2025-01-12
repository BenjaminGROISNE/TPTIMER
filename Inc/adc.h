/*
 * adc.h
 *
 *  Created on: Dec 30, 2024
 *      Author: Nathan & Benjamin
 */

#ifndef ADC_H_
#define ADC_H_
#include "stm32f401xe.h"
#include "tim.h"
#define adc ADC1
#define adcCom ADC1_COMMON
extern uint16_t ADC_value[2];
extern char read_temp;

void initADC();
uint16_t ADC_read();

float calculate_L(float Y, float Yn);


#endif /* ADC_H_*/
