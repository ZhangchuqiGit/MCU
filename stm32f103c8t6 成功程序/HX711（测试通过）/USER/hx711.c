/***************STM32F103C8T6**********************
 * �ļ���  ��hx711.c
 * ����    ��hx711���ش�������Ӧ�ķŴ�ADת��
 * �ӿ�    ��PB13-DT  PB12-SCK

********************LIGEN*************************/

#include "stm32f10x.h"
#include "hx711.h"
#include "delay.h"

#define DT GPIO_Pin_13
#define SCK GPIO_Pin_12

//��ʼ��hx711��Ӧ�ӿ�
void hx711_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	//����DT�˿�
	GPIO_InitStructure.GPIO_Pin = DT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//����SCK�˿�
	GPIO_InitStructure.GPIO_Pin = SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//��ȡ����������
unsigned long hx711_read()
{
	unsigned long num = 0;
	unsigned char i;
	
	GPIO_SetBits(GPIOB,DT);//DT��1
	Delay_us(1);
	GPIO_ResetBits(GPIOB,SCK);//SCK��0
	Delay_us(1);
	while(GPIO_ReadInputDataBit(GPIOB,DT));//ת�����
	Delay_us(1);
	//��ȡ����
	for(i=0;i<24;i++)//128������
	{
		GPIO_SetBits(GPIOB,SCK);
		num = num<<1;
		Delay_us(1);
		GPIO_ResetBits(GPIOB,SCK);
		Delay_us(1);
		if(GPIO_ReadInputDataBit(GPIOB,DT))
			num++;
	}
	
	GPIO_SetBits(GPIOB,SCK);//��25�������������ת��
	num = num^0x800000;
	Delay_us(1);
	GPIO_ResetBits(GPIOB,SCK);
	Delay_us(1);
	return num;
}


