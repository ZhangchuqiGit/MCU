/***************STM32F103C8T6**********************
 * 文件名  ：timer.c
 * 描述    : TIMx定时器
 * 实验平台：STM32F103C8T6
 * 备注    ：使用定时器来产生pwm波，控制舵机动作
 * 接口    ：TIM1, CH1-PB13, CH2-PB14, //CH3-PB15
****************STM32F103C8T6**********************/
#include "timer.h"
#include "misc.h"
#include "usart.h"

/***************
中断/溢出  时间 t=  (  72M /(TIM_Prescaler+1)  )  *  (  TIM_Period+1  )
//TIM2_Int_Init(1000-1,72-1);
1000-1    72-1  = 1ms 
中断周期为1ms
//  TIM3_Int_Init(5000-1,7200-1);//10Khz的计数频率，计数到5000为500ms 
// 中断/溢出  时间 t=  (  72M /(psc+1)  )  *  (  arr+1  )
****************/

//-----------------------------------------------------------------
//----------------------------------------------------------------
/*配置TIM1输出的PWM信号的模式，如周期、极性、占空比 */
void TIM1_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	u16 CCR1_Val = 0;        
	u16 CCR2_Val = 0;
	//u16 CCR3_Val = 50000;


/* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;//定时器周期,20ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;//时钟预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数方式：向上计数

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

/* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//选择定时器模式：tim脉宽调制2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//极性
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

/* PWM1 Mode configuration: Channel2 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;//****
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);	

/* PWM1 Mode configuration: Channel3 */
/*
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;//开反向通道
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  

    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);	*/

    TIM_ARRPreloadConfig(TIM1, ENABLE);

    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

void TIM1_Int_Init()/* 配置TIM1复用输出PWM时用到的I/O  */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOB, ENABLE);//定时器1时钟使能//GPIOB时钟使能

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 ;//| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*配置和初始化TIM1 */
void TIM1_PWM_Init(void)
{
	TIM1_Int_Init();
	TIM1_Mode_Config();	
}

//-----------------------------------------------------------------
//----------------------- TIM2 ---------------------------------
void TIM2_Int_Init(u16 arr,u16 psc)
{
//定时器TIM2初始化   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//中断优先级NVIC设置
   NVIC_InitTypeDef NVIC_InitStructure; 
    
//定时器TIM2初始化   **************************************************  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);//时钟使能
    
    TIM_DeInit(TIM2);//复位定时器2所有寄存器
    
    TIM_TimeBaseStructure.TIM_Period=arr;	//自动重装载寄存器周期的值
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	//时钟预分频数 72M/72      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
/*位9:8 CKD[1:0]: 时钟分频因子定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)使用的采样频率之间的分频比例。*/
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
   
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	// 清除溢出中断标志 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//使能指定的TIM2中断,允许更新中断

//中断优先级NVIC设置   **************************************************     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //设置NVIC中断分组0:4位抢占优先级，4位响应优先级 
  
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	   //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级3级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能   
    NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
 
    TIM_Cmd(TIM2, ENABLE);	// 开启时钟    
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//先关闭等待使用      
    /*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//先关闭  //使能命令  使能TIME2的时钟
    TIM_Cmd(TIM2, ENABLE);	// 开启时钟      
*/
}

//-----------------------------------------------------------------
//----------------------------------------------------------------
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
//定时器TIM3初始化   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//中断优先级NVIC设置
   NVIC_InitTypeDef NVIC_InitStructure; 
    
//定时器TIM3初始化   **************************************************  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
    TIM_DeInit(TIM3);//复位定时器3所有寄存器

    TIM_TimeBaseStructure.TIM_Period=arr;	//自动重装载寄存器周期的值
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	//时钟预分频数 72M/72      
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
/*位9:8 CKD[1:0]: 时钟分频因子定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)使用的采样频率之间的分频比例。*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);	// 清除溢出中断标志 
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能指定的TIM3中断,允许更新中断

 //中断优先级NVIC设置   **************************************************     
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx		// 开启时钟    
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);	//先关闭等待使用      
    /*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);TIM_Cmd(TIM3, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM3, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE);	//先关闭  //使能命令  使能TIME2的时钟
    TIM_Cmd(TIM3, ENABLE);	// 开启时钟      
*/
}

//-----------------------------------------------------------------
//----------------------------------------------------------------
//定时器1中断服务程序
void TIM1_IRQHandler(void)   //TIM1中断
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //检查TIM1更新中断发生与否
    {
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx更新中断标志 
            //////////***//////////////
//         printf(" //定时器 1 中断服务程序\r\n");            
            //////////***//////////////
	}
}

//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
            //////////***//////////////
//         printf(" //定时器 2 中断服务程序\r\n");            
            //////////***//////////////
	}
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 

            //////////***//////////////
//         printf(" //定时器 3 中断服务程序\r\n");  
            //////////***//////////////
	}
}


//舵机摆动
void SG90_out(u16 num)//500-2500
{
	TIM1->CCR1 = num;//pwm波高电平持续时间，2ms
}
//舵机摆动
void MG90S_out(u16 num)//500-2500
{
	TIM1->CCR2 = num;//pwm波高电平持续时间，2ms
}

