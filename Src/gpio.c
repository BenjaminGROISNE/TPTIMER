/*
 * gpio.c
 *
 *  Created on: Jan 9, 2025
 *      Author: benjamin
 */
#include "gpio.h"

void initGPIO(){
	//One pin for adc,and one for TIM2
	//PB0 for adc ; PA5 for tim2 ;

	//Port mode Register
	//analog mode for adc (11)
	pb->MODER &= ~GPIO_MODER_MODER0;
	pb->MODER |= GPIO_MODER_MODER0;

	//Alternate mode for tim2 (10)
	pa->MODER &= ~GPIO_MODER_MODER5_1;
	pa->MODER |= GPIO_MODER_MODER5_1;

	//pull-up/pull-down Register
	pa->PUPDR &= ~GPIO_PUPDR_PUPD5;//NO push pull for PA5 tim (00)
	pb->PUPDR &= ~GPIO_PUPDR_PUPD0;//NO push pull for PB0 adc (00)

	//Output type Register
	pa->OTYPER &= ~GPIO_OTYPER_OT5;//Output Push-pull PA5 TIM (0)
	pb->OTYPER &= ~GPIO_OTYPER_OT0;//Output Push-pull PB0 adc (0)

	//Alternate function low Register
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5_0;  //AF01 alternate function TIM2_CH1 (0001)
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
}
