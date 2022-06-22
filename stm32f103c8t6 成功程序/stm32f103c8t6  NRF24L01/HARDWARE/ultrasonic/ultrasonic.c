/*************************************
 * 文件名  ：main.c
 * 描述    ：超声波测距
 * 实验平台：STM32F103C8T6
 * 备注    ：2017.01.08
 * 接口    ：

**********************************************************************************/
#include "ultrasonic.h"

u16 Distance=0; //计算出的距离//前方测距
u16 Distance_B=0; //计算出的距离//后方测距

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void Ultrasonic_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

/*   前方测距    */
	GPIO_InitStructure.GPIO_Pin = Trig;					 //PA1接 TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOA, &GPIO_InitStructure);	                 //初始化外设GPIO 

	GPIO_InitStructure.GPIO_Pin = Echo;				     //PA0接 ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为浮空输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);						 //初始化GPIO
	
/*   后方测距    */
	GPIO_InitStructure.GPIO_Pin = Trig_B;					 //PA1接 TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(GPIOA, &GPIO_InitStructure);	                 //初始化外设GPIO 

	GPIO_InitStructure.GPIO_Pin = Echo_B;				     //PA0接 ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为浮空输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //速度50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure);						 //初始化GPIO
	
//    TIM_Cmd(TIM2, DISABLE);	////定时器2失能
/* 中断/溢出 时间 TIM2 Tout us= ( 1000 * 72 )/ 72 = 1 000 us  */
//	TIM2_Int_Init(10000-1,72-1); 
/* "TIM_GetCounter(TIM2)"->时间计数总数 ?us/10000.0=?ms * "340"->空气速度340m/s= 340mm/ms / "2.0"->往返路程;  */
}
#if 0
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
	TIM_ITConfig(TIM2,TIM_IT_Trigger,ENABLE);//允许更新中断，触发方式中断

//中断优先级NVIC设置   **************************************************     
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //设置NVIC中断分组0:4位抢占优先级，4位响应优先级 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	   //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能   
    NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
 
//    TIM_Cmd(TIM2, ENABLE);	// 开启时钟    
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//先关闭等待使用      
    /*
#define START_TIME  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME   TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)
 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);	//先关闭  //使能命令  使能TIME2的时钟
    TIM_Cmd(TIM2, ENABLE);	// 开启时钟      
*/
}
#endif

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void Ultrasonic_Measure(void)
{
	GPIO_SetBits(GPIOA,Trig);//送>10US的高电平
	delay_us(20);//延时20US
	GPIO_ResetBits(GPIOA,Trig);
	while(!GPIO_ReadInputDataBit(GPIOA,Echo));//等待高电平
	TIM_Cmd(TIM2, ENABLE);//开启时钟
	while(GPIO_ReadInputDataBit(GPIOA,Echo));//等待低电平
	TIM_Cmd(TIM2, DISABLE);//定时器2失能
	
//	TIM2_Int_Init(10000-1,720-1);// 中断/溢出 时间 Tout us= ( 10000==计数次数 * 72 )/ 72 = 100 000 uS
/* "TIM_GetCounter(TIM2)"->时间计数总数 ?us/10000=?ms * "340"->空气速度340m/s= 340mm/ms / "2.0"->往返路程;  */
	Distance=TIM_GetCounter(TIM2)*340/200;
	TIM_SetCounter(TIM2,0);
}
void Ultrasonic_Measure_B(void)//后方测距
{
	GPIO_SetBits(GPIOA,Trig_B);//送>10US的高电平
	delay_us(20);//延时20US
	GPIO_ResetBits(GPIOA,Trig_B);
	while(!GPIO_ReadInputDataBit(GPIOA,Echo_B));//等待高电平
	TIM_Cmd(TIM2, ENABLE);//开启时钟
	while(GPIO_ReadInputDataBit(GPIOA,Echo_B));//等待低电平
	TIM_Cmd(TIM2, DISABLE);//定时器2失能
	
//	TIM2_Int_Init(10000-1,720-1);// 中断/溢出 时间 Tout us= ( 10000==计数次数 * 72 )/ 72 = 100 000 uS
/* "TIM_GetCounter(TIM2)"->时间计数总数 ?us/10000=?ms * "340"->空气速度340m/s= 340mm/ms / "2.0"->往返路程;  */
	Distance_B=TIM_GetCounter(TIM2)*340/200;
	TIM_SetCounter(TIM2,0);
}
