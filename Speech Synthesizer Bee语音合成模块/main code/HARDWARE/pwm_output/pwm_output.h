#ifndef __PWM_OUTPUT_H
#define	__PWM_OUTPUT_H

#include "stm32f10x.h"
#include "delay.h"

void TIM1_GPIO_Config(void) ;
void TIM1_PWM_Init(void);
void Servo_out (void);
#endif /* __PWM_OUTPUT_H */

