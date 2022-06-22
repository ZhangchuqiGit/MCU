#ifndef __TIMER_ASMX_PWM_H
#define __TIMER_ASMX_PWM_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "misc.h"
#include "play_music.h"

extern u8 s_val;
extern u8 s;
extern u16 i;
extern u16 j;

/***************   STM32F103C8T6 ���� STM32F10X_MD 
��ʱ��(16λ) ��4�� TIM1,TIM2,TIM3,TIM4

	ͨ�ö�ʱ��(TIM2~TIM5)����Ҫ����: ���˻����Ķ�ʱ���Ĺ����⣬
�����в��������źŵ����峤��( ���벶��) ���߲����������( ����ȽϺ�PWM)ͨ�ö�ʱ����ʱ����Դ;
	a:�ڲ�ʱ��(CK_INT)
	b:�ⲿʱ��ģʽ1���ⲿ�����(TIx)
	c:�ⲿʱ��ģʽ2���ⲿ��������(ETR)
	d:�ڲ���������(ITRx)��ʹ��һ����ʱ����Ϊ��һ����ʱ����Ԥ��Ƶ��
	
ͨ�ö�ʱ����TIM2-5����ʱ�Ӳ���ֱ������APB1������ͨ��APB1��Ԥ��Ƶ���Ժ�ŵ��ﶨʱ��ģ��
	��APB1��Ԥ��Ƶ��ϵ��Ϊ1ʱ�������Ƶ���Ͳ��������ˣ���ʱ����ʱ��Ƶ�ʵ���APB1��Ƶ�ʣ�
	��APB1��Ԥ��Ƶϵ��Ϊ������ֵ(��Ԥ��Ƶϵ��Ϊ2��4��8��16)ʱ�������Ƶ�������ã���ʱ����ʱ��Ƶ�ʵ���APB1ʱ��Ƶ�ʵ�����
**********************/
//TIM1��TIM8�Ǹ߼���ʱ��
void TIM1_PWM_Init(u16 arr,u16 psc) ; //����Ŀ���

//TIM2-TIM5��ͨ�ö�ʱ��  
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);

//����ڶ�			pwm���ߵ�ƽ����ʱ�䣬2ms
void SG90_out(u16 num);//500-2500  --  20 000
void MG90S_out(u16 num);//500-2500  --  20 000

#endif
