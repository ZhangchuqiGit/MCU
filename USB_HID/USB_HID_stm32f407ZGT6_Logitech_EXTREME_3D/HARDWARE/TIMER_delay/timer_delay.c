#include "timer_delay.h"

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
/* STM32F407 ------ 使用定时器实现精确延时
测试环境：主频168M 
定时器 TIM4 时钟 84M，分频系数 0，所以 84M 的计数频率，计数84次为1us */
void TIM4_init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);  ///使能时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 	//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);//初始化
	
	TIM_Cmd(TIM4, DISABLE); //不使能定时器
}

void timer_delay_init(void)
{
	/* 定时器 TIM4 时钟 84M，分频系数 0，所以 84M 的计数频率，计数84次为1us */
	TIM4_init(84-1, 0);
}

//延时nms
//nus为要延时的 ms 数.                                               
void timer_delay_ms(u32 nms)
{    
    nms = nms*1000 - 1;
    TIM_Cmd(TIM4, ENABLE); //使能定时器
    while(nms--)
    {
        while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
    TIM_Cmd(TIM4, DISABLE); //不使能定时器       
}   

//延时nus
//nus为要延时的us数.                                               
void timer_delay_us(u32 nus)
{
    TIM_Cmd(TIM4, ENABLE); //使能定时器
    while(nus--)
    {
        while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
    TIM_Cmd(TIM4, DISABLE); //不使能定时器
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
