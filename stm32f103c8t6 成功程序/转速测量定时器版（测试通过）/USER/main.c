/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    : �������
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��TIM2�ⲿ������TIM3��ʱ
 * �ӿ�    ��PB0

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
  SystemInit();//����ϵͳʱ��Ϊ72M	
  USART1_Config();//���ô���
	LED_GPIO_Config();//led��ʼ��
	
	TIM2_GPIO_Init();//TIM2����˿�����
	TIM2_Configuration();//TIM2����
	
	TIM3_NVIC_Configuration(); //TIM3�ж����� 
	TIM3_Configuration(); 	//TIM3����

  printf(" -------�������------\r\n");
	
	START_TIME;	 // TIM3 ��ʼ��ʱ 
	
  while (1)
  {
		while(time3 == 1000)//1000ms����ÿ���ж��ٸ�����
		{
			COUN2 = TIM2->CNT;//��ȡTIM2����ֵ
			printf("COUNT = %d \r\n",COUN2);
			LED_Toggle();
			TIM2->CNT = 0;//TIM2����ֵ����
			time3 = 0;//TIM3��ʱ������
		}
  }
}



