/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    : �������
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��ʹ�ö�ʱ��������pwm�������ƶ������
 * �ӿ�    ��PB13�Ӷ���ź�

********************LIGEN*************************/
#include "stm32f10x.h"
#include "pwm_output.h"
#include "led.h"


int main(void)
{
  
	SystemInit(); //����ϵͳʱ��Ϊ72M   
	
	TIM1_PWM_Init(); //TIM1 PWM�������ʼ������ʹ��TIM1 PWM���
	
	Servo_out(); //����ڶ�
	
}




