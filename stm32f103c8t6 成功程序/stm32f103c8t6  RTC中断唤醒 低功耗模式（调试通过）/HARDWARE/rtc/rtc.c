#include "rtc.h" 		    

//��һ�����õ�ʱ��������ֵ�����ҶϿ���غ͵�Դһ��ʱ�䣬��֤���ݵ���Ҳ���꣬���߶̽�vbat��GND
/*
    ��ʵ����32.768K�ľ��������ϲ�����1M(>1mҲ����)�ĵ����û���ˣ�������ô�����Ⱑ��
    ��Ҫ��STM32��LSE���������е�󣬲������裬���ܼ�С�������裬�������ϵ������Ӧ�ģ�LSE�Ĺ��ľͻ��һ�㡣
    ������û��ϵ��STM32��ʹ���ⲿ��Դʱ���ڲ����ػ��LSE�ĵ�Դ�л����ⲿ��Դ�ġ�
    1��40mAH��С��أ���3��Ӧ��û���⡣
    3�꣬�������Ʒ���᲻�ỹ��ʹ�ö��Ǹ������ˡ�˵�����������ˡ�

*/

#define RTCAlarm_Way 1  //0�ж��߷�����1 ԭ�ӷ���

_calendar_obj calendar;//ʱ�ӽṹ�� 
 
static void RTC_NVIC_Config(void)
{	
		 NVIC_InitTypeDef NVIC_InitStructure;
		 NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;  //RTCȫ���ж�
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //ʹ�ܸ�ͨ���ж�
		 NVIC_Init(&NVIC_InitStructure);
#if  (RTCAlarm_Way==0)
		 NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;  //�����ж�
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��RTCȫ���жϵ����ȼ���
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		 NVIC_Init(&NVIC_InitStructure);
#endif
}

#if (RTCAlarm_Way==0)
static void RTC_Alarm_EXIT(void)
{ 
		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_ClearITPendingBit(EXTI_Line17);
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
}
#endif

//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������
u8 RTC_Init(void)
{	
	u8 temp = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);//��Դʱ�Ӻͱ���ʱ��//ʹ��PWR��BKP����ʱ��
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܺ󱸼Ĵ�������//����������д
//	PWR_WakeUpPinCmd(ENABLE);//ʹ��/���ѹܽŹ���
//  RCC_RTCCLKCmd(ENABLE); 	
// 	PWR_DeInit();
	
	if(PWR_GetFlagStatus(PWR_FLAG_SB)!=RESET) //������ڴ��ڴ���ģʽ
	{
		PWR_ClearFlag(PWR_FLAG_SB);//�������ģʽ
	}

/*	if (BKP_ReadBackupRegister(BKP_DR1) != 0xC0B4)����˼������STM32�ϵ���Լ��ǲ��ǵ�һ�������������
	�����һ������������������ֵһ����0X0000��ֵ��0XC0B4����Ⱦͽ������ó�ʼ������
	����Ĵ�����д��0XC0B4,ע��BKP_DR1���ֵ��д��֮����㸴λ��Ҳ���ᱻ�����0000��
	�����Ļ����㸴λ���������ϵ磬��ʼ����Ҳ������ִ��һ�飬����RTC��ֵ�Ͳ��������������ˡ�
	�����ҪRTC��ֵ���´�0��ʼ�������ǾͿ��԰�0XC0B4�ĳ�һ���µ����֣���������һ�γ���Ϳ����ˡ�*/
	if (BKP_ReadBackupRegister(BKP_DR1) != data_change)//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
	{	 			
		BKP_DeInit();//��λ�������� 		
		RCC_LSEConfig(RCC_LSE_ON);//�����ⲿ���پ���(LSE),ʹ��������پ���	
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<100)//���ָ����RCC��־λ�������,�ȴ����پ������
		{
			temp++;
			delay_ms(10);
		}
		if(temp>=100)
		{
//			printf("jingzhen-error\r\n");
			return 1;//��ʼ��ʱ��ʧ��,����������
		}	
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ�� 
		
/*ΪRTC�ṩʱ��Ƶ�ʻ���������ѡ��ģ�����������һ�־͹��ˡ�
RTC_WaitForLastTask()��RTC_WaitForSynchro()���ǵȴ������д������ɵ���˼��
��RTC���õ�ʱ�������������һ�����Ǻ���Ҫ�ģ�
������λ�ò�����ȱʧ������ʱ��Ῠ��һ���ط�*/
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();//�ȴ�RTC�Ĵ���(RTC_CNT,RTC_ALR��RTC_PRL)��RTC APBʱ��ͬ��				

		RTC_ITConfig(RTC_IT_SEC, ENABLE);//ʹ��RTC���ж�
		RTC_ITConfig(RTC_IT_ALR, ENABLE);//ʹ��RTC�������ж�
		
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������

		RTC_EnterConfigMode();// �������ã���������RTCģʽ
		
/*ʱ������ѡ����ⲿ����ʱ��LSE������RTC�ṩʱ��Ƶ�ʡ���Ƶ��Ϊ��32.768KHZ.
�ֳ�1HZ = 32768/(32767+1)*/
		RTC_SetPrescaler(32767);//����RTCԤ��Ƶ��ֵ		
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
		RTC_ExitConfigMode();//�˳�����ģʽ	
		
        BKP_WriteBackupRegister(BKP_DR1, data_change);//��ָ���ĺ󱸼Ĵ�����д���û���������
			
//		printf("time_set_ok\r\n"); 
//		RTC_Set(2020,3,1,1,1,0);        //����ʱ��//�ı�ʱ��
//		RTC_Alarm_Set(2020,3,1,1,1,5);  //�������� 		
//		PWR_BackupAccessCmd(DISABLE);//������������д����
	}	
	else //�ڶ���������������ʱ��ִ�е������Ҫ�Ǵ�RTC�������ж�
	{
//		PWR_BackupAccessCmd(DISABLE);//������������д����
//		RTC_WaitForSynchro();//�ȴ�RTC�Ĵ���(RTC_CNT,RTC_ALR��RTC_PRL)��RTC APBʱ��ͬ��				
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	  //ʹ��RTC���ж�
		RTC_ITConfig(RTC_IT_ALR, ENABLE);   //ʹ��RTC�������ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
//		printf("time_ok\r\n"); 			 
	}
	
	RTC_NVIC_Config();//RCT�жϷ�������
		
#if (RTCAlarm_Way==0)		
    RTC_Alarm_EXIT();	
#endif		
		RTC_Get();//����ʱ��	
		return 0; //ok
}	


//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Get(void)
{
			u32 timecount=0; 
			timecount=RTC_GetCounter();	 
			sec_2_time(&calendar,timecount) ;   
			return 0;
}	 
u8 Time_Get(_calendar_obj* time)
{
 			u32 timecount=0; 
			timecount=RTC_GetCounter();	
			sec_2_time(time,timecount) ;   
			return 0;  
}
//��1970��1��1��Ϊ��׼,1970~2099��Ϊ�Ϸ����
//syear,smon,sday,hour,min,sec�����ӵ�������ʱ����
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
			u32 seccount=0;
			seccount=time_2_sec(syear,smon,sday,hour,min,sec);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
			PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
			RTC_SetCounter(seccount);	//����RTC��������ֵ
			RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
			return 0;	    
}


//��ʼ������		  
//��1970��1��1��Ϊ��׼,1970~2099��Ϊ�Ϸ����
//syear,smon,sday,hour,min,sec�����ӵ�������ʱ����
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
		u32 seccount=0;
		seccount=time_2_sec(syear,smon,sday,hour,min,sec);		    
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ�� 	
		PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
		RTC_SetAlarm(seccount);//�����´�����Ϊseccount��
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
		return 0;	    
}

//printf("**********����4s��������**********\r\n");  �������ģʽǰִ�У�������
//RTC_Alarm_Set_after(0,0,0,4);//0��0Сʱ0��4�����������ж�,����һ�µ�������һ���������ø�����
void RTC_Alarm_Set_after(u8 sday,u8 hour,u8 min,u8 sec)
{
		u32 seccount=0;
		u32 time_go=time_2_sec_go(sday,hour,min,sec);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
		PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  	
		seccount=RTC_GetCounter();
		RTC_SetAlarm(seccount+time_go);//�����´�����Ϊ time_go ��
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
}

//��������
u32 time_2_sec_go(u8 sday,u8 hour,u8 min,u8 sec)
{
    return (sday*24*3600+hour*3600+min*60+sec);
}
u32 time_2_sec(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
  seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ
  return seccount;
}
u8 sec_2_time(_calendar_obj* time,u32 timecount)
{
	u16 daycnt=0;
	u32 temp=0;
	u16 temp1=0;	  
 	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		(* time).w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year((* time).w_year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		(* time).w_month=temp1+1;	//�õ��·�
		(* time).w_date=temp+1;  	//�õ����� 
	}
	temp=timecount%86400;     		//�õ�������   	   
	(* time).hour=temp/3600;     	//Сʱ
	(* time).min=(temp%3600)/60; 	//����	
	(* time).sec=(temp%3600)%60; 	//����
	(* time).week=RTC_Get_Week((* time).w_year,(* time).w_month,(* time).w_date);//��ȡ����   
	return 0;
}

s32 compare_time(_calendar_obj time1,_calendar_obj time2)
{
   u32 seccount1=0,seccount2=0;
	 seccount1=time_2_sec(time1.w_year,time1.w_month,time1.w_date,time1.hour,time1.min,time1.sec);
	 seccount2=time_2_sec(time2.w_year,time2.w_month,time2.w_date,time2.hour,time2.min,time2.sec);
	 return (seccount2-seccount1);
}



//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}			  

///////////////////////////////////
#if (RTCAlarm_Way==0)
//RTCʱ���ж�
//ÿ�봥��һ��  
void RTC_IRQHandler(void)
{
		if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
		{
				RTC_Get();//����ʱ��
		}
		RTC_ClearITPendingBit(RTC_IT_SEC); 
		RTC_WaitForLastTask();
}
//�����ж�
//�����ȿ����ж����ж�  RTC_Alarm_EXIT();
void RTCAlarm_IRQHandler(void)
{     
		if(RTC_GetITStatus(RTC_IT_ALR) != RESET)//�Ƿ������жϷ���
		{
//			printf("**********�ж��߷�������**********\r\n");//�������ʱ��
//			printf("THE ALARM  READY =%d \r\n",RTC_GetCounter());//�����ʱ������
		}
		EXTI_ClearITPendingBit(EXTI_Line17);
		RTC_WaitForLastTask();
		RTC_ClearITPendingBit(RTC_IT_ALR);
		RTC_WaitForLastTask();
}	

#else
void RTC_IRQHandler(void)//RTC���ж�
{		
	u32 seccount=0;
//�����ж�
//����ԭ�ӵĲ�ѯ�жϷ���
	if(RTC_GetITStatus(RTC_IT_ALR)!=RESET) //�Ƿ������жϷ���
	{
//		printf("THE ALARM  READY =%d \r\n",RTC_GetCounter());//�����ʱ������
		RTC_ClearITPendingBit(RTC_IT_ALR);//�������ж�	  	 
		
		PWR_BackupAccessCmd(ENABLE);//ʹ�ܺ󱸼Ĵ�������  	
		
		RTC_EnterConfigMode();// �������ã���������RTCģʽ
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������  
	
		//printf("**********����4s��������**********\r\n");
//		RTC_Alarm_Set_after(0,0,0,4);//0��0Сʱ0��3�����������ж�,����һ�µ�������һ���������ø�����
		
		RTC_ExitConfigMode();//�˳�����ģʽ	
		
		PWR_BackupAccessCmd(DISABLE);//������������д����
	}
	
//RTCʱ���ж�
//ÿ�봥��һ��  
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
	{							
		RTC_Get();//����ʱ��   
	}

//	if(PWR_GetFlagStatus(PWR_FLAG_SB)!=RESET) //������ڴ��ڴ���ģʽ
//	{
//		PWR_ClearFlag(PWR_FLAG_SB);//�������ģʽ
//	}
	
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW); //������жϱ�־λ�����λ
	RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������  
}
#endif
