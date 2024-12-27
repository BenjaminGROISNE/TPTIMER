/**
 ******************************************************************************
 * @file           : main.c
 * @author         : NL
 * @brief          : Main program body,
 * 					 simple application to test and debug
 ******************************************************************************
 */
#include "main.h"
#include <stdint.h>
#include "stm32f4xx.h"

void TIM2_init_pwm(void){
	TIM2->PSC=SystemCoreClock/100000 - 1;
	TIM2->ARR= 100000 /1000 -1;
	TIM2->DIER=1;
	NVIC_SetPriority(TIM2_IRQn,5);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CR1|=1;
}

void TIM2_set_pwm(uint32_t duty){
__WFI();
}

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR|=1;
	//PA5 alternate function(01)
	GPIOA->MODER &=~(1<<10);//set 0
	GPIOA->MODER |=1<<11;
	// set afr to af01(0001);
	GPIOA->AFR[0]&=~(0xF<<20);
	GPIOA->AFR[0]|=1<<20;
	TIM2_init_pwm();
	TIM2_set_pwm(5);
}




