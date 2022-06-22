/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    : 电机测速
 * 实验平台：STM32F103C8T6
 * 备注    ：TIM2外部计数，TIM3定时
 * 接口    ：PB0

********************LIGEN*************************/

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "usart1.h"
#include "led.h"
#include "timer2.h"
#include "timer3.h"
#include "misc.h"

u16 time3;
u16 COUN2 = 0;

int main(void)
{ 
  SystemInit();//配置系统时钟为72M	
  USART1_Config();//配置串口
	LED_GPIO_Config();//led初始化
	
	TIM2_GPIO_Init();//TIM2输入端口配置
	TIM2_Configuration();//TIM2配置
	
	TIM3_NVIC_Configuration(); //TIM3中断配置 
	TIM3_Configuration(); 	//TIM3配置

  printf(" -------电机测速------\r\n");
	
	START_TIME;	 // TIM3 开始计时 
	
  while (1)
  {
		while(time3 == 1000)//1000ms，即每秒有多少个脉冲
		{
			COUN2 = TIM2->CNT;//读取TIM2计数值
			printf("COUNT = %d \r\n",COUN2);
			LED_Toggle();
			TIM2->CNT = 0;//TIM2计数值清零
			time3 = 0;//TIM3定时器清零
		}
  }
}



