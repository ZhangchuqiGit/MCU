#ifndef __PLAY_MUSIC_H
#define __PLAY_MUSIC_H

#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LobotServoController.h"
#include "timer_asmx_pwm.h"
#include "dht11.h"
#include "ds18b20.h"
#include "I2C_MPU6050.h"
//#include "myimu.h"

extern uint16_t _value[4];
extern u8 mmc;

#if MPU6050_EN

extern	Int16_xyz Accel,Gyro;	   //�����ۺϺ�Ĵ���������
extern	Int16_xyz	Acc_Data_Con;  //�˲���ļ��ٶ�
extern	Float_angle  Att_Angle;	 //ATT�������������̬��
#endif

void sensor_task(void);

void select_num(u8 num);

void voice_do(void);//---------
void voice_go(u8 Times);
void voice_back(u8 Times);
void voice_right(u8 Times);
void voice_left(u8 Times);

void yuying_Android(void);//�����Ի�
void ds18b20(void);
void MQ135(void);
void sensor_0(void);


#endif
