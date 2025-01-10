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
	Baud_Rate=115200;
	us2->BRR=SystemCoreClock/Baud_Rate;//set USARTDIV
}

void resetString(char * data,int size){
	memset(data,0,size);
}

int __io_getchar(void){
    uint8_t receivedChar = us2->DR;
    rxBuffer[rxIndex++] = receivedChar;
    return (int)receivedChar;
}



//Adds one char in Putty terminal
void __io_putchar(int ch){
	us2->DR = ch;// transmit char
	us2->SR&= ~USART_SR_TXE;
}

//Adds a sequence of chars in Putty Terminal
void USART2_Transmit(uint8_t * data,int len){
	resetString(txBuffer,txLength);
	txBuffer=data;
	txIndex=0;
	txLength=len;
	us2->CR1|= USART_CR1_TXEIE;
}

void USART2_IRQHandler() {
    // End of transmission interrupt
    if (us2->SR & USART_SR_TXE) {
        if (txIndex < txLength-1) {
        	__io_putchar(txBuffer[txIndex++]);
        }
        //disable transmission interrupt if there's no data to be sent
        else USART1->CR1 &= ~USART_CR1_TXEIE;
    }

    // End of Reception Interrupt
    else if (us2->SR & USART_SR_RXNE) {
        // Read the received character
        // Store the character in the buffer
    	//handle Delay with TIM;
        if (rxIndex < RX_BUFFER_SIZE-1) {
        	__io_getchar();
        }
        else{
        	rxSavedBuffer=rxBuffer;
        	bufferFull=1;
        }
    }
}

void resetString(char word[]){
	memset(word, 0, sizeof(word));
}

void setCR1(){
	us2->CR1|=USART_CR1_UE;//ENABLE USART
	us2->CR1|=USART_CR1_RXNEIE; //enable RXNE interrupt only once
	us2->CR1|=USART_CR1_TE;//enable transmitter
	us2->CR1|=USART_CR1_RE;//enable Receiver
}
