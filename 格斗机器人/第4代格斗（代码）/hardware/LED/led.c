#include "led.h"


//初始化PD2为输出口.并使端口时钟		    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initsturct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTC时钟
	GPIO_Initsturct.GPIO_Mode=GPIO_Mode_Out_PP;//PC.13推挽输出
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOC,&GPIO_Initsturct);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);    //PC.13输出低 
	RCC->APB2ENR|=1<<5;    	 
	GPIOD->CRL&=0XFFFFF0FF;
	GPIOD->CRL|=0X00000300;
	GPIOD->ODR|=1<<2;      
}




