#ifndef __ULTRASONIC3_H
#define	__ULTRASONIC3_H


void Ultrasonic3_Config(void);               //对超声波模块初始化
void Ultrasonic3_Measure(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间

#endif /* __ULTRASONIC_H */
