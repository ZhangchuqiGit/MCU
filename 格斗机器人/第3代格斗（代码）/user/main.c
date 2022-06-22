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
File��ps2ң��С��
Author��pinggai    Version:1.0     Data:2015/05/30
Description: ps2ң��С�����ƶ����ҡ�ˣ�ǰ���ƶ�������С��ǰ�����˵��ٶ�
             �ƶ��Ҳ�ҡ�ˣ������ƶ�������С��ת��İ뾶��С��ת����Ƹ�
			 ����
**********************************************************/	 	 	  
int main(void)
{								  
	s16 speed = 0,speed1,speed2;	
	u8 flywheel;
	s16 swerve = 0;           //ת����	  
	//Stm32_Clock_Init(); //ϵͳʱ������
	delay_init();	     //��ʱ��ʼ��
	uart_init(9600);  //����1��ʼ�� 
	TIM3_PWM_Init(900,0); //arr�趨�������Զ���װֵ   
                                     //pscԤ��Ƶ������Ƶ,psc=0
	LED_Init();
	PS2_Init();
	M_Init();	   //�����ת��������źŶ˿ڳ�ʼ��	
	while(1)
	{
		if( !PS2_RedLight()) //�ж��ֱ��Ƿ�Ϊ���ģʽ���ǣ�ָʾ��LED����
		{
			delay_ms(30);	 //��ʱ����Ҫ����ȥ
			LED = 0;
			flywheel = PS2_DataKey();	 //�ֱ�����������
			//flywheel=PS2_AnologData(PSS_RY);      //R1�����֣��İ���״̬������Data[4]��
			//speed = PS2_AnologData(PSS_LY)-127;	   
			//swerve = (PS2_AnologData(PSS_RX)-128)*2.3*((float)abs(speed)/128); //	speedȡ����ֵ��	�������㣬�õ�ת������
			speed = -(PS2_AnologData(PSS_LY)-127)*7;	   //����ǰ����  ��������
			swerve = -(PS2_AnologData(PSS_RX)-127)*7;     //������ת��  ������ת
		
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
			if(speed >1) //ǰ��
			{
				speed1 = speed2 = speed;
				Motor_Speed_Control(speed1, speed2);
			}
		  else if(speed < -1)//����
			{
				  speed1 = speed2 = speed;
				  Motor_Speed_Control(speed1, speed2);
			}
			else if(swerve > 1)//��ת  
			{
				 if(swerve > 500) swerve = 500;
				 speed1 = swerve;
         speed2 = -swerve;
				 Motor_Speed_Control(speed1, speed2);
			}
			else if(swerve <-1)//��ת
			{
				 if(swerve < -500) swerve = -500;
         speed1 = swerve;
				 speed2 = -swerve;
				 Motor_Speed_Control(speed1, speed2);
			}
			else  Motor_Speed_Control(0, 0); //ֹͣ
		}
		else	//�ж��ֱ����Ǻ��ģʽ��ָʾ��LEDϨ��
		{
			LED = 1;
			Motor_Speed_Control(0, 0);
			delay_ms(50);
		}

	}	 
}

























