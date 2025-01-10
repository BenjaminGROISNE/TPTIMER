/*
 * usart.c
 *
 *  Created on: Jan 4, 2025
 *      Author: benjamin
 */
#include "usart.h"

uint16_t Baud_Rate;
volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxSavedBuffer[RX_BUFFER_SIZE];
volatile uint8_t txBuffer[TX_BUFFER_SIZE];
uint8_t rxIndex;
volatile uint8_t txIndex;
volatile uint8_t txLength;
int bufferFull;


void initUSART(){

	us2->CR1|=USART_CR1_UE;//ENABLE USART
	us2->CR1|=USART_CR1_RXNEIE; //enable RXNE interrupt only once
	us2->CR1|=USART_CR1_TE;//enable transmitter
	us2->CR1|=USART_CR1_RE;//enable Receiver

	rxIndex=0;
	txIndex=0;
	txLength=0;
	bufferFull=0;

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
int __io_putchar(int ch){
	us2->DR = ch;// transmit char
	us2->SR&= ~USART_SR_TXE;
	return ch;
}

//Adds a sequence of chars in Putty Terminal
void USART2_Transmit(uint8_t* data,int len){

	resetString(txBuffer,txLength);
	int minSize=TX_BUFFER_SIZE;
	if(len<TX_BUFFER_SIZE)minSize=len;
	memcpy(txBuffer,data,minSize);
	txIndex=0;
	txLength=minSize;
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
        	memcpy(rxSavedBuffer,rxBuffer,RX_BUFFER_SIZE);
        	bufferFull=1;
        }
    }
}
