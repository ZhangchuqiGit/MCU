#include "led.h"

u8 adc_q=0,humi_q=0,Measure_q=0,ds18b20c_q=0,Human_en=0; 

//初始化PD2为输出口.并使端口时钟		    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initsturct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTC时钟
	
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_11;
	GPIO_Initsturct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOB,&GPIO_Initsturct);
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_11);	
}
void Human_infrared_init(void)//人体红外
{
	GPIO_InitTypeDef GPIO_Initsturct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTC时钟
	
	GPIO_Initsturct.GPIO_Pin=GPIO_Pin_12; 
	GPIO_Initsturct.GPIO_Mode= GPIO_Mode_IPD;// 下拉输入
	GPIO_Initsturct.GPIO_Speed=GPIO_Speed_50MHz;//速度50MHz
	GPIO_Init(GPIOB,&GPIO_Initsturct);
}
