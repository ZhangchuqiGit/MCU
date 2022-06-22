#include "stm32f10x.h"
#include "sys.h"
#include "math.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"		

//#include "timer.h"
#include "GPIO_JTAG.h"
#include "ds18b20.h"
#include "led4pin.h"
#include "rtc.h" 		    
//#include "exti.h"

/*-------------------------------------------------------------------*/
int main(void)
{
	u8 haha=0;
	u32 i=0;
	_calendar_obj time1,time2;
	short DS18B20_tmp=0;
	SystemInit();//系统时钟设置,配置系统时钟为72M	
	delay_init(72);//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	JTAG_SWD_GPIO_Config();//不使用JTAG调试，对应的IO口如PB3,PB4,PA15引脚作为普通IO口使用，可以在使用SWD仿真和烧录的情况下释放出来
//	uart1_init(115200);// USART1_TX PA.9     RX PA.10
	
	led4pin_Init();
	DS18B20_Init();
	RTC_Init(DISABLE/*RTC秒中断*/,ENABLE/*RTC闹钟秒中断*/);
//	while(RTC_Init())
//	{
//		printf("INIT Programing is ERROR!!\r\n");
//	}
//	RTC_Set(2020,3,5,1,1,0);//改变时间
	Time_Get(&time1);//定格此刻时间作为初始时间
	
///***************void TIM2_Int_Init(u16 arr,u16 psc);
//中断/溢出  时间 t=  (  72M /(psc+1)  )  *  (  arr+1  )****************/
//	TIM2_Int_Init(1000-1,720-1);// t= 72M / 720  * 1000 =  10 000 us = 10ms 	
	PCout(13)=1;

	while(1) 
	{  
		i++;
		if(i>100000000)i=0;
		
		if(i%6000==0)
		{		
			Time_Get(&time2);//定格此刻时间
			haha=compare_time(time1,time2);//计算time2-time1的秒数		
			if(haha%3==0)
			{
				//printf("**********设置4s后响闹钟**********\r\n");  进入待机模式前执行！！！！
				//RTC_Alarm_Set_after(0,0,0,4);//0天0小时0分4秒后产生闹钟中断,大于一月的用下面一个函数设置更方便
				RTC_Alarm_Set_after(0,0,0,6); //RTC_Alarm_Set(2017,5,22,22,31,5);//设置闹钟 
				
				PWR_EnterSTANDBYMode();//进入待机模式 唤醒延迟		
				//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);//停机模式//电源不进低功耗 唤醒基本 没延迟//中断唤醒
				//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFE);///停机模式//电源不进低功耗 唤醒基本 没延迟//事件唤醒
				//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//停机模式//电源进去低功耗 不过唤醒启动 有一点延迟 //中断唤醒
				//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);///停机模式//电源进去低功耗 不过唤醒启动 有一点延迟 //事件唤醒
			}
		}
		
		if(i%28==0) DS18B20_tmp=DS18B20_Get_Temp( ); //返回值 -12389 = 温度 -123.89
		if(i%51==0)
		{	
			led4pin_Display(2,DS18B20_tmp/1000%10);
			led4pin_Display(3,DS18B20_tmp/100%10);
			led4pin_Display(4,DS18B20_tmp/10%10);			
			if(DS18B20_tmp/100 >=100) led4pin_Display(1,DS18B20_tmp/10000%10);	
			if(DS18B20_tmp<0) led4pin_Display(1,11);//显示 -
			else led4pin_Display(1,15);//显示 无		
		}

	}
}

