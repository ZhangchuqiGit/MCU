#ifndef __ULTRASONIC_H
#define	__ULTRASONIC_H
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "usart.h"	  

#define Trig GPIO_Pin_1 //PA1接 TRIG
#define Echo GPIO_Pin_0 //PA0接 Echo

#define Trig_B GPIO_Pin_6//PA6接 TRIG
#define Echo_B GPIO_Pin_5 //PA5接 Echo

extern u16 Distance; //计算出的距离//前方测距
extern u16 Distance_B; //计算出的距离//后方测距

void Ultrasonic_Config(void); //对超声波模块初始化
void Ultrasonic_Measure(void); //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间

void Ultrasonic_Measure_B(void);//后方测距

#endif /* __ULTRASONIC_H */


