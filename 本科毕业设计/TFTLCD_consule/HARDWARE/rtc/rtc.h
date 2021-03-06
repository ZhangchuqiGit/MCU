#ifndef __RTC_H
#define __RTC_H	    

#include "sys.h"
#include "delay.h"

//#define data_change 0xC0B4
#define data_change 0x4456

//时间结构体
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;	

extern _calendar_obj calendar,time001;	//日历结构体

extern u8 const mon_table[12];	//月份日期数据表
void Disp_Time(u8 x,u8 y,u8 size);//在制定位置开始显示时间
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//在指定位置显示星期

//u8 RTC_Init(void);//初始化RTC,返回0,失败;1,成功;
//u8 RTC_Init(u8 RTC_SEC_ENABLE_DISABLE,u8 RTC_IT__ENABLE_DISABLE);  //初始化RTC,返回0,失败;1,成功;
u8 RTC_Init(FunctionalState RTC_SEC_ENABLE_DISABLE,FunctionalState RTC_IT__ENABLE_DISABLE); //初始化RTC,返回0,失败;1,成功;

u8 Is_Leap_Year(u16 year);//平年,闰年判断

u32 time_2_sec(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u32 time_2_sec_go(u8 sday,u8 hour,u8 min,u8 sec);//用于闹钟
u8 sec_2_time(_calendar_obj* time,u32 timecount);
s32 compare_time(_calendar_obj time1,_calendar_obj time2);
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);

//printf("**********设置4s后响闹钟**********\r\n");  进入待机模式前执行！！！！
//RTC_Alarm_Set_after(0,0,0,4);//0天0小时0分4秒后产生闹钟中断,大于一月的用下面一个函数设置更方便
//  先：RTC_Alarm_Set_after(0,0,0,4);  后：PWR_EnterSTANDBYMode();//进入待机模式		
//  后：PWR_EnterSTOPMode(PWR_Regulator,PWR_STOPEntry);//停机模式
/*停机模式，有两个参数 
第一个参数 PWR_Regulator 是选择电源是否进入低功耗
#define PWR_Regulator_ON//电源不进低功耗 唤醒基本没延迟
#define PWR_Regulator_LowPower//电源进去低功耗 不过唤醒启动有一点延迟    
第二个参数 PWR_STOPEntry 
#define PWR_STOPEntry_WFI//中断唤醒
#define PWR_STOPEntry_WFE//事件唤醒  */ 

void RTC_Alarm_Set_after(u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //更新时间 
u8 Time_Get(_calendar_obj* time);
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间			 
#endif


