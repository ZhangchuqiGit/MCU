#include"Uitr.h"


void Uitr_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;						//定义GPIO初始化结构体
								  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//打开GPIOA的时钟
	
	/*TRIG*/
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;				//选择要设置的管脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;			//设置GPIO模式
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;		//设置GPIO速度
	GPIO_Init(GPIOB,&GPIO_InitStruct);				    //IO初始化 
	
	/*ECHO*/
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;				//选择要设置的管脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;			//设置GPIO模式
	GPIO_Init(GPIOB,&GPIO_InitStruct);				    //IO初始化 
}
