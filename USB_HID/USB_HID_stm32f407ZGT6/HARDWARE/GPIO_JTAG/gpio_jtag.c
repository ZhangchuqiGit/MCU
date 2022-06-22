#include "GPIO_JTAG.h"

/* 用到PB3，PB4，PA15等引脚控制外设。发现不管怎么配置，这三个引脚都不能置零。
发现是包括这三个引脚在内的 PB3，PB4，PA13，PA14，PA15 是特殊的IO口，用作 JTAG/SWD 仿真器的调试接口。
其中 	PA13，PA14 分别作为SWD调试的 SWIO和SWCLK；
		PB3，PB4，PA13，PA14，PA15 共同用于JTAG
这五个IO引脚非常特殊，正常情况下作为SWJ仿真器的调试引脚，如果要作为普通IO口使用需要特别的配置*/

/**stm32的PB3 PB4 PA15引脚可以在使用SWD仿真和烧录的情况下释放出来，作为普通IO口使用**/
void JTAG_SWD_GPIO_Config()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOx时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOB,GPIO_Pin_3);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOB,GPIO_Pin_4);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);  
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}




