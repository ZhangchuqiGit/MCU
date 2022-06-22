/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    : 电机测速
 * 实验平台：STM32F103C8T6
 * 备注    ：
 * 接口    ：PB0

****************STM32F103C8T6**********************/

#include "gpio_in.h"
#include "stm32f10x_gpio.h"
#include "led.h"

#define SPEED GPIO_Pin_1

//io口输入设置
void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //开启按键端口PB的时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //开启按键端口PA的时钟
	GPIO_InitStructure.GPIO_Pin = SPEED; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //端口配置为浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化端口

}

u8 Rotate(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,SPEED) == 0)
	{
		while(GPIO_ReadInputDataBit(GPIOB,SPEED) == 1)
			return 1;
	}
	else
		return 0;
}



//









