/*
 * tim.c
 *
 *  Created on: Jan 9, 2025
 *      Author: benjamin
 */

#include "tim.h"

TIM_TypeDef * tim2=TIM2;


void TIM2_init_pwm(void){
	    TIM2->PSC = 160 - 1;      // Prescaler (16 MHz / 16000 = 1 kHz)
		TIM2->ARR = 1000 - 1;     // Période (100 Hz)
		TIM2->CCR1 = 0;           // Rapport cyclique initial
	    //we want a 10 ms cycle
	    //So a frequency of 100 Hz
	    // CNT input clock (desired counter clock = 100kHz)
	    uint32_t Frequency = SystemCoreClock;
	    //1 second to count to 16M with prescaler equal to 1;
	    // Prescaler 100kHz


	tim2->CR1=0;
	tim2->CR2=0;
	tim2->CR1&=~TIM_CR1_DIR;// Upcounting Direction
	tim2->CR1|= TIM_CR1_CEN;//Counter enable
	tim2->CR2|=TIM_CR2_MMS_0;//Update mode TRGO activated everytime ARR overflows

	//Note: The clock of the slave timer and ADC must be enabled prior to receiving events from
	//the master timer, and must not be changed on-the-fly while triggers are received from
//	the master timer.
	tim2->CR2&= ~TIM_CR2_TI1S;//Select TI1 for CH1;

	tim2->DIER|=TIM_DIER_CC1IE;// CC1 interrupt enable

	tim2->CCMR1|=6<<4;//PWM MODE 1 channel 1 active when CNT < CCR1

	tim2->CCER|=TIM_CCER_CC1E;//Activate OC1

	tim2->CCER&=~TIM_CCER_CC1P;//Active High

	tim2->DIER|=TIM_DIER_UIE;//Update Interrupt enable UIF flag


}


void TIM2_set_pwm(uint32_t duty) {
    if (duty > 999) {
        duty = 999; // Ensure duty cycle is within valid range
    }

    // Set the capture/compare register value
    TIM2->CCR1 = duty;
}

void TIM2_IRQHandler(){

	//UIF useful ?
	if(tim2->SR & TIM_SR_CC1IF){

		tim2->SR&=~TIM_SR_CC1IF;
	}
	if (tim2->SR & TIM_SR_UIF) {       // Vérifier l'overflow
		tim2->SR &= ~TIM_SR_UIF;       // Réinitialiser le drapeau d'interruption

	}
}

void initTIM(){

	TIM2_init_pwm();

}
