/***************STM32F103C8T6**********************
 
 * �ļ���  ��main.c
 * ����    ��ģ��
 * ��ע    ��
 * �ӿ�    ��

********************LIGEN*************************/


#include "stm32f10x.h"
#include "led.h"
#include "usart1.h"
#include "delay.h"
#include "i2c.h"
#include "bh1750.h"

//��������
u16  lx;

//������
int main(void)
{ 
	
  SystemInit();//����ϵͳʱ��Ϊ72M	
	LED_GPIO_Config();//led��ʼ��
	USART1_Config();//���ڳ�ʼ��
	Delay_init(72);//Delay��ʼ�� 
	I2C_Init();//i2c��ʼ��
	BH1750_Init();//bh1750��ʼ��
	
	
  while (1)
  {
		lx = BH1750_Read();//��ȡ����ǿ��
		printf("%d \r\n",lx);
		LED_Twinkle();
		Delay_ms(1000);
  }
}



