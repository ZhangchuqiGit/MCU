#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "oled.h"
#include "bmp.h"
#include "rtc.h"
#include "dht11.h"

/*      TX     RX
����1   PA9   PA10
����2   PA2   PA3      
����3   PB10  PB11

2020/1/10*/

int main(void)
{		

	u8 t=0;
	u8 temp;
	u8 humi;
	delay_init();	    	 //��ʱ������ʼ��	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	OLED_Init();
	OLED_ColorTurn(0);
	OLED_DisplayTurn(0);
	OLED_Refresh();
	
	RTC_Init();
	//RTC_Set(2020,1,10,11,22,00);//����ʱ��
 
	while(1)
	{
	 OLED_ShowChinese(0,0,0,16);
	 OLED_ShowChinese(18,0,1,16);
	 OLED_ShowString(10,18,"    -  -  ",16);
	 OLED_ShowString(45,0,"  :  :  ",16);
	 
	 if(t!=calendar.sec)
	 {
		 t=calendar.sec;
		 OLED_ShowNum(10,18,calendar.w_year,4,16);
		 OLED_ShowNum(51,18,calendar.w_month,2,16);
		 OLED_ShowNum(75,18,calendar.w_date,2,16);
		 
		 switch(calendar.week)
		{
			case 0:
				OLED_ShowString(35,36,"Sunday   ",16);
				break;
			case 1:
				OLED_ShowString(35,36,"Monday   ",16);
				break;
			case 2:
				OLED_ShowString(35,36,"Tuesday  ",16);
				break;
			case 3:
				OLED_ShowString(35,36,"Wednesday",16);
				break;
			case 4:
				OLED_ShowString(35,36,"Thursday ",16);
				break;
			case 5:
				OLED_ShowString(35,36,"Friday   ",16);
				break;
			case 6:
				OLED_ShowString(35,36,"Saturday ",16);
				break;  
		}
		OLED_ShowNum(45,0,calendar.hour,2,16);
		OLED_ShowNum(69,0,calendar.min,2,16);
		OLED_ShowNum(93,0,calendar.sec,2,16);
		OLED_Refresh();
		}		
	}	 
}
