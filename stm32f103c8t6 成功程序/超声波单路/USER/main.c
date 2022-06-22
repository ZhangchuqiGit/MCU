/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    ：超声波
 * 实验平台：STM32F103C8T6
 * 备注    ：单路超声波距离监测
 * 接口    ：PB6，PB7

********************LIGEN*************************/

#include "stm32f10x.h"
#include "usart1.h" 
#include "led.h"
#include "misc.h"
#include "ultrasonic.h"
#include "timer3.h"

u16 time3;

int main(void)
{ 
  SystemInit();//配置系统时钟为72M	
  USART1_Config();//配置串口
	LED_GPIO_Config();//led初始化
	TIM3_NVIC_Configuration();//定时器中断设置
	TIM3_Configuration();//定时器设置
	Ultrasonic_Config();//超声波端口初始化
	
	START_TIME;//开始计时
	
	while(1)
	{
		while(time3 == 1000)
		{
			time3 = 0;
			Ultrasonic_Measure();//超声波测距
			LED_Toggle();
		}
	}
}



