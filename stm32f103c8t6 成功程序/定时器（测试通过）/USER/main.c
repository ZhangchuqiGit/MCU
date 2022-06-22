/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    : �������
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ������һ����ʱ����ʱ���ⲿ���뿪����ʱ
 * �ӿ�    ��PB0

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
  SystemInit();//����ϵͳʱ��Ϊ72M	
  USART1_Config();//���ô���
	LED_GPIO_Config();//led��ʼ��
	
	TIM2_NVIC_Configuration(); // TIM2 ��ʱ���� 
	TIM2_Configuration(); 	
	START_TIME;	 // TIM2 ��ʼ��ʱ 

 // printf(" -------�������------\r\n");
	

  while (1)
  {
		if(time == 100)
		{
			time = 0;
			LED_Twinkle();
		}
  }
}



