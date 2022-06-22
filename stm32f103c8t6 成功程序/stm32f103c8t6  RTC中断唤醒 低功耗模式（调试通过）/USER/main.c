#include "stm32f10x.h"
#include "sys.h"
#include "math.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"		

//#include "timer.h"
#include "ds18b20.h"
#include "led4pin.h"
#include "rtc.h" 		    
//#include "exti.h"


/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
int main(void)
{
	u8 haha=0,i=0;
	_calendar_obj time1,time2;
	short DS18B20_tmp=0;
	SystemInit();//系统时钟设置,配置系统时钟为72M	
	delay_init(72);//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//	JTAG_SWD_GPIO_Config();//不使用JTAG调试，对应的IO口如PB3,PB4,PA15引脚作为普通IO口使用，可以在使用SWD仿真和烧录的情况下释放出来
//	uart1_init(115200);// USART1_TX PA.9     RX PA.10
	
	led4pin_Init();
	DS18B20_Init();
	RTC_Init();
//	while(RTC_Init())
//	{
//		printf("INIT Programing is ERROR!!\r\n");
//	}
	RTC_Set(2018,5,22,0,0,0);//设置时间
	RTC_Set(2020,3,5,1,1,0);//改变时间
	Time_Get(&time1);//定格此刻时间作为初始时间
	
	while(1) 
	{   	
		Time_Get(&time2);//定格此刻时间
		haha=compare_time(time1,time2);//计算time2-time1的秒数		
//		if(haha%3==0)
//		{
//printf("**********设置4s后响闹钟**********\r\n");  进入待机模式前执行！！！！
//RTC_Alarm_Set_after(0,0,0,4);//0天0小时0分4秒后产生闹钟中断,大于一月的用下面一个函数设置更方便
//			RTC_Alarm_Set_after(0,0,0,3);
			//RTC_Alarm_Set(2017,5,22,22,31,5);  //设置闹钟 
			
led4pin_Display(4,6);
			RTC_Alarm_Set_after(0,0,0,3);
				PWR_EnterSTANDBYMode();//进入待机模式		
			
/*停机模式，有两个参数 
第一个参数 PWR_Regulator 是选择电源是否进入低功耗模式
#define PWR_Regulator_ON//电源不进低功耗 唤醒基本没延迟
#define PWR_Regulator_LowPower//电源进去低功耗 不过唤醒启动有一点延迟    
第二个参数 PWR_STOPEntry 
#define PWR_STOPEntry_WFI//中断唤醒
#define PWR_STOPEntry_WFE//事件唤醒  */ 
//led4pin_Display(2,2);
//			RTC_Alarm_Set_after(0,0,0,3);
//				PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);//停机模式//电源不进低功耗 唤醒基本没延迟//中断唤醒


//led4pin_Display(3,3);
//			RTC_Alarm_Set_after(0,0,0,3);
//				PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFE);///停机模式//电源不进低功耗 唤醒基本没延迟//事件唤醒

//led4pin_Display(4,4);
//			RTC_Alarm_Set_after(0,0,0,3);
//				PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//停机模式//电源进去低功耗 不过唤醒启动有一点延迟    //中断唤醒


//RTC_Alarm_Set_after(0,0,0,3);
//				PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);///停机模式//电源进去低功耗 不过唤醒启动有一点延迟   //事件唤醒

//		}
			
//		i++;
//		if(i>=10)//50 000*0.1us=5ms
//		{
//			i=0;
//			DS18B20_tmp=DS18B20_Get_Temp( ); //返回值 -12389 = 温度 -123.89
//		}
//		if(1)
//		{	
//			led4pin_Display(2,DS18B20_tmp/1000%10);
//			led4pin_Display(3,DS18B20_tmp/100%10);
//			led4pin_Display(4,DS18B20_tmp/10%10);			
//			if(DS18B20_tmp<0) led4pin_Display(1,11);//显示 -
//			else led4pin_Display(1,15);//显示 无		
//			if(DS18B20_tmp/100 >=100) led4pin_Display(1,DS18B20_tmp/10000%10);	
//		}	  
	}
}

