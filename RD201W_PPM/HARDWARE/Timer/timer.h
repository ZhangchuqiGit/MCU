#ifndef TIMER_H
#define TIMER_H
#include "delay.h"

#include "stm32f10x.h"
//#include "stm32f10x_tim.h" //�˾�ɾ��������main������START_TIME����
 
#if 0
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

void TIM1_Int_Init(void);
void TIM1_PWM_Init(void); 
void TIM1_Mode_Config(void);

void TIM2_Int_Init(u16 arr,u16 psc);

void TIM3_Int_Init(u16 arr,u16 psc);
#endif

/* ��ʱ��2 ͨ��2 (PA1) ���� PPM �������� */
extern u16 ppm_rx[];
//TIM2_Capture_PPM_Init(0XFFFF,72-1);	//��1Mhz��Ƶ�ʼ��� 
//void TIM2_Capture_PPM_Init(u16 arr,u16 psc); // ���� ң���� PPM ��ͨ��
void TIM2_Capture_PPM_Init(); // ���� ң���� PPM ��ͨ��

void TIM4_Int_Init(void);
void TIM4_PWM_SG90_MG90_Init(void); 
void TIM4_Mode_Config(void);

//����ڶ�
void SG90_MG90_1(u16 num);//500-2500
//����ڶ�
void SG90_MG90_2(u16 num);//500-2500
//����ڶ�
void SG90_MG90_3(u16 num);//500-2500
//����ڶ�
void SG90_MG90_4(u16 num);//500-2500

#endif	/* TIME_TEST_H */
