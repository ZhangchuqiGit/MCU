/***************STM32F103C8T6**********************
 * 文件名  ：hx711.c
 * 描述    ：hx711称重传感器对应的放大AD转换
 * 接口    ：PB13-DT  PB12-SCK

********************LIGEN*************************/

#include "stm32f10x.h"
#include "hx711.h"
#include "delay.h"

#define DT GPIO_Pin_13
#define SCK GPIO_Pin_12

//初始化hx711对应接口
void hx711_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
	//配置DT端口
	GPIO_InitStructure.GPIO_Pin = DT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//配置SCK端口
	GPIO_InitStructure.GPIO_Pin = SCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//读取传感器数据
unsigned long hx711_read()
{
	unsigned long num = 0;
	unsigned char i;
	
	GPIO_SetBits(GPIOB,DT);//DT置1
	Delay_us(1);
	GPIO_ResetBits(GPIOB,SCK);//SCK置0
	Delay_us(1);
	while(GPIO_ReadInputDataBit(GPIOB,DT));//转换完成
	Delay_us(1);
	//读取数据
	for(i=0;i<24;i++)//128倍增益
	{
		GPIO_SetBits(GPIOB,SCK);
		num = num<<1;
		Delay_us(1);
		GPIO_ResetBits(GPIOB,SCK);
		Delay_us(1);
		if(GPIO_ReadInputDataBit(GPIOB,DT))
			num++;
	}
	
	GPIO_SetBits(GPIOB,SCK);//第25个脉冲完成数据转换
	num = num^0x800000;
	Delay_us(1);
	GPIO_ResetBits(GPIOB,SCK);
	Delay_us(1);
	return num;
}


