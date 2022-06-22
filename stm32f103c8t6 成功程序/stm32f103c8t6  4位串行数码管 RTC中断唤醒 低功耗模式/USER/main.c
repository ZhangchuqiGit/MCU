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
	SystemInit();//ϵͳʱ������,����ϵͳʱ��Ϊ72M	
	delay_init(72);//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	JTAG_SWD_GPIO_Config();//��ʹ��JTAG���ԣ���Ӧ��IO����PB3,PB4,PA15������Ϊ��ͨIO��ʹ�ã�������ʹ��SWD�������¼��������ͷų���
//	uart1_init(115200);// USART1_TX PA.9     RX PA.10
	
	led4pin_Init();
	DS18B20_Init();
	RTC_Init(DISABLE/*RTC���ж�*/,ENABLE/*RTC�������ж�*/);
//	while(RTC_Init())
//	{
//		printf("INIT Programing is ERROR!!\r\n");
//	}
//	RTC_Set(2020,3,5,1,1,0);//�ı�ʱ��
	Time_Get(&time1);//����˿�ʱ����Ϊ��ʼʱ��
	
///***************void TIM2_Int_Init(u16 arr,u16 psc);
//�ж�/���  ʱ�� t=  (  72M /(psc+1)  )  *  (  arr+1  )****************/
//	TIM2_Int_Init(1000-1,720-1);// t= 72M / 720  * 1000 =  10 000 us = 10ms 	
	PCout(13)=1;

	while(1) 
	{  
		i++;
		if(i>100000000)i=0;
		
		if(i%6000==0)
		{		
			Time_Get(&time2);//����˿�ʱ��
			haha=compare_time(time1,time2);//����time2-time1������		
			if(haha%3==0)
			{
				//printf("**********����4s��������**********\r\n");  �������ģʽǰִ�У�������
				//RTC_Alarm_Set_after(0,0,0,4);//0��0Сʱ0��4�����������ж�,����һ�µ�������һ���������ø�����
				RTC_Alarm_Set_after(0,0,0,6); //RTC_Alarm_Set(2017,5,22,22,31,5);//�������� 
				
				PWR_EnterSTANDBYMode();//�������ģʽ �����ӳ�		
				//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);//ͣ��ģʽ//��Դ�����͹��� ���ѻ��� û�ӳ�//�жϻ���
				//PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFE);///ͣ��ģʽ//��Դ�����͹��� ���ѻ��� û�ӳ�//�¼�����
				//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//ͣ��ģʽ//��Դ��ȥ�͹��� ������������ ��һ���ӳ� //�жϻ���
				//PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);///ͣ��ģʽ//��Դ��ȥ�͹��� ������������ ��һ���ӳ� //�¼�����
			}
		}
		
		if(i%28==0) DS18B20_tmp=DS18B20_Get_Temp( ); //����ֵ -12389 = �¶� -123.89
		if(i%51==0)
		{	
			led4pin_Display(2,DS18B20_tmp/1000%10);
			led4pin_Display(3,DS18B20_tmp/100%10);
			led4pin_Display(4,DS18B20_tmp/10%10);			
			if(DS18B20_tmp/100 >=100) led4pin_Display(1,DS18B20_tmp/10000%10);	
			if(DS18B20_tmp<0) led4pin_Display(1,11);//��ʾ -
			else led4pin_Display(1,15);//��ʾ ��		
		}

	}
}

