#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#include "pstwo.h"

#include "GPIO_JTAG.h"

#include "ultrasonic.h"

#define MR_GO PBout(5)//�ҵ������ 
#define MR_BACK PBout(4)

#define ML_GO PBout(3)//��������
#define ML_BACK PAout(15)

#define speed_max 100

extern u8 flywheel;

void motor_Init(void);//������ƶ˿ڳ�ʼ��

void PWMA(u16 speedval);/*�����ٶȿ��� CH1(PA6)*/
void PWMB(u16 speedval);/*�����ٶȿ��� CH1(PA6)*/

/*	TIM3_PWM_Init (arr, psc);   
����TIM3_PWM_Init (1000,144);    �趨�Զ�װ��ֵΪ250������Ƶ��PWMƵ��=72000/250=80Khz    
�ж�/���  ʱ�� Tout us= (arr*psc)/Tclk
�ж�/���  ʱ�� Tout us=  1000 * 144 / (72 MHz) = 2000 us  */
void TIM3_PWM_Init(u16 arr,u16 psc); //arr�趨�������Զ���װֵ ;  pscԤ��Ƶ������Ƶ,psc=0

void Motor_Control(s16 speed, s16 swerve);//speed:ǰ��/���� , swerve:��ת/��ת 

void stop(void);//ɲ��
void stop_Measure(void);//ɲ��
void stop_Measure_B(void);//ɲ��

#endif






