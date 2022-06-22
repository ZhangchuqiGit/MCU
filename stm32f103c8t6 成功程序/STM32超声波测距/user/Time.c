#include"Time.h"

void NVIC_TimeConfig(void)
{
	/*定义定时器中断优先级数组*/
	NVIC_InitTypeDef NVIC_InitStruct;

	/*设置中断优先级分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/*设置优先级*/
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
  	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
 	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}
void TIM3_Config(void)
{
	/*定义初始化定时器结构体*/
	TIM_TimeBaseInitTypeDef TIM_InitStruct;
	/*设置中断优先级*/
	NVIC_TimeConfig();

	/*打开定时器时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	/*清除中断标志*/
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	/*配置定时器*/	
	TIM_InitStruct.TIM_Period=1000;						//初值
	TIM_InitStruct.TIM_Prescaler=8999; 				//预分频
	TIM_InitStruct.TIM_ClockDivision=0;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;	//向上
	TIM_TimeBaseInit(TIM3,&TIM_InitStruct);

	/*使能TIM中断*/
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	/*使能定时器*/
	TIM_Cmd(TIM3,ENABLE);	 
}
void TIM4_Config(void)
{
	/*定义初始化定时器结构体*/
	TIM_TimeBaseInitTypeDef TIM_InitStruct;

	/*打开定时器时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	/*配置定时器*/	
	TIM_InitStruct.TIM_Period=65535;						//初值
	TIM_InitStruct.TIM_Prescaler=71; 						//预分频
	TIM_InitStruct.TIM_ClockDivision=0;
	TIM_InitStruct.TIM_CounterMode=TIM_CounterMode_Up;		//向上
	TIM_TimeBaseInit(TIM4,&TIM_InitStruct);
	/*使能定时器*/
	//TIM_Cmd(TIM4,ENABLE);	 
}
