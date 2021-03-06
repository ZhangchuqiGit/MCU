#include <stm32f10x.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "led.h"	
#include "pstwo.h"
#include "motor.h"
#include "math.h"
#include "stdlib.h"
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File：ps2遥控小车
Author：pinggai    Version:1.0     Data:2015/05/30
Description: ps2遥控小车，推动左边摇杆，前后移动，控制小车前进后退的速度
             推动右侧摇杆，左右移动，控制小车转弯的半径，小车转弯控制更
			 加灵活。
**********************************************************/	 	 	  
int main(void)
{								  
	s16 speed = 0,speed1,speed2;	
	u8 flywheel;
	s16 swerve = 0;           //转弯量	  
	//Stm32_Clock_Init(); //系统时钟设置
	delay_init();	     //延时初始化
	uart_init(9600);  //串口1初始化 
	TIM3_PWM_Init(900,0); //arr设定计数器自动重装值   
                                     //psc预分频器不分频,psc=0
	LED_Init();
	PS2_Init();
	M_Init();	   //电机旋转方向控制信号端口初始化	
	while(1)
	{
		if( !PS2_RedLight()) //判断手柄是否为红灯模式，是，指示灯LED点亮
		{
			delay_ms(30);	 //延时很重要不可去
			LED = 0;
			flywheel = PS2_DataKey();	 //手柄按键捕获处理
			//flywheel=PS2_AnologData(PSS_RY);      //R1（飞轮）的按键状态数据在Data[4]中
			//speed = PS2_AnologData(PSS_LY)-127;	   
			//swerve = (PS2_AnologData(PSS_RX)-128)*2.3*((float)abs(speed)/128); //	speed取绝对值，	算数运算，得到转弯量。
			speed = -(PS2_AnologData(PSS_LY)-127)*7;	   //正：前进；  负：后退
			swerve = -(PS2_AnologData(PSS_RX)-127)*7;     //正：左转；  负：右转
		
			  if(flywheel == 9)
					F_1 = 0;
				 if(flywheel == 11)
				 {
					F_2 = 0;
					F_1 = 1;
				 }
				if(flywheel == 10)
					F_2 = 0;
				if(flywheel == 12)
				{
					F_2 = 1;
				  F_1 = 0;
				}
			if(speed >1) //前进
			{
				speed1 = speed2 = speed;
				Motor_Speed_Control(speed1, speed2);
			}
		  else if(speed < -1)//后退
			{
				  speed1 = speed2 = speed;
				  Motor_Speed_Control(speed1, speed2);
			}
			else if(swerve > 1)//左转  
			{
				 if(swerve > 500) swerve = 500;
				 speed1 = swerve;
         speed2 = -swerve;
				 Motor_Speed_Control(speed1, speed2);
			}
			else if(swerve <-1)//右转
			{
				 if(swerve < -500) swerve = -500;
         speed1 = swerve;
				 speed2 = -swerve;
				 Motor_Speed_Control(speed1, speed2);
			}
			else  Motor_Speed_Control(0, 0); //停止
		}
		else	//判断手柄不是红灯模式，指示灯LED熄灭
		{
			LED = 1;
			Motor_Speed_Control(0, 0);
			delay_ms(50);
		}

	}	 
}

























