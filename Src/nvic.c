/*
 * nvic.c
 *
 *  Created on: Jan 9, 2025
 *      Author: benjamin
 */


#include "nvic.h"

void initNVIC(){
	//SysTick_Config(16000);//16 MHz pas utile finalement
	//NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);

	NVIC_SetPriority(USART2_IRQn,2);
	NVIC_SetPriority(TIM2_IRQn,3);
	NVIC_SetPriority(ADC_IRQn,4);

}
