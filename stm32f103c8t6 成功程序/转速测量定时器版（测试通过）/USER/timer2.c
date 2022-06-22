/***************STM32F103C8T6**********************
 * �ļ���  ��timer2.c
 * ����    : TIM2��ʱ��
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��tim2�ⲿ����ģʽ
 * �ӿ�    ��PA0

********************LIGEN*************************/
#include "timer2.h"
#include "misc.h"

/* TIM2 �ⲿ�˿�����*/
void TIM2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//���ýṹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//���ö˿�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���ö˿�ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//���ö˿�Ƶ��
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}


/*�ж�����Ϊ1ms*/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);//����RCC
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;	//�Զ���װ�ؼĴ������ڵ�ֵ 
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	//ʱ��Ԥ��Ƶ�� 72M/1
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;//  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);//ETR�ⲿ����
	//�ر�Ԥ��ƵTIM_ExtTRGPSC_OFF
	//�ⲿ�������Բ����򣬸ߵ�ƽ����������ЧTIM_ExtTRGPolarity_NonInverted
	//�ⲿ�������˲���
    
	TIM_SetCounter(TIM2,0); //���ü�����ʼֵΪ0
	TIM_Cmd(TIM2, ENABLE);	// ����ʱ��    
}
