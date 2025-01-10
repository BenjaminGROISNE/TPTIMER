/*
 * gpio.h
 *
 *  Created on: Jan 9, 2025
 *      Author: benjamin
 */

#ifndef GPIO_H_
#define GPIO_H_
#include "stm32f4xx.h"

GPIO_TypeDef* pa =GPIOA;
GPIO_TypeDef* pb =GPIOB;

void initGPIO();

#endif /* GPIO_H_ */
