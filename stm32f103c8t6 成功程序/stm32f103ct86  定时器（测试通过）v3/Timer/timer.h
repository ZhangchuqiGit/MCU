#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h" //此句删除会引起main函数里START_TIME报错
 
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

void Servo_out (void);

void TIM1_Int_Init(void);
void TIM1_PWM_Init(void); 
void TIM1_Mode_Config(void);

void TIM2_Int_Init(u16 arr,u16 psc);

void TIM3_Int_Init(u16 arr,u16 psc);

void Delay_ms(unsigned int i);
void Delay_us(unsigned int i);


#endif	/* TIME_TEST_H */
