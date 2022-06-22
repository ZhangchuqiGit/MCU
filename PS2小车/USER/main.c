#include <stm32f10x.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "math.h"
#include "stdlib.h"
#include "pstwo.h"
#include "motor.h"
#include "timer.h"
#include "ultrasonic.h"

#define EN_APC220 0

#if EN_APC220
#define APC220_IN  PAin(4) 
void APC220_Init(void);
#endif

void mm_auto(void);

int main(void)
{
	u8 flag_auto=0,auto_state=0;
	u16 sys_time=0;
	
	SystemInit();//ϵͳʱ������,����ϵͳʱ��Ϊ72M	
	delay_init(72);//��ʱ��ʼ��  
	JTAG_SWD_GPIO_Config();//��ʹ��JTAG���ԣ���Ӧ��IO����PB3,PB4,PA15������Ϊ��ͨIO��ʹ�ã�������ʹ��SWD�������¼��������ͷų���
	/*  �ж����ȼ�NVIC����  */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	uart1_init(38400);//USART1_TX PA.9 ; USART1_RX PA.10 
#if EN_APC220
//	APC220_Init();
	while(1)
	{
		sys_time++;
		delay_ms(1000);
		delay_ms(500);
//		if(APC220_IN==1)
//		{
		printf("receive NO:%d\n",sys_time);	
//		}			
//		else
//		{
//			
//		}
	}
#else	
	PS2_Init(); //�����˿ڳ�ʼ��
	PS2_SetInit();	//�����ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸� //������ģʽ
    PS2_Vibration(0x40,0x40);//������motor1  0xFF���������أ�motor2  0x40~0xFF

/*	TIM3_PWM_Init (arr, psc);   
����TIM3_PWM_Init (1000,144);    �趨�Զ�װ��ֵΪ1000����Ƶ144��     
�ж�/���  ʱ�� Tout us= (arr*psc)/Tclk   ��λ: us  */
	TIM3_PWM_Init(7200,speed_max); // 7200 * (speed_max=100) / 72Mhz = 10 ms 

	motor_Init( );//������ƶ˿ڳ�ʼ��
	
/*	TIM2_Int_Init (arr, psc);   
����TIM2_Int_Init (10000-1,72-1);    �趨�Զ�װ��ֵΪ10000����Ƶ72��     
�ж�/���  ʱ�� Tout us= (arr+1)*(psc+1)/Tclk    ��λ: us   */
//	TIM2_Int_Init(10000-1,72-1); // 10 000 * 72 / 72Mhz = 10000 us   
/* "TIM_GetCounter(TIM2)"->ʱ��������� ?us / 10000.0 =?ms * "340"->�����ٶ�340m/s= 340mm/ms / "2.0"->����·��;  */
//	Ultrasonic_Config( ); //�Գ�����ģ���ʼ��
	while(1)
	{
		if(sys_time>9000)//9ms
		{
			sys_time=0;			
			if( PS2_RedLight() == 0 )
			{
				delay_ms(4);			
				flywheel = PS2_DataKey(); //�ֱ�����ֵ������ 			
				if(flywheel==PSB_RED)
				{
					flag_auto++;
					if(flag_auto>200)
					{
						flag_auto=0;
						auto_state=!auto_state;
					}
				}
				if(auto_state==0)
				{
					flag_auto=0;
					speed_select( );//�õ�һ��ҡ�˵�ģ����  ��Χ0~256  ;  �ٶ�: speed -100/+100 ; swerve -100/+100 ;
					Motor_Control(speed,swerve);//speed:ǰ��/���� , swerve:��ת/��ת 	
				}
			}
			else//�ж��ֱ����Ǻ��ģʽ
			{
				flag_auto=0;
				auto_state=0;
				flywheel = 0; //û���κΰ�������
				stop();//�ٶ� = 0;
			}
		}	
		if(auto_state!=0)
		{
			mm_auto();
		}	
		sys_time++;
		delay_us(1);
	}
#endif
}

void mm_auto(void)
{
	Motor_Control(speed,swerve);//speed:ǰ��/���� , swerve:��ת/��ת 	
}
#if EN_APC220
void APC220_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
}
#endif
//	if(speed<-60)//�������ˣ�  ����ǰ��
//	{
//		Ultrasonic_Measure( ); //��ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
//		if(Distance<20) //������ľ���//ǰ�����
//		{
//			Ult_Measure=1;
//		}
//	}
//	else if(speed>60)//�������ˣ�  ����ǰ��
//	{
//		Ultrasonic_Measure_B( );//�󷽲��
//		if(Distance_B<20) //������ľ���//�󷽲��
//		{
//			Ult_Measure_B=1;
//		}
//	}
//	else ;
