#include "motor.h"

u8 flywheel=0;

void motor_Init(void)//电机控制端口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
//	JTAG_SWD_GPIO_Config();//不使用JTAG调试，对应的IO口如PB3,PB4,PA15引脚作为普通IO口使用，可以在使用SWD仿真和烧录的情况下释放出来

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推免输出  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_5);  //设置 0
	
	PWMA(0); //左电机速度控制 CH1(PA6)
	PWMB(0); //右电机速度控制 CH2(PA7)
}

void PWMA(u16 speedval)/*左电机速度控制 CH1(PA6)*/
{
	TIM3->CCR1=speedval;//左电机速度控制 CH1(PA6) //PWMA()
}
void PWMB(u16 speedval)/*左电机速度控制 CH2(PA7)*/
{
	TIM3->CCR2=speedval;//右电机速度控制 CH2(PA7) //PWMB()
}

/*	TIM3_PWM_Init (arr, psc);   
设置TIM3_PWM_Init (1000,144);    设定自动装载值为1000，分频144。     
中断/溢出  时间 Tout us= (arr*psc)/Tclk
中断/溢出  时间 Tout us=  1000 * 144 / (72 MHz) = 2000 us  */
//定时器TIM3，PWM控制初始化,CH1（PA6）、CH2(PA7)。
//arr：rr设定计数器自动重装值
//psc：时钟预分频数，预分频器
//设置TIM3_PWM_Init(250,0); 
//设定自动装载值为250，不分频。PWM频率=72000/250=80Khz
void TIM3_PWM_Init(u16 arr,u16 psc) 
{
	RCC->APB1ENR|=1<<1;       //TIM3时钟使能    
	  	
	GPIOA->CRL&=0X00FFFFFF;//PA6,7输出
	GPIOA->CRL|=0XBB000000;//复用功能输出 	  
	GPIOA->ODR|=3<<6;//PA6.7上拉	

	TIM3->ARR=arr;//设定计数器自动重装值 
	TIM3->PSC=psc;//预分频器不分频
	
	TIM3->CCMR1|=6<<4;  //CH1 PWM1模式	高电平有效	 
	TIM3->CCMR1|=1<<3; //CH1预装载使能
	TIM3->CCMR1|=6<<12;  //CH2 PWM1模式		 
	TIM3->CCMR1|=1<<11; //CH2预装载使能	   

	TIM3->CCER|=1<<0;   //OC1 输出使能
	TIM3->CCER|=1<<4;   //OC2 输出使能	   

	TIM3->CR1=0x0080;   //ARPE使能 
	TIM3->CR1|=0x01;    //使能定时器3 
	
	TIM3->CCR1=0;//左电机速度控制 CH1(PA6) //PWMA()
	TIM3->CCR2=0;//右电机速度控制 CH2(PA7) //PWMB()
}

#if 1
void Motor_Control(s16 speed, s16 swerve)//speed:前进/后退 , swerve:左转/右转 
{
	u8 sweval=20;
	speed=speed*7.0/10.0;/* 降速比 *///正：后退；  负：前进

	if( speed >= +1 )//前进
	{
		if(flywheel==PSB_L3)
		{
			speed=speed*10.0/7.0;/* 降速比 */
			//speed=speed*7.0/10.0;/* 降速比 */
		}
		MR_GO = 1;
		MR_BACK = 0;	
		ML_GO = 1;
		ML_BACK = 0;			
		if( swerve >= +1 )//右转
		{
			PWMA(speed);/*左电机速度控制 CH1(PA6)*/
			PWMB(	((100.0-swerve)<=sweval)?  sweval:(speed*(100.0-swerve)/100.0)	);
		}
		else if( swerve <= -1 )//左转
			{
				swerve=-swerve;//取正数
				PWMA(((100.0-swerve)<=sweval)?  sweval:(speed*(100.0-swerve)/100.0)); 				
				PWMB(speed); 
			}
			else //前进
			{
				PWMA(speed);
				PWMB(speed); 
			}
	}
	else if( speed <= -1 )//后退
		{
			speed=-speed;//取正数
			MR_GO = 0;
			MR_BACK = 1;	
			ML_GO = 0;
			ML_BACK = 1;			
			if( swerve >= +1 )//右转
			{
				PWMA(speed); 
				PWMB(((100.0-swerve)<=sweval)? sweval:(speed*(100.0-swerve)/100.0));				
			}
			else if( swerve <= -1 )//左转
				{		
					swerve=-swerve;//取正数
					PWMA(((100.0-swerve)<=sweval)?  sweval:(speed*(100.0-swerve)/100.0)); 				
					PWMB(speed); 
				}
				else //后退
				{
					PWMA(speed);
					PWMB(speed); 
				}
		}
		else //原地打转--既不前进也不后退
		{
			if( swerve >= +1 )//原地右转
			{
				MR_GO = 0;
				MR_BACK = 1;	
				ML_GO = 1;
				ML_BACK = 0;	
				if(flywheel==PSB_R3) 
				{
					PWMA(swerve);
					PWMB(swerve); 
				}
				else 
				{
					PWMA(swerve*70.0/100.0);
					PWMB(swerve*70.0/100.0); 
				}
			}		
			else if( swerve <= -1 )//原地左转
				{
					swerve=-swerve;//取正数
					MR_GO = 1;
					MR_BACK = 0;	
					ML_GO = 0;
					ML_BACK = 1;			
					if(flywheel==PSB_R3) 
					{
						PWMA(swerve);
						PWMB(swerve); 
					}
					else 
					{
						PWMA(swerve*70.0/100.0);
						PWMB(swerve*70.0/100.0); 
					}
				}
				else //speed=0,swerve=0
				{
					stop();
				}
		}	
}
#else
void Motor_Control(s16 speed, s16 swerve)//speed:前进/后退 , swerve:左转/右转 
{
//	printf("手柄按键捕获处理\r\n");
	if( speed >= +1 )//前进
	{
		if( swerve >= +1 )//右转
		{
			if(speed_max<=speed*(100.0-swerve)/100.0)			
			{
				MR_GO = 1;
				MR_BACK = 0;	
			}
			else
			{
				MR_GO = 0;
				MR_BACK = 0;	
			}
			if(speed_max<=speed)		
			{
				ML_GO = 1;
				ML_BACK = 0;			
			}
			else
			{
				ML_GO = 0;
				ML_BACK = 0;			
			}
		}
		else if( swerve <= -1 )//左转
			{
				swerve=-swerve;//取正数
				if(speed_max<=speed)				
				{
					MR_GO = 1;
					MR_BACK = 0;	
				}
				else
				{
					MR_GO = 0;
					MR_BACK = 0;	
				}
				if(speed_max<=speed*(100.0-swerve)/100.0)	
				{
					ML_GO = 1;
					ML_BACK = 0;			
				}
				else
				{
					ML_GO = 0;
					ML_BACK = 0;			
				}
			}
			else //前进
			{
				MR_GO = 1;
				MR_BACK = 0;	
				ML_GO = 1;
				ML_BACK = 0;			
			}
		}
		else if( speed <= -1 )//后退
		{
			speed=-speed;//取正数
			if( swerve >= +1 )//右转
			{
				if(speed_max<=speed*(100.0-swerve)/100.0)			
				{
					MR_GO = 0;
					MR_BACK = 1;	
				}
				else
				{
					MR_GO = 0;
					MR_BACK = 0;	
				}
				if(speed_max<=speed )			
				{
					ML_GO = 0;
					ML_BACK = 1;			
				}
				else
				{
					ML_GO = 0;
					ML_BACK = 0;			
				}
			}
			else if( swerve <= -1 )//左转
				{		
					swerve=-swerve;//取正数
					if(speed_max<=speed)			
					{
						MR_GO = 0;
						MR_BACK = 1;	
					}
					else
					{
						MR_GO = 0;
						MR_BACK = 0;	
					}
					if(speed_max<=speed*(100.0-swerve)/100.0 )			
					{
						ML_GO = 0;
						ML_BACK = 1;			
					}
					else
					{
						ML_GO = 0;
						ML_BACK = 0;			
					}
				}
				else //后退
				{
					MR_GO = 0;
					MR_BACK = 1;	
					ML_GO = 0;
					ML_BACK = 1;			
				}
		}
		else //原地打转--既不前进也不后退
		{
			if( swerve >= +1 )//原地右转
			{
				if(flywheel==PSB_R3) 
				{
					if(speed_max<= swerve)			
					{
						MR_GO = 0;
						MR_BACK = 1;	
						ML_GO = 1;
						ML_BACK = 0;			
					}
					else 		
					{
						MR_GO = 0;
						MR_BACK = 0;	
						ML_GO = 0;
						ML_BACK = 0;			
					}
				}
				else 
				{
					if(speed_max<= swerve*6.0/8.0)			
					{
						MR_GO = 0;
						MR_BACK = 1;	
						ML_GO = 1;
						ML_BACK = 0;			
					}
					else 		
					{
						MR_GO = 0;
						MR_BACK = 0;	
						ML_GO = 0;
						ML_BACK = 0;			
					}
				}
			}
			else if( swerve <= -1 )//原地左转
				{
					swerve=-swerve;//取正数
					if(flywheel==PSB_R3) 
					{
						if(speed_max<= swerve)			
						{
							MR_GO = 1;
							MR_BACK = 0;	
							ML_GO = 0;
							ML_BACK = 1;			
						}
						else 		
						{
							MR_GO = 0;
							MR_BACK = 0;	
							ML_GO = 0;
							ML_BACK = 0;			
						}
					}
					else 
					{
						if(speed_max<= swerve*6.0/8.0)			
						{
							MR_GO = 1;
							MR_BACK = 0;	
							ML_GO = 0;
							ML_BACK = 1;			
						}
						else 		
						{
							MR_GO = 0;
							MR_BACK = 0;	
							ML_GO = 0;
							ML_BACK = 0;			
						}
					}
				}
				else //speed=0,swerve=0
				{
					stop();
				}
		}	
}
#endif 

void stop(void)//刹车
{
	MR_GO = 0;
	MR_BACK = 0;
	ML_GO = 0;
	ML_BACK = 0;
	PWMA(0);
	PWMB(0);
}

void stop_Measure(void)//刹车
{
	MR_GO = 0;
	MR_BACK = 1;
	ML_GO = 0;
	ML_BACK = 1;
	PWMA(speed_max);
	PWMB(speed_max);
}
void stop_Measure_B(void)//刹车
{
	MR_GO = 1;
	MR_BACK = 0;
	ML_GO = 1;
	ML_BACK = 0;
	PWMA(speed_max);
	PWMB(speed_max);
}

