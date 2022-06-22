#ifndef __TIMER_ASMX_PWM_H
#define __TIMER_ASMX_PWM_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "misc.h"
#include "play_music.h"

extern u8 s_val;
extern u8 s;
extern u16 i;
extern u16 j;

/***************   STM32F103C8T6 属于 STM32F10X_MD 
定时器(16位) 有4个 TIM1,TIM2,TIM3,TIM4

	通用定时器(TIM2~TIM5)的主要功能: 除了基本的定时器的功能外，
还具有测量输入信号的脉冲长度( 输入捕获) 或者产生输出波形( 输出比较和PWM)通用定时器的时钟来源;
	a:内部时钟(CK_INT)
	b:外部时钟模式1：外部输入脚(TIx)
	c:外部时钟模式2：外部触发输入(ETR)
	d:内部触发输入(ITRx)：使用一个定时器作为另一个定时器的预分频器
	
通用定时器（TIM2-5）的时钟不是直接来自APB1，而是通过APB1的预分频器以后才到达定时器模块
	当APB1的预分频器系数为1时，这个倍频器就不起作用了，定时器的时钟频率等于APB1的频率；
	当APB1的预分频系数为其它数值(即预分频系数为2、4、8或16)时，这个倍频器起作用，定时器的时钟频率等于APB1时钟频率的两倍
**********************/
//TIM1和TIM8是高级定时器
void TIM1_PWM_Init(u16 arr,u16 psc) ; //舵机的控制

//TIM2-TIM5是通用定时器  
//arr：自动重装值。
//psc：时钟预分频数
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);

//舵机摆动			pwm波高电平持续时间，2ms
void SG90_out(u16 num);//500-2500  --  20 000
void MG90S_out(u16 num);//500-2500  --  20 000

#endif
