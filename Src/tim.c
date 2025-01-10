/*
 * tim.c
 *
 *  Created on: Jan 9, 2025
 *      Author: benjamin
 */

#include "tim.h"

TIM_TypeDef * tim2=TIM2;


void TIM2_IRQHandler(){

	//UIF useful ?
	if(tim2->SR & TIM_SR_CC1IF){
		tim2->SR&=~TIM_SR_CC1IF;
	}
}

void initTIM(){
	tim2->CR1&=~TIM_CR1_DIR;// Upcounting Direction
	tim2->CR1|= TIM_CR1_CEN;//Counter enable

	tim2->CR2&= ~TIM_CR2_TI1S;
	//MMS setup maybe ?
	tim2->DIER|=TIM_DIER_CC1IE;// CC1 interrupt enable

	tim2->CCMR1&=~(7<<4);//reset bits
	tim2->CCMR1|=6<<4;//PWM MODE 1 channel 1 active when CNT < CCR1

	tim2->CCER|=TIM_CCER_CC1E;//Activate OC1

	tim2->CCER&=~TIM_CCER_CC1P;//Active High

	//tim2->DIER|=TIM_DIER_UIE;//Update Interrupt enable not necessary
}
