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
	
	SystemInit();//系统时钟设置,配置系统时钟为72M	
	delay_init(72);//延时初始化  
	JTAG_SWD_GPIO_Config();//不使用JTAG调试，对应的IO口如PB3,PB4,PA15引脚作为普通IO口使用，可以在使用SWD仿真和烧录的情况下释放出来
	/*  中断优先级NVIC设置  */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

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
	PS2_Init(); //驱动端口初始化
	PS2_SetInit();	//配配置初始化,配置“红绿灯模式”，并选择是否可以修改 //开启震动模式
    PS2_Vibration(0x40,0x40);//振动设置motor1  0xFF开，其他关，motor2  0x40~0xFF

/*	TIM3_PWM_Init (arr, psc);   
设置TIM3_PWM_Init (1000,144);    设定自动装载值为1000，分频144。     
中断/溢出  时间 Tout us= (arr*psc)/Tclk   单位: us  */
	TIM3_PWM_Init(7200,speed_max); // 7200 * (speed_max=100) / 72Mhz = 10 ms 

	motor_Init( );//电机控制端口初始化
	
/*	TIM2_Int_Init (arr, psc);   
设置TIM2_Int_Init (10000-1,72-1);    设定自动装载值为10000，分频72。     
中断/溢出  时间 Tout us= (arr+1)*(psc+1)/Tclk    单位: us   */
//	TIM2_Int_Init(10000-1,72-1); // 10 000 * 72 / 72Mhz = 10000 us   
/* "TIM_GetCounter(TIM2)"->时间计数总数 ?us / 10000.0 =?ms * "340"->空气速度340m/s= 340mm/ms / "2.0"->往返路程;  */
//	Ultrasonic_Config( ); //对超声波模块初始化
	while(1)
	{
		if(sys_time>9000)//9ms
		{
			sys_time=0;			
			if( PS2_RedLight() == 0 )
			{
				delay_ms(4);			
				flywheel = PS2_DataKey(); //手柄按键值捕获处理 			
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
					speed_select( );//得到一个摇杆的模拟量  范围0~256  ;  速度: speed -100/+100 ; swerve -100/+100 ;
					Motor_Control(speed,swerve);//speed:前进/后退 , swerve:左转/右转 	
				}
			}
			else//判断手柄不是红灯模式
			{
				flag_auto=0;
				auto_state=0;
				flywheel = 0; //没有任何按键按下
				stop();//速度 = 0;
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
	Motor_Control(speed,swerve);//speed:前进/后退 , swerve:左转/右转 	
}
#if EN_APC220
void APC220_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;// 浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
}
#endif
//	if(speed<-60)//正：后退；  负：前进
//	{
//		Ultrasonic_Measure( ); //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
//		if(Distance<20) //计算出的距离//前方测距
//		{
//			Ult_Measure=1;
//		}
//	}
//	else if(speed>60)//正：后退；  负：前进
//	{
//		Ultrasonic_Measure_B( );//后方测距
//		if(Distance_B<20) //计算出的距离//后方测距
//		{
//			Ult_Measure_B=1;
//		}
//	}
//	else ;
