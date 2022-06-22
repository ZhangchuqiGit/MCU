/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    : 电机测速
 * 实验平台：STM32F103C8T6
 * 备注    ：采用一个定时器定时，外部输入开启定时
 * 接口    ：PB0

********************LIGEN*************************/

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "usart1.h"
#include "led.h"
#include "timer2.h"
#include "misc.h"

u32 time;

int main(void)
{ 
  SystemInit();//配置系统时钟为72M	
  USART1_Config();//配置串口
	LED_GPIO_Config();//led初始化
	
	TIM2_NVIC_Configuration(); // TIM2 定时配置 
	TIM2_Configuration(); 	
	START_TIME;	 // TIM2 开始计时 

 // printf(" -------电机测速------\r\n");
	

  while (1)
  {
		if(time == 100)
		{
			time = 0;
			LED_Twinkle();
		}
  }
}



