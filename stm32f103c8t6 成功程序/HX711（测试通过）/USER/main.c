/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    ：hx711传感器
 * 备注    ：hx711采集压力传感器放大后传输给
 * 接口    ：参看hx711.c

********************LIGEN*************************/


#include "stm32f10x.h"
#include "led.h"
#include "usart1.h"
#include "delay.h"
#include "hx711.h"

unsigned long count;

int main(void)
{ 
	
  SystemInit();//配置系统时钟为72M	
	LED_GPIO_Config();//led初始化
	USART1_Config();//串口初始化
	hx711_config();//hx711初始化
	
  while (1)
  {
		count = hx711_read();
		printf("%ld \r\n",count);
		LED_Toggle();
		Delay_ms(1000);
  }
}



