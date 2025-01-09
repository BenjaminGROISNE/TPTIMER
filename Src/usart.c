/*
 * usart.c
 *
 *  Created on: Jan 4, 2025
 *      Author: benjamin
 */
#include "usart.h"


void initUSART(){

	initCR1();
	setBaudRate();
}

void setBaudRate(){
	uint16_t Baud_Rate=115200;
	us2->BRR=SystemCoreClock/115200;//set USARTDIV
}

void USART2_IRQHandler(){

}



void setCR1(){
	us2->CR1|= USART_CR1_UE;//ENABLE USART
	us2->CR1|=USART_CR1_TXEIE;//enable TXE interrupt
	us2->CR1|=USART_CR1_RXNEIE; //enable RXNE interrupt
	us2->CR1|=USART_CR1_TE;//enable transmitter
	us2->CR1|=USART_CR1_RE;//enable Receiver
	#undef ucr1
}
