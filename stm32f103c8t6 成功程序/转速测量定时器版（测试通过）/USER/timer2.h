#ifndef TIMER2_H
#define TIMER2_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h" //此句删除会引起main函数里START_TIME报错

void TIM2_GPIO_Init(void);
void TIM2_Configuration(void);

#endif	/* TIME_TEST_H */
