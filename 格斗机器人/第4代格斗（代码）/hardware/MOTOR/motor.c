#include "motor.h"
/***********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File：TB6612驱动代码
Author：pinggai    Version:1.0     Data:2015/05/06
Description: Balance-STM32 TB6612FNG驱动代码
             
			 PC0~3接AIN1,AIN2,BIN1,BIN2;方向控制端口        //PA0~3
			 PA6接PWMA;右电机（J1）速度控制 
			 PA7接PWMB;左电机（J2）速度控制
             
			 371电机与电机驱动TB6612FNG连接方式			
	         右电机（J1）：黄色连AO1;橙色连接AO2
             左电机（J2）：黄色连BO1;橙色连接BO2
	         
			 向前：从右侧看，两电机顺时针方向转动  正转
	         向后：从右侧看，两电机逆时针方向转动  反转  
***********************************************************/

//电机旋转方向控制信号端口初始化
//PC0~3推挽输出，输出高
//void M_Init(void)
//{
//	RCC->APB2ENR|=1<<4;      //使能PORTC时钟
//	GPIOC->CRL&=0XFFFF0000;
//	GPIOC->CRL|=0X00003333;  //PC3~0推挽输出
//	GPIOC->ODR|=0XF<<0;	     //PC3~0输出高电平
//}
void M_Init(void)
{
	GPIO_InitTypeDef GPIO_Initsturct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Initsturct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_Initsturct);

	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init(GPIOA,&GPIO_Initsturct);
	
//	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_6;
//  GPIO_Init(GPIOA,&GPIO_Initsturct);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
//	GPIO_SetBits(GPIOA,GPIO_Pin_6);
}

//定时器TIM3，PWM控制初始化,CH1（PA6）、CH2(PA7)。
//arr：自动重装值
//psc：时钟预分频数
//设置TIM3_PWM_Init(250,0); 
//设定自动装载值为250，不分频。PWM频率=72000/250=80Khz
void TIM3_PWM_Init(u16 arr,u16 psc) //arr设定计数器自动重装值   
                                    //psc预分频器不分频,psc=0
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
}
/**************        *****************  电机  控制  *****************          *****************/
void Motor__Control(s16 speed  , s16 swerve/*转弯量*/)//speed:前进/后退   swerve:左转/右转 
{
//	printf("手柄按键捕获处理\r\n");
	if( speed >= +1 )//前进 0~46
	{
		if( swerve >= +1 )//右转swerve: 0~50
		{
			M1_1 = 1;
			M1_2 = 0;	
			TIM3->CCR1 = speed>swerve ? speed : swerve  ;		
			M2_1 = 1;
			M2_2 = 0;			
			TIM3->CCR2 = speed>swerve ? speed-swerve/2 : swerve*65/100 ;//75-52/2=75-26=49= 65%
		}
		else if( swerve <= -1 )//左转swerve: -50~0
			{
				swerve=-swerve;//取正数
				M1_1 = 1;
				M1_2 = 0;	
				TIM3->CCR1 = speed>swerve ? speed-swerve/2 : swerve*65/100 ;//75-52/2=75-26=49= 65%
				M2_1 = 1;
				M2_2 = 0;			
				TIM3->CCR2 = speed>swerve ? speed : swerve  ;	
			}
			else //前进speed: 0~46  
			{
				M1_1 = 1;
				M1_2 = 0;	
				TIM3->CCR1 = speed ;	
				M2_1 = 1;
				M2_2 = 0;			
				TIM3->CCR2 = speed;		
			}
	}
	else if( speed <= -1 )//后退
		{
			speed=-speed;//取正数
			if( swerve >= +1 )//右转swerve: 0~50
			{
				M1_1 = 0;
				M1_2 = 1;	
				TIM3->CCR1 = speed>swerve ? speed : swerve  ;	
				M2_1 = 0;
				M2_2 = 1;			
				TIM3->CCR2 = speed>swerve ? speed-swerve/2 : swerve*65/100 ;//75-52/2=75-26=49= 65%
			}
			else if( swerve <= -1 )//左转swerve: -50~0
				{		
					swerve=-swerve;//取正数
					M1_1 = 0;
					M1_2 = 1;	
					TIM3->CCR1 =  speed>swerve ? speed-swerve/2 : swerve*65/100 ;//75-52/2=75-26=49= 65%
					M2_1 = 0;
					M2_2 = 1;			
					TIM3->CCR2 =  speed>swerve ? speed : swerve ;
				}
				else //后退speed: 0~46
				{
					M1_1 = 0;
					M1_2 = 1;	
					TIM3->CCR1 = speed ;	
					M2_1 = 0;
					M2_2 = 1;			
					TIM3->CCR2 = speed;		
				}
		}
		else //原地打转--既不前进也不后退
		{
			if( swerve >= +1 )//原地右转swerve: 0~46
			{
				M1_1 = 1;
				M1_2 = 0;	
				TIM3->CCR1 = swerve;	
				M2_1 = 0;
				M2_2 = 1;			
				TIM3->CCR2 = swerve;		
			}
			else if( swerve <= -1 )//原地左转swerve: -46~0
				{
					swerve=-swerve;//取正数
					M1_1 = 0;
					M1_2 = 1;	
					TIM3->CCR1 = swerve ;	
					M2_1 = 1;
					M2_2 = 0;			
					TIM3->CCR2 = swerve;		
				}
				else
				{
					M1_1 = 1;
					M1_2 = 1;
					M2_1 = 1;
					M2_2 = 1;
					TIM3->CCR1 = 255;
					TIM3->CCR2 = 255;
				}
		}	
}

#if 0
//左右电机方向与速度控制
//motor1：右电机（J1）；motor2：左电机（J2）
//小于0时，向前，大于0时，向后    
//motor1/2的取值范围（-250~+250）；值的大小决定占空比的大小
//例motor1取值为90，则占空比为10%。
void Motor_Speed_Control(s16 motor1, s16 motor2)	 
{
    s16 motor1speed = 0, motor2speed = 0 ;	
    if(motor1>250)  motor1speed = 250;
	    else if (motor1<-250)  motor1speed = -250;
			else  motor1speed = motor1;
	if(motor2>250)  motor2speed = 250;
	    else if (motor2<-250)  motor2speed = -250;
			else  motor2speed = motor2;
/**********************************************/
	if(motor1speed == 0) //刹车
	{
		M1_1 = 1;
		M1_2 = 1;
		TIM3->CCR1 = 250;
	}
	else if(motor1speed > 0)
		{
			M1_1 = 1;
			M1_2 = 0;
			TIM3->CCR1 = motor1speed;
		}
		else
		{
			M1_1 = 0;
			M1_2 = 1;
			TIM3->CCR1 = -motor1speed;
		}
/**********************************************/
	if(motor2speed == 0)//刹车
	{
		M2_1 = 1;
		M2_2 = 1;
		TIM3->CCR2 = 250;
	}
	else if(motor2speed > 0)
		{
			M2_1 = 1;
			M2_2 = 0;
			TIM3->CCR2 = motor2speed;
		}
		else
		{
			M2_1 = 0;
			M2_2 = 1;
			TIM3->CCR2 = -motor2speed;
		}
}

///*************************************************定时器*******************************************************/
////通用定时器2中断初始化
////这里时钟选择为APB1的2倍，而APB1为36M=72M
////arr：自动重装值。
////psc：时钟预分频数
//void TIM2_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
//	NVIC_InitTypeDef 			NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
//    TIM_DeInit(TIM2);//复位定时器2所有寄存器 

//	//定时器TIM2初始化
//	TIM_TimeBaseStructure.TIM_Period = arr; 					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 			//根据指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 					//使能指定的TIM2中断,允许更新中断

//	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//先占优先级1级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  		//从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  							//初始化NVIC寄存器

//	TIM_Cmd(TIM2, ENABLE);  									//使能TIM2					 
//}

///* **************** STM32F103C8T6 定时器中断服务程序**************** */
//void TIM2_IRQHandler(void)
//{
//}

#endif
