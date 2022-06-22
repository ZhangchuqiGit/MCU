#include"Time.h"

void NVIC_TimeConfig(void)
{
	/*���嶨ʱ���ж����ȼ�����*/
	NVIC_InitTypeDef NVIC_InitStruct;

	/*�����ж����ȼ�����*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/*�������ȼ�*/
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
 	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
void TIM3_Config(void)
{
	/*�����ʼ����ʱ���ṹ��*/
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	/*�����ж����ȼ�*/
	NVIC_TimeConfig();

	/*�򿪶�ʱ��ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	/*����жϱ�־*/
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	/*���ö�ʱ��*/	
	TIM_InitStruct.TIM_Period=1000;						//��ֵ
	TIM_InitStruct.TIM_Prescaler=8999; 				//Ԥ��Ƶ
	TIM_InitStruct.TIM_ClockDivision=0;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;	//����
	TIM_TimeBaseInit(TIM3,&TIM_InitStruct);

	/*ʹ��TIM�ж�*/
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	/*ʹ�ܶ�ʱ��*/
	TIM_Cmd(TIM3,ENABLE);	 
}
void TIM4_Config(void)
{
	/*�����ʼ����ʱ���ṹ��*/
	TIM_TimeBaseInitTypeDef TIM_InitStruct;

	/*�򿪶�ʱ��ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	/*���ö�ʱ��*/	
	TIM_InitStruct.TIM_Period=65535;						//��ֵ
	TIM_InitStruct.TIM_Prescaler=71; 						//Ԥ��Ƶ
	TIM_InitStruct.TIM_ClockDivision=0;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;		//����
	TIM_TimeBaseInit(TIM4,&TIM_InitStruct);
	/*ʹ�ܶ�ʱ��*/
	//TIM_Cmd(TIM4,ENABLE);	 
}
