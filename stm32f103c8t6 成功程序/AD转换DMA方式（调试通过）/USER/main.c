/***************STM32F103C8T6**********************
 * 文件名  ：main.c
 * 描述    : ADC
 * 实验平台：STM32F103C8T6
 * 备注    ：模数转换
 * 接口    ：PA0，PA1，PA2

********************LIGEN*************************/


#include "stm32f10x.h"
#include "adc.h" 
#include "led.h"
#include "usart1.h"
#include "timer3.h"


extern __IO u16 ADC_ConvertedValue[3];	
volatile u32 time3; // ms 计时变量
double tem;

int main(void)
{ 
  SystemInit();//配置系统时钟为72M	
  USART1_Config();//配置串口
  ADC1_Init();//初始化adc
	LED_GPIO_Config();//led初始化

	//TIM3初始化
	TIM3_NVIC_Configuration();
	TIM3_Configuration();
	
	START_TIME;
  printf(" -------这是一个ADC实验------\r\n");

  while (1)
  {
		while(time3 == 1000)
		{
			time3 = 0;
			LED_Toggle();
			//printf("ad1: %d \r\n",ADC_ConvertedValue[0]);
			//printf("ad2: %d \r\n",ADC_ConvertedValue[1]);
			//printf("ad3: %d \r\n",ADC_ConvertedValue[2]);
			tem = (float)ADC_ConvertedValue[3]*3.3/4096;//转化为电压值
			tem = (1.43-tem)/0.0042+25;//转化为温度值
			printf("tem: %lf \r\n",tem);
		}			
  }
}



