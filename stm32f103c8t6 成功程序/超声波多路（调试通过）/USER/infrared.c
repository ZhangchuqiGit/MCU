/***********************************
���ƣ����⴫�������
��ע��B0-B5��·���⴫�������
********************************/

#include "infrared.h"
#include "stm32f10x_gpio.h"
#include "usart1.h"

char Infrared[6];

void Infrared_Config()
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // ʹ��PC�˿�ʱ��  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_4 | GPIO_Pin_5;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PC�˿�
	
}

void Infrared_Scan()
{
	Infrared[0] = 1 - GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
	Infrared[1] = 1 - GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
	Infrared[2] = 1 - GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
	Infrared[3] = 1 - GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
	Infrared[4] = 1 - GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
	Infrared[5] = 1 - GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
	printf("%d_%d_%d_%d_%d_%d_end_",Infrared[0],Infrared[1],Infrared[2],Infrared[3],Infrared[4],Infrared[5]);
}


