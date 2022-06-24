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
/*把 STM32 的优先级分组的 4 个位均设成抢占优先级，也就是说完全放弃亚优先级。为何要这么设置？
其实这得怪 FreeRTOS 机构里面被驴踢过的逗逼，这些逗逼为了自己省事，直接默认不使用亚优先级   */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//但是在移植 FreeRTOS 时必须要修改成优先级分组方式 4	 
	delay_init(72);
    	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // 不使用JTAG调试，对应的IO口作为普通IO口使用
    	OLED_Init();//初始化OLED     SCL--PA5    SDA--PA7
    	OLED_DrawBMP(0,0,128,8,BMP1);//默认  
	/* 中断/溢出  时间 Tout us= ((arr+1)*(psc+1))/Tclk  */
	// TIM2 中断/溢出  时间 Tout us=   1000 * 72 / 72 = 1000 us
	TIM2_Int_Init(1000-1,720-1);// 10ms中断
	TIM1_PWM_Init(20000-1,72-1); //舵机的控制   用来产生PWM 频率  20ms = 50hz. 
	uart1_init(115200);//语音                                                           USART1_TX PA.9  RX  PA.10
	uart2_init(115200);//蓝牙 Android   												PA2 TXD2        PA3 RXD2   #&0001%
	uart3_init(9600);//舵机控制板                                                       PB10  TXD3      PB11 RXD3 
    	ADC1_Mode_init( );///stm32_adc转换，模拟输入端为  													PB0
	DHT11_Init();//																						PA11
   	DS18B20_Init();// 																					PB9 

	while(1) {
	/* test */
    	}
}
