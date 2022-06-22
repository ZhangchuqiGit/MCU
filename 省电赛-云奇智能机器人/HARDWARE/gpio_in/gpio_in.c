/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    : �������
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��
 * �ӿ�    ��PB0

****************STM32F103C8T6**********************/

#include "gpio_in.h"
#include "stm32f10x_gpio.h"
#include "led.h"

#define SPEED GPIO_Pin_1

//io����������
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //���������˿�PB��ʱ��
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //���������˿�PA��ʱ��
	GPIO_InitStructure.GPIO_Pin = SPEED; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //�˿�����Ϊ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ���˿�

}

u8 Rotate(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,SPEED) == 0)
	{
		while(GPIO_ReadInputDataBit(GPIOB,SPEED) == 1)
			return 1;
	}
	else
		return 0;
}



//









