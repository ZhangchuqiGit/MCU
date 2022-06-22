#ifndef __ULTRASONIC_H
#define	__ULTRASONIC_H
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "usart.h"	  

#define Trig GPIO_Pin_1 //PA1�� TRIG
#define Echo GPIO_Pin_0 //PA0�� Echo

#define Trig_B GPIO_Pin_6//PA6�� TRIG
#define Echo_B GPIO_Pin_5 //PA5�� Echo

extern u16 Distance; //������ľ���//ǰ�����
extern u16 Distance_B; //������ľ���//�󷽲��

void Ultrasonic_Config(void); //�Գ�����ģ���ʼ��
void Ultrasonic_Measure(void); //��ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��

void Ultrasonic_Measure_B(void);//�󷽲��

#endif /* __ULTRASONIC_H */


