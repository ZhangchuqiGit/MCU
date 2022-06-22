#ifndef TIMER_H
#define TIMER_H
#include "delay.h"

#include "stm32f10x.h"
//#include "stm32f10x_tim.h" //此句删除会引起main函数里START_TIME报错
 
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)


void TIM1_Int_Init(void);
void TIM1_PWM_Init(void); 
void TIM1_Mode_Config(void);

void TIM2_Int_Init(u16 arr,u16 psc);

void TIM3_Int_Init(u16 arr,u16 psc);

void SG90_out(u16 num);
void MG90S_out(u16 num);//500-2500


#endif	/* TIME_TEST_H */
