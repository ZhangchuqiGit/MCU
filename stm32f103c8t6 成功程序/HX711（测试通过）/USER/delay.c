/***************STM32F103C8T6**********************
 * �ļ���  ��delay.c
 * ����    ��delay��ȷ��ʱ	
 * ��ע    ������ϵͳ��ʱ��SysTick  

********************LIGEN*************************/

#include "delay.h"

//us��ʱ
void Delay_us(uint32_t time)
{
	SysTick->LOAD = 72*time;//װ�ؼ���ֵ��ʱ��Ϊ72M��72��Ϊ1us
	SysTick->CTRL = 0x00000005;//ʱ����ԴΪHCLK(72M),�򿪶�ʱ��
	while(!(SysTick->CTRL&0x00010000));//�ȴ�������0
	SysTick->CTRL = 0x00000004;//�رն�ʱ��
}

//ms��ʱ
void Delay_ms(uint32_t time)
{
	for(;time>0;time--)
		Delay_us(1000);
}
