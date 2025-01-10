/*
 * usart.c
 *
 *  Created on: Jan 4, 2025
 *      Author: benjamin
 */
#include "usart.h"

uint16_t Baud_Rate;
volatile uint8_t txBuffer[TX_BUFFER_SIZE];
int transmitted;
volatile uint8_t txIndex;
volatile uint8_t txLength;


void initUSART(){

	us2->CR1|=USART_CR1_UE;//ENABLE USART
	us2->CR1|=USART_CR1_TE;//enable transmitter
	us2->CR1|=USART_CR1_RE;//enable Receiver
	us2->CR1|=USART_CR1_RXNEIE; //enable RXNE interrupt only once


	rxIndex=0;
	txIndex=0;
	txLength=0;
	bufferFull=0;
	transmitted=1;
	setBaudRate();
}

void setBaudRate(){
	Baud_Rate=9600;
	us2->BRR=SystemCoreClock/Baud_Rate;//set USARTDIV
}

//Adds one char in Putty terminal
int __io_putchar(int ch){
	while (!(us2->SR & USART_SR_TXE));
	us2->DR =(char) ch;// transmit char
	us2->SR= ~USART_SR_TXE;
	return ch;
}


//Adds a sequence of chars in Putty Terminal
void USART2_Transmit(uint8_t* data,int len){
	while(!transmitted);
	transmitted=0;
	//resetString(txBuffer,txLength);
	int minSize=TX_BUFFER_SIZE;
	if(len<TX_BUFFER_SIZE)minSize=len;
	memcpy(txBuffer,data,minSize);
	txIndex=0;
	txLength=minSize;
	//for (int i = 0; i < txLength; i++) {
	//    printf("txBuffer[%d] = %c\n", i, txBuffer[i]);
	//}
	us2->CR1|= USART_CR1_TXEIE;
}

void USART2_IRQHandler() {
    // End of transmission interrupt
    if (us2->SR & USART_SR_TXE) {
        if (txIndex < txLength-1) {
        	__io_putchar(txBuffer[txIndex++]);
        }
        //disable transmission interrupt if there's no data to be sent
        else {
        	us2->CR1 &= ~USART_CR1_TXEIE;
        	txIndex=0;
        	transmitted=1;
        }
    }
}
