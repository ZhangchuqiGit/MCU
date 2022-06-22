#include"LED.h"

void LED_GPIO_Config(void)
{
  /*��������IO�ڵĽṹ��*/
  GPIO_InitTypeDef GPIO_InitStruct;	 

  /*��GPIOA��ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

  /*ѡ����Ҫ���õ�IO��*/
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;

  /*����Ϊ�������*/
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;

  /*����IO������Ϊ50Mhz*/
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  /*����IO��ʼ������*/
  GPIO_Init(GPIOC,&GPIO_InitStruct);
}

