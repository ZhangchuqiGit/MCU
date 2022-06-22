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
	SystemInit();//ϵͳʱ������,����ϵͳʱ��Ϊ72M	
	delay_init(72);//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
//	JTAG_SWD_GPIO_Config();//��ʹ��JTAG���ԣ���Ӧ��IO����PB3,PB4,PA15������Ϊ��ͨIO��ʹ�ã�������ʹ��SWD�������¼��������ͷų���
//	uart1_init(115200);// USART1_TX PA.9     RX PA.10
	
	led4pin_Init();
	DS18B20_Init();
	RTC_Init();
//	while(RTC_Init())
//	{
//		printf("INIT Programing is ERROR!!\r\n");
//	}
	RTC_Set(2018,5,22,0,0,0);//����ʱ��
	RTC_Set(2020,3,5,1,1,0);//�ı�ʱ��
	Time_Get(&time1);//����˿�ʱ����Ϊ��ʼʱ��
	
	while(1) 
	{   	
		Time_Get(&time2);//����˿�ʱ��
		haha=compare_time(time1,time2);//����time2-time1������		
//		if(haha%3==0)
//		{
//printf("**********����4s��������**********\r\n");  �������ģʽǰִ�У�������
//RTC_Alarm_Set_after(0,0,0,4);//0��0Сʱ0��4�����������ж�,����һ�µ�������һ���������ø�����
//			RTC_Alarm_Set_after(0,0,0,3);
			//RTC_Alarm_Set(2017,5,22,22,31,5);  //�������� 
			
led4pin_Display(4,6);
			RTC_Alarm_Set_after(0,0,0,3);
				PWR_EnterSTANDBYMode();//�������ģʽ		
			
/*ͣ��ģʽ������������ 
��һ������ PWR_Regulator ��ѡ���Դ�Ƿ����͹���ģʽ
#define PWR_Regulator_ON//��Դ�����͹��� ���ѻ���û�ӳ�
#define PWR_Regulator_LowPower//��Դ��ȥ�͹��� ��������������һ���ӳ�    
�ڶ������� PWR_STOPEntry 
#define PWR_STOPEntry_WFI//�жϻ���
#define PWR_STOPEntry_WFE//�¼�����  */ 
//led4pin_Display(2,2);
//			RTC_Alarm_Set_after(0,0,0,3);
//				PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);//ͣ��ģʽ//��Դ�����͹��� ���ѻ���û�ӳ�//�жϻ���


//led4pin_Display(3,3);
//			RTC_Alarm_Set_after(0,0,0,3);
//				PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFE);///ͣ��ģʽ//��Դ�����͹��� ���ѻ���û�ӳ�//�¼�����

//led4pin_Display(4,4);
//			RTC_Alarm_Set_after(0,0,0,3);
//				PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//ͣ��ģʽ//��Դ��ȥ�͹��� ��������������һ���ӳ�    //�жϻ���


//RTC_Alarm_Set_after(0,0,0,3);
//				PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFE);///ͣ��ģʽ//��Դ��ȥ�͹��� ��������������һ���ӳ�   //�¼�����

//		}
			
//		i++;
//		if(i>=10)//50 000*0.1us=5ms
//		{
//			i=0;
//			DS18B20_tmp=DS18B20_Get_Temp( ); //����ֵ -12389 = �¶� -123.89
//		}
//		if(1)
//		{	
//			led4pin_Display(2,DS18B20_tmp/1000%10);
//			led4pin_Display(3,DS18B20_tmp/100%10);
//			led4pin_Display(4,DS18B20_tmp/10%10);			
//			if(DS18B20_tmp<0) led4pin_Display(1,11);//��ʾ -
//			else led4pin_Display(1,15);//��ʾ ��		
//			if(DS18B20_tmp/100 >=100) led4pin_Display(1,DS18B20_tmp/10000%10);	
//		}	  
	}
}

