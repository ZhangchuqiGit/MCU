#include "GPIO_JTAG.h"

/* �õ�PB3��PB4��PA15�����ſ������衣���ֲ�����ô���ã����������Ŷ��������㡣
�����ǰ����������������ڵ� PB3��PB4��PA13��PA14��PA15 �������IO�ڣ����� JTAG/SWD �������ĵ��Խӿڡ�
���� 	PA13��PA14 �ֱ���ΪSWD���Ե� SWIO��SWCLK��
		PB3��PB4��PA13��PA14��PA15 ��ͬ����JTAG
�����IO���ŷǳ����⣬�����������ΪSWJ�������ĵ������ţ����Ҫ��Ϊ��ͨIO��ʹ����Ҫ�ر������*/

/**stm32��PB3 PB4 PA15���ſ�����ʹ��SWD�������¼��������ͷų�������Ϊ��ͨIO��ʹ��**/
void JTAG_SWD_GPIO_Config()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE); //ʹ��GPIOxʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOB,GPIO_Pin_3);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOB,GPIO_Pin_4);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);  
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}




