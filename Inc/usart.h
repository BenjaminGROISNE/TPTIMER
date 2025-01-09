/*
 * usart.h
 *
 *  Created on: Jan 4, 2025
 *      Author: benjamin
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f4xx.h"
#define us2 USART2

void initUSART();
void setBaudRate();
void initCR1();




#endif /* USART_H_ */
