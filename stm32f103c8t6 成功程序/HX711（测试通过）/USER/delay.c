/***************STM32F103C8T6**********************
 * 文件名  ：delay.c
 * 描述    ：delay精确延时	
 * 备注    ：采用系统定时器SysTick  

********************LIGEN*************************/

#include "delay.h"

//us延时
void Delay_us(uint32_t time)
{
	SysTick->LOAD = 72*time;//装载计数值，时钟为72M，72次为1us
	SysTick->CTRL = 0x00000005;//时钟来源为HCLK(72M),打开定时器
	while(!(SysTick->CTRL&0x00010000));//等待计数到0
	SysTick->CTRL = 0x00000004;//关闭定时器
}

//ms延时
void Delay_ms(uint32_t time)
{
	for(;time>0;time--)
		Delay_us(1000);
}
