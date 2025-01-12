/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Benjamin
 * @brief          : Main program body,
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "rcc.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"
#include "nvic.h"
#include "adc.h"
#include "main.h"



void initMain(){
	initRCC();//done
	initNVIC();//done
	initGPIO();//done
	initADC();
	TIM2_init_pwm();
	initUSART();// 3/4 done

}



int main(void)
{
	initMain();
	while(1){
		//Enter sleep mode
		__WFI();
	}
}




