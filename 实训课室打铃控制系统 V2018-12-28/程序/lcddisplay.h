#ifndef __LCDDISPLAY_H_
#define __LCDDISPLAY_H_

//---ͷ�ļ�---//

#include<reg51_typedef.h>
#include"lcd.h"
#include"ds1302.h"
#include"timer.h"

//---�����IO��---//
extern u8 armhour,armmin,armsec;//����ʱ���֡���   
extern u8 year,month,date,week;//�ꡢ�¡��ա�����   
extern u8 hour,min,sec;//ʱ���֡���   
extern u8 code num_buf[6];//����ƶ�,lcd��ʾ������   
extern u8 code Alarm_clock_buf[4]; /*����ƶ�*/ 
extern char code EN_week[8][3]; 
extern u8 Alarm_clock[];//"00:00:00"
extern u8 code para_month[13];  //�����²α���
extern u8 time_class_s[5][8];
extern u8 time_class_i[8];

//---����---//

//����ļ���   
u8 leap_year( void );  
//���ڵ��Զ�����ʹ���   
u8 week_proc(void); 
//������
void LcdDisplay(void);
//��������ʾ����
void Lcd_SetState_0(void);
void dis_class(u8 NUM); // �α�ʱ��鿴

#endif
