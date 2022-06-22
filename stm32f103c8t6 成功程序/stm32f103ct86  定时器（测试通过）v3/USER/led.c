
//**************************************
/***************STM32F103C8T6**********************
 * 文件名  ：led.c
 * 描述    : GPIO_Pin_8控制
 * 实验平台：STM32F103C8T6
 * 备注    ：实现板载GPIO_Pin_8的亮、灭、闪烁
 * 接口    ：PB12

********************LIGEN*************************/

#include "led.h"


 /***************  配置I/O口 *******************/
void LED_GPIO_Config(void)	
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); // 使能PC端口时钟  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PC端口
    
//    GPIO_SetBits(GPIOB, GPIO_Pin_8);//输出高	 // GPIO_ResetBits
//    GPIO_SetBits(GPIOB, GPIO_Pin_9);//输出1	 // GPIO_ResetBits
    GPIO_ResetBits(GPIOB, GPIO_Pin_8);//输出0
    GPIO_ResetBits(GPIOB, GPIO_Pin_9);//输出0
    //  GPIO_SetBits可以对多个IO口同时进行置位1 

 /*   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
    GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
    GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
*/
    
}


void LED_Toggle(void)//led翻转电平
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8))));//led电平翻转
}
void LED_On()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8,(BitAction)0);//GPIO_Pin_8 = GPIO_Pin_8
}
void LED_Off()
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_8,(BitAction)1);//GPIO_WriteBit是对一个IO口进行写操作，可以是写0或者写1
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
void LED_Twinkle()//led闪一下
{
	LED_On();
	Delayms(10);
	LED_Off();
}
