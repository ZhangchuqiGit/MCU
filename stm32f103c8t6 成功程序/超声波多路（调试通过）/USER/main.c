/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    �����7·��������������6·����
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��2017.01.08
 * �ӿ�    ��PA�ڳ��˴���ռ�õ�PA9��PA10�⣬�����ڶ���Ϊ�������ӿ�
             PB0-PB5��Ϊ��·����Ľӿ�

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
  SystemInit();//����ϵͳʱ��Ϊ72M	
  USART1_Config();//���ô���
	LED_GPIO_Config();//led��ʼ��
	
	TIM2_NVIC_Configuration(); // TIM2 ��ʱ���� 
	TIM2_Configuration(); 	
	Ultrasonic_Config();//�������˿ڳ�ʼ��
	
  printf(" -------������------\r\n");
	while(1)
	{		
		if(Distance < 100)
			
		Ultrasonic_Measure();
		Delayms(500);
		LED_Toggle();
	}
}



