/***************STM32F103C8T6**********************
 * �ļ���  ��main.c
 * ����    : ADC
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ��ģ��ת��
 * �ӿ�    ��PA0��PA1��PA2

********************LIGEN*************************/


#include "stm32f10x.h"
#include "adc.h" 
#include "led.h"
#include "usart1.h"
#include "timer3.h"


extern __IO u16 ADC_ConvertedValue[3];	
volatile u32 time3; // ms ��ʱ����
double tem;

int main(void)
{ 
  SystemInit();//����ϵͳʱ��Ϊ72M	
  USART1_Config();//���ô���
  ADC1_Init();//��ʼ��adc
	LED_GPIO_Config();//led��ʼ��

	//TIM3��ʼ��
	TIM3_NVIC_Configuration();
	TIM3_Configuration();
	
	START_TIME;
  printf(" -------����һ��ADCʵ��------\r\n");

  while (1)
  {
		while(time3 == 1000)
		{
			time3 = 0;
			LED_Toggle();
			//printf("ad1: %d \r\n",ADC_ConvertedValue[0]);
			//printf("ad2: %d \r\n",ADC_ConvertedValue[1]);
			//printf("ad3: %d \r\n",ADC_ConvertedValue[2]);
			tem = (float)ADC_ConvertedValue[3]*3.3/4096;//ת��Ϊ��ѹֵ
			tem = (1.43-tem)/0.0042+25;//ת��Ϊ�¶�ֵ
			printf("tem: %lf \r\n",tem);
		}			
  }
}



