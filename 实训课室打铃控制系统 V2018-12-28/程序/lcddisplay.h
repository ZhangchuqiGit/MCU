#ifndef __LCDDISPLAY_H_
#define __LCDDISPLAY_H_

//---头文件---//

#include<reg51_typedef.h>
#include"lcd.h"
#include"ds1302.h"
#include"timer.h"

//---定义的IO口---//
extern u8 armhour,armmin,armsec;//闹钟时、分、秒   
extern u8 year,month,date,week;//年、月、日、星期   
extern u8 hour,min,sec;//时、分、秒   
extern u8 code num_buf[6];//光标移动,lcd显示缓冲区   
extern u8 code Alarm_clock_buf[4]; /*光标移动*/ 
extern char code EN_week[8][3]; 
extern u8 Alarm_clock[];//"00:00:00"
extern u8 code para_month[13];  //星期月参变数
extern u8 time_class_s[5][8];
extern u8 time_class_i[8];

//---函数---//

//闰年的计算   
u8 leap_year( void );  
//星期的自动运算和处理   
u8 week_proc(void); 
//主界面
void LcdDisplay(void);
//主界面显示设置
void Lcd_SetState_0(void);
void dis_class(u8 NUM); // 课表时间查看

#endif
