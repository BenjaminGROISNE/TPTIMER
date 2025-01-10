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
#include "main.h"


void initMain(){
	initRCC();//done
	initNVIC();//done
	initSYSCFG();//not to do
	initGPIO();//done
	initTIM();
	initUSART();// 3/4 done
	initADC();
}



int main(void)
{
	initMain();
	__enable_irq();
	while(1){

	}
}




