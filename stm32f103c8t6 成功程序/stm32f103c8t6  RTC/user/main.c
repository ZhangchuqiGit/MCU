
#include "includes.h"

int main()
{	
	u8 t=0,haha;	
	_calendar_obj time1,time2;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��

	USART1_Init(115200,0);	 //���ڳ�ʼ��Ϊ115200
	printf("ok\r\n"); 		
	RTC_Init();	  			//RTC��ʼ�� 
	Time_Get(&time1);
	//printf("time1:%d-%d-%d  %d:%d:%d  %d\r\n",time1.w_year,time1.w_month,time1.w_date,time1.hour,time1.min,time1.sec,time1.week);

	while(1)
	{								    
		if(t!=calendar.sec)
		{
			t=calendar.sec;
			printf("%4d-%02d-%02d %02d:%02d:%02d ��%d ",calendar.w_year,calendar.w_month,calendar.w_date,
												 calendar.hour,calendar.min,calendar.sec,calendar.week);									  
			Time_Get(&time2);
		  //printf("time2:%d-%d-%d  %d:%d:%d  ��%d\r\n",time2.w_year,time2.w_month,time2.w_date,time2.hour,time2.min,time2.sec,time2.week);
			haha=compare_time(time1,time2);//����time2-time1������
			printf("����:%3ds\r\n",haha);
			if(haha%10==0)
			{
				printf("**********����3s��������**********\r\n");
				RTC_Alarm_Set_after(0,0,0,3);//0��0Сʱ0��3�����������ж�,����һ�µ�������һ���������ø�����
				//RTC_Alarm_Set(2017,5,22,22,31,5);  //�������� 
			}
			if(haha%15==0)
			{
				printf("*************����ʱ��*************\r\n");
				RTC_Set(2018,5,22,0,0,0);//�ı�ʱ��
				Time_Get(&time1);
			}							
		} 
	}
}
/*ͨ�����ĺ궨�廹�л����������жϷ���#define RTCAlarm_Way 1  //0�ж��߷�����1 ԭ�ӷ���
ע�����
��1��ÿ����������rtc��Ҫ����  #define data_change 0x3456  
       ���óɹ��Ժ�����ʱ�䲻��Ҫ����    #define data_change 0x3456    ֱ�����������������Ϳ�����
       RTC_Set(2017,5,22,11,14,0);        //����ʱ��
       RTC_Alarm_Set(2017,5,22,11,14,5);  //��������
��2��rtcʱ����ʱ��׼��ʱ�������32.768K�ľ��������ϲ�����1M(>1mҲ����)�ĵ��裬һ���ܽ��
��3��#define RTCAlarm_Way 1  �ò�ѯ��ʽ���������ж�
       ԭ�ӵ�������������RTC_ITConfig(RTC_IT_ALR, ENABLE);   //ʹ��RTC�������ж�   û�������ж�
       #define RTCAlarm_Way 0  �����ж���17�ж��߷������������ж�*/