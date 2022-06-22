/***************STM32F103C8T6**********************
 * 文件名  ：timer2.c
 * 描述    : TIM2定时器
 * 实验平台：STM32F103C8T6
 * 备注    ：tim2外部计数模式
 * 接口    ：PA0

********************LIGEN*************************/
#include "timer2.h"
#include "misc.h"

/* TIM2 外部端口设置*/
void TIM2_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//设置结构体
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//设置端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//设置端口模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//设置端口频率
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}


/*中断周期为1ms*/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);//设置RCC
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;	//自动重装载寄存器周期的值 
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	//时钟预分频数 72M/1
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;//  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);//ETR外部计数
	//关闭预分频TIM_ExtTRGPSC_OFF
	//外部出发极性不反向，高电平或上升沿有效TIM_ExtTRGPolarity_NonInverted
	//外部触发无滤波器
    
	TIM_SetCounter(TIM2,0); //设置计数初始值为0
	TIM_Cmd(TIM2, ENABLE);	// 开启时钟    
}
