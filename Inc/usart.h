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

#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 128
#define us2 USART2


void initUSART();
void setBaudRate();


void USART2_Transmit(uint8_t data[],int len);
int __io_putchar(int ch);
void USART2_IRQHandler(void);


#endif /* USART_H_ */
