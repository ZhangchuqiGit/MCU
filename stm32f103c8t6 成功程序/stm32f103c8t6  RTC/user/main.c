
#include "includes.h"

int main()
{	
	u8 t=0,haha;	
	_calendar_obj time1,time2;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化

	USART1_Init(115200,0);	 //串口初始化为115200
	printf("ok\r\n"); 		
	RTC_Init();	  			//RTC初始化 
	Time_Get(&time1);
	//printf("time1:%d-%d-%d  %d:%d:%d  %d\r\n",time1.w_year,time1.w_month,time1.w_date,time1.hour,time1.min,time1.sec,time1.week);

	while(1)
	{								    
		if(t!=calendar.sec)
		{
			t=calendar.sec;
			printf("%4d-%02d-%02d %02d:%02d:%02d 周%d ",calendar.w_year,calendar.w_month,calendar.w_date,
												 calendar.hour,calendar.min,calendar.sec,calendar.week);									  
			Time_Get(&time2);
		  //printf("time2:%d-%d-%d  %d:%d:%d  周%d\r\n",time2.w_year,time2.w_month,time2.w_date,time2.hour,time2.min,time2.sec,time2.week);
			haha=compare_time(time1,time2);//计算time2-time1的秒数
			printf("运行:%3ds\r\n",haha);
			if(haha%10==0)
			{
				printf("**********设置3s后响闹钟**********\r\n");
				RTC_Alarm_Set_after(0,0,0,3);//0天0小时0分3秒后产生闹钟中断,大于一月的用下面一个函数设置更方便
				//RTC_Alarm_Set(2017,5,22,22,31,5);  //设置闹钟 
			}
			if(haha%15==0)
			{
				printf("*************设置时间*************\r\n");
				RTC_Set(2018,5,22,0,0,0);//改变时间
				Time_Get(&time1);
			}							
		} 
	}
}
/*通过更改宏定义还切换两种闹钟中断方案#define RTCAlarm_Way 1  //0中断线方案，1 原子方案
注意事项：
（1）每次重新配置rtc需要更改  #define data_change 0x3456  
       配置成功以后设置时间不需要更改    #define data_change 0x3456    直接用下面两个函数就可以了
       RTC_Set(2017,5,22,11,14,0);        //设置时间
       RTC_Alarm_Set(2017,5,22,11,14,5);  //设置闹钟
（2）rtc时间走时不准的时候可以在32.768K的晶振引脚上并个上1M(>1m也可以)的电阻，一般能解决
（3）#define RTCAlarm_Way 1  用查询方式进入闹钟中断
       原子的例子少了两句RTC_ITConfig(RTC_IT_ALR, ENABLE);   //使能RTC闹钟秒中断   没法进入中断
       #define RTCAlarm_Way 0  开启中断线17中断线方案进入闹钟中断*/