/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    ��hx711������
 * ��ע    ��hx711�ɼ�ѹ���������Ŵ�����
 * �ӿ�    ���ο�hx711.c

********************LIGEN*************************/


#include "stm32f10x.h"
#include "led.h"
#include "usart1.h"
#include "delay.h"
#include "hx711.h"

unsigned long count;

int main(void)
{ 
	
  SystemInit();//����ϵͳʱ��Ϊ72M	
	LED_GPIO_Config();//led��ʼ��
	USART1_Config();//���ڳ�ʼ��
	hx711_config();//hx711��ʼ��
	
  while (1)
  {
		count = hx711_read();
		printf("%ld \r\n",count);
		LED_Toggle();
		Delay_ms(1000);
  }
}



