#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"
#include "pstwo.h"

#define M1_1 PAout(0)
#define M1_2 PAout(1)

#define M2_1 PAout(2)
#define M2_2 PAout(3)

#define F_1  PAout(4)
#define F_2  PAout(5)
//#define F_COM  PAout(6)


void M_Init(void);	   //�����ת��������źŶ˿ڳ�ʼ��
void TIM3_PWM_Init(u16 arr,u16 psc); //arr�趨�������Զ���װֵ   
                                     //pscԤ��Ƶ������Ƶ,psc=0
//void Motor_Speed_Control(s16 motor1, s16 motor2);  //���ҵ���������ٶȿ���	
/**************        *****************  ���  ����  *****************          *****************/
void Motor__Control(s16 speed  , s16 swerve/*ת����*/);//speed:ǰ��/����   swerve:��ת/��ת 

#endif






