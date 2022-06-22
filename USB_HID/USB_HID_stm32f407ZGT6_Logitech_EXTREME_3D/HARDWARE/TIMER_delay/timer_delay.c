#include "timer_delay.h"

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
/* STM32F407 ------ ʹ�ö�ʱ��ʵ�־�ȷ��ʱ
���Ի�������Ƶ168M 
��ʱ�� TIM4 ʱ�� 84M����Ƶϵ�� 0������ 84M �ļ���Ƶ�ʣ�����84��Ϊ1us */
void TIM4_init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  ///ʹ��ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 	//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);//��ʼ��
	
	TIM_Cmd(TIM4, DISABLE); //��ʹ�ܶ�ʱ��
}

void timer_delay_init(void)
{
	/* ��ʱ�� TIM4 ʱ�� 84M����Ƶϵ�� 0������ 84M �ļ���Ƶ�ʣ�����84��Ϊ1us */
	TIM4_init(84-1, 0);
}

//��ʱnms
//nusΪҪ��ʱ�� ms ��.                                               
void timer_delay_ms(u32 nms)
{    
    nms = nms*1000 - 1;
    TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��
    while(nms--)
    {
        while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
    TIM_Cmd(TIM4, DISABLE); //��ʹ�ܶ�ʱ��       
}   

//��ʱnus
//nusΪҪ��ʱ��us��.                                               
void timer_delay_us(u32 nus)
{
    TIM_Cmd(TIM4, ENABLE); //ʹ�ܶ�ʱ��
    while(nus--)
    {
        while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
    TIM_Cmd(TIM4, DISABLE); //��ʹ�ܶ�ʱ��
}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}
