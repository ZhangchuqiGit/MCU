/**************************************
 * 文件名  ：led.c
 *          
 * 实验平台：STM32F103C8T6
 * 硬件连接：LED端口PC13
 * 备注：修改led端口时要同时修改gpio和pin
         同时加入非精确定时以便调试时使用

********************LIGEN*************************/

#include "led.h"

#define LED GPIO_Pin_12

 /***************  配置LED用到的I/O口 *******************/
void LED_GPIO_Config(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PC端口时钟  
  GPIO_InitStructure.GPIO_Pin = LED;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PC端口
  GPIO_SetBits(GPIOB, LED);	 // 关闭所有LED
}
void LED_Toggle(void)
{
	GPIO_WriteBit(GPIOB, LED, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, LED))));//led电平翻转
}
void LED_On()
{
	GPIO_WriteBit(GPIOB, LED,(BitAction)0);
}
void LED_Off()
{
	GPIO_WriteBit(GPIOB, LED,(BitAction)1);
}

void Delayus(u16 i)
{
	unsigned char t = 0;
	for(;i>0;i--)
	{
		for(t=0;t<2;t++)
		{}
	}
}
void Delayms(u16 t)
{
	while(t--)
	{
		Delayus(1000);
	}
}
void LED_Twinkle()
{
	LED_On();
	Delayms(10);
	LED_Off();
}
