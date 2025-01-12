/*
 * nvic.c
 *
 *  Created on: Jan 9, 2025
 *      Author: benjamin
 */


#include "nvic.h"

void initNVIC(){

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
	//USART2 > TIM2 > ADC
	NVIC_SetPriority(USART2_IRQn,2);
	NVIC_SetPriority(TIM2_IRQn,3);
	NVIC_SetPriority(ADC_IRQn,4);

}
