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

//init CR1 register
void initUSART(){
	us2->CR1|=USART_CR1_UE;//ENABLE USART
	us2->CR1|=USART_CR1_TE;//enable transmitter
//	us2->CR1|=USART_CR1_RE;//enable Receiver
	//We don't enable RXNEIE because we don't need it
	txIndex=0;
	txLength=0;
	transmitted=1;
	setBaudRate();
}





//Set a rate of 9600 symbols/second to transmit
//Didn't work with 115200 with both Putty and function
void setBaudRate(){
	Baud_Rate=9600;
	us2->BRR=SystemCoreClock/Baud_Rate;//set USARTDIV
}

//Adds one char in Putty terminal
//Verify if DR is free
int __io_putchar(int ch){
	while (!(us2->SR & USART_SR_TXE));
	us2->DR =(char) ch;// transmit char
	us2->SR= ~USART_SR_TXE;
	return ch;
}


//Adds a sequence of chars in Putty Terminal
//Activate TXE interruptions for the word
void USART2_Transmit(uint8_t* data,int len){
	while(!transmitted);
	transmitted=0;
	//resetString(txBuffer,txLength);
	int minSize=TX_BUFFER_SIZE;
	if(len<TX_BUFFER_SIZE)minSize=len;
	memcpy(txBuffer,data,minSize);
	txIndex=0;
	txLength=minSize;
	us2->CR1|= USART_CR1_TXEIE;
}

//Usart IRQHandler
//When the data has shifted away from us2->DR TXE interrupt will be called
//in a loop until all characters of the word have been sent
void USART2_IRQHandler() {
    // End of transmission interrupt
    if (us2->SR & USART_SR_TXE) {
        if (txIndex < txLength-1 &&txBuffer[txIndex]!='\0') {
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
