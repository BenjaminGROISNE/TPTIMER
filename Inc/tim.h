/*
 * tim2.h
 *
 *  Created on: Dec 30, 2024
 *      Author: nathan
 */

#ifndef TIM_H_
#define TIM_H_
#include "stm32f401xe.h"

#define tim2 TIM2

void TIM2_init_pwm(void);
void TIM2_set_pwm(uint16_t duty);
void TIM2_IRQHandler(void);

#endif /* INC_TIM2_H_ */
