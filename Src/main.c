/**
 ******************************************************************************
 * @file           : main.c
 * @author         : NL
 * @brief          : Main program body,
 * 					 simple application to test and debug
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "rcc.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"
#include "syscfg.h"
#include "nvic.h"
#include "adc.h"
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

void initMain(){
	initRCC();//done
	initSYSCFG();//not to do
	initGPIO();//done
	initNVIC();//done
	initTIM();
	initUSART();// 3/4 done
	initADC();
}



int main(void)
{
	initMain();

}




