#include "SysTick.h"

void Delay_ms(u16 time)
	{
	  SysTick->LOAD=9000*time;							//设置计数器初始值
	  SysTick->VAL=0;									//清空计数器值
	  SysTick->CTRL=0x00000001;					   	    //使能计数，开关中断，设置时钟
	  
	  while(!(SysTick->CTRL&0x00010000));			    //等待计数器减小到0
	  
	  SysTick->CTRL=0;									//清空计数器
	  SysTick->VAL=0;	  							    //关闭计数器
	}

void Delay_us(u16 time)
	{
	  SysTick->LOAD=9*time;								//设置计数器初始值
	  SysTick->VAL=0;									//清空计数器值
	  SysTick->CTRL=0x00000001;					   	    //使能计数，开关中断，设置时钟
	  
	  while(!(SysTick->CTRL&0x00010000));			    //等待计数器减小到0
	  
	  SysTick->CTRL=0;									//清空计数器
	  SysTick->VAL=0;	  							    //关闭计数器
	}
