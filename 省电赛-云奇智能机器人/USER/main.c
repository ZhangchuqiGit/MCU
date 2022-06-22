/***************STM32F103C8T6**********************/

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
#include "math.h"

#include "adc.h"
#include "oled.h"
#include "dht11.h"
#include "adc.h"

#include "ds18b20.h"
#include "timer_asmx_pwm.h"


int main(void)
{ 
/*�� STM32 �����ȼ������ 4 ��λ�������ռ���ȼ���Ҳ����˵��ȫ���������ȼ���Ϊ��Ҫ��ô���ã�
��ʵ��ù� FreeRTOS �������汻¿�߹��Ķ��ƣ���Щ����Ϊ���Լ�ʡ�£�ֱ��Ĭ�ϲ�ʹ�������ȼ�   */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//��������ֲ FreeRTOS ʱ����Ҫ�޸ĳ����ȼ����鷽ʽ 4	 
	delay_init(72);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // ��ʹ��JTAG���ԣ���Ӧ��IO����Ϊ��ͨIO��ʹ��
    OLED_Init();//��ʼ��OLED     SCL--PA5    SDA--PA7
    OLED_DrawBMP(0,0,128,8,BMP1);//Ĭ��  
	/* �ж�/���  ʱ�� Tout us= ((arr+1)*(psc+1))/Tclk  */
	// TIM2 �ж�/���  ʱ�� Tout us=   1000 * 72 / 72 = 1000 us
	TIM2_Int_Init(1000-1,720-1);// 10ms�ж�
	TIM1_PWM_Init(20000-1,72-1); //����Ŀ���   ��������PWM Ƶ��  20ms = 50hz. 
	uart1_init(115200);//����                                                           USART1_TX PA.9  RX  PA.10
	uart2_init(115200);//���� Android   												PA2 TXD2        PA3 RXD2   #&0001%
	uart3_init(9600);//������ư�                                                       PB10  TXD3      PB11 RXD3 
    ADC1_Mode_init( );///stm32_adcת����ģ�������Ϊ  													PB0
	DHT11_Init();//																						PA11
    DS18B20_Init();// 																					PB9 

	while(1) 
    {
	

    }
}
