/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    ：监测7路超声波传感器和6路红外
 * 实验平台：STM32F103C8T6
 * 备注    ：2017.01.08
 * 接口    ：PA口除了串口占用的PA9、PA10外，其他口都作为超声波接口
             PB0-PB5作为六路红外的接口

********************LIGEN*************************/

#include "stm32f10x.h"
#include "usart1.h" 
#include "led.h"
#include "Time_test.h"
#include "misc.h"
#include "ultrasonic.h"

extern unsigned short int Distance; 

int main(void)
{ 
  SystemInit();//配置系统时钟为72M	
  USART1_Config();//配置串口
	LED_GPIO_Config();//led初始化
	
	TIM2_NVIC_Configuration(); // TIM2 定时配置 
	TIM2_Configuration(); 	
	Ultrasonic_Config();//超声波端口初始化
	
  printf(" -------超声波------\r\n");
	while(1)
	{		
		if(Distance < 100)
			
		Ultrasonic_Measure();
		Delayms(500);
		LED_Toggle();
	}
}



