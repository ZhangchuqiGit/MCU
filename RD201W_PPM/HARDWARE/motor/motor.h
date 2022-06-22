#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#include "pstwo.h"

#include "GPIO_JTAG.h"

#include "ultrasonic.h"

#define MR_GO PBout(5)//右电机控制 
#define MR_BACK PBout(4)

#define ML_GO PBout(3)//左电机控制
#define ML_BACK PAout(15)

#define speed_max 100

extern u8 flywheel;

void motor_Init(void);//电机控制端口初始化

void PWMA(u16 speedval);/*左电机速度控制 CH1(PA6)*/
void PWMB(u16 speedval);/*左电机速度控制 CH1(PA6)*/

/*	TIM3_PWM_Init (arr, psc);   
设置TIM3_PWM_Init (1000,144);    设定自动装载值为250，不分频。PWM频率=72000/250=80Khz    
中断/溢出  时间 Tout us= (arr*psc)/Tclk
中断/溢出  时间 Tout us=  1000 * 144 / (72 MHz) = 2000 us  */
void TIM3_PWM_Init(u16 arr,u16 psc); //arr设定计数器自动重装值 ;  psc预分频器不分频,psc=0

void Motor_Control(s16 speed, s16 swerve);//speed:前进/后退 , swerve:左转/右转 

void stop(void);//刹车
void stop_Measure(void);//刹车
void stop_Measure_B(void);//刹车

#endif






