/*
 * rcc.c
 *
 *  Created on: Jan 4, 2025
 *      Author: benjamin
 */


#include "rcc.h"



//Enable clocks
void initRCC(){
	RCC->AHB1ENR=0;
	RCC->APB1ENR=0;
	RCC->APB2ENR=0;
	//GPIO
	RCC->AHB1ENR|= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;
	//USART
	RCC->APB1ENR|= RCC_APB1ENR_USART2EN;
	//TIM
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;//TIM2
	//ADC
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;

}


