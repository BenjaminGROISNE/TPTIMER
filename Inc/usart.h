/*
 * usart.h
 *
 *  Created on: Jan 4, 2025
 *      Author: benjamin
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

uint16_t Baud_Rate;
#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 128
volatile uint8_t rxBuffer[RX_BUFFER_SIZE];
volatile uint8_t rxSavedBuffer[RX_BUFFER_SIZE];
volatile uint8_t txBuffer[TX_BUFFER_SIZE];
volatile uint8_t rxIndex = 0;
volatile uint8_t txIndex = 0;
volatile uint8_t txLength = 0;
int bufferFull=0;
#define us2 USART2

char dataBuffer[DATASIZE];
void initUSART();
void setBaudRate();
void initCR1();

void USART2_Transmit(uint8_t * data);
int __io_putchar(int ch);
int __io_getchar();


void sendWord()
void sendChar();
void receive();
void resetString(char word[]);
void USART2_IRQHandler();


#endif /* USART_H_ */
