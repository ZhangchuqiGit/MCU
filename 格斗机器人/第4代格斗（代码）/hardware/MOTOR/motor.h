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


void M_Init(void);	   //电机旋转方向控制信号端口初始化
void TIM3_PWM_Init(u16 arr,u16 psc); //arr设定计数器自动重装值   
                                     //psc预分频器不分频,psc=0
//void Motor_Speed_Control(s16 motor1, s16 motor2);  //左右电机方向与速度控制	
/**************        *****************  电机  控制  *****************          *****************/
void Motor__Control(s16 speed  , s16 swerve/*转弯量*/);//speed:前进/后退   swerve:左转/右转 

#endif






