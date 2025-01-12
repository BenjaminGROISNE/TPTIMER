#include "stm32f4xx.h"
#include "tim.h"
/*
 * tim2.c
 *
 *  Created on: Dec 30, 2024
 *      Author: nathan & benjamin
 */

void TIM2_init_pwm(void)
{
	//We begin With SystemCoreClock at 16 Mhz
	//We want a final frequency of 100 Hz
	//So we want 100 Hz = (16 000 000 / PSC) / ARR
	//So either (PSC:16 | ARR:10000)
	//or (PSC:160 | ARR:1000) or (PSC:1600 | ARR:100) or (PSC:1600 | ARR:100) ...

	    TIM2->PSC = 160 - 1;
	    TIM2->ARR = 1000 - 1;
	    TIM2->CCR1 = 0;
	    TIM2->CCMR1 |= 3 <<5;  // PWM mode 1
	    TIM2->CCER |= TIM_CCER_CC1E;   // Activer TIM2 CH1
	    TIM2->CCER &= ~TIM_CCER_CC1P;   // OC1 active high

	    TIM2->CR2 |= TIM_CR2_MMS_1;  // Activate TRGO
	    TIM2->CR1 |= TIM_CR1_CEN;      // Activate TIM2
}


//change Capture Compare register 1 for output mode PWM
void TIM2_set_pwm(uint16_t duty)
{
	if(duty<999)
	{
		tim2->CCR1 = duty; // Set duty Cycle
	}
}

//TIM2 IRQHandler
//Nothing to do TRGO already activated whenever there's an auto reload
void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;

    }
}
