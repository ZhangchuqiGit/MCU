#include"Uitr.h"


void Uitr_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;						//����GPIO��ʼ���ṹ��
								  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//��GPIOA��ʱ��
	
	/*TRIG*/
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;				//ѡ��Ҫ���õĹܽ�
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;			//����GPIOģʽ
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;		//����GPIO�ٶ�
	GPIO_Init(GPIOB,&GPIO_InitStruct);				    //IO��ʼ�� 
	
	/*ECHO*/
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;				//ѡ��Ҫ���õĹܽ�
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;			//����GPIOģʽ
	GPIO_Init(GPIOB,&GPIO_InitStruct);				    //IO��ʼ�� 
}
