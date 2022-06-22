/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    ��������
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ����·������������
 * �ӿ�    ��PB6��PB7

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
  SystemInit();//����ϵͳʱ��Ϊ72M	
  USART1_Config();//���ô���
	LED_GPIO_Config();//led��ʼ��
	TIM3_NVIC_Configuration();//��ʱ���ж�����
	TIM3_Configuration();//��ʱ������
	Ultrasonic_Config();//�������˿ڳ�ʼ��
	
	START_TIME;//��ʼ��ʱ
	
	while(1)
	{
		while(time3 == 1000)
		{
			time3 = 0;
			Ultrasonic_Measure();//���������
			LED_Toggle();
		}
	}
}



