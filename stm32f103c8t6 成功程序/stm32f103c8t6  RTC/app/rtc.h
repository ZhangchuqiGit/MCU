#ifndef __RTC_H
#define __RTC_H	    

#include "includes.h"


//ʱ��ṹ��
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��

extern u8 const mon_table[12];	//�·��������ݱ�
void Disp_Time(u8 x,u8 y,u8 size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//��ָ��λ����ʾ����
u8 RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�

u32 time_2_sec(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u32 time_2_sec_go(u8 sday,u8 hour,u8 min,u8 sec);//��������
u8 sec_2_time(_calendar_obj* time,u32 timecount);
s32 compare_time(_calendar_obj time1,_calendar_obj time2);
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
void RTC_Alarm_Set_after(u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //����ʱ�� 
u8 Time_Get(_calendar_obj* time);
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��			 
#endif


