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
#include "led.h"
#include "SYN6288.h"

#include "ds18b20.h"
#include "timer_asmx_pwm.h"

int main(void)
{ 
/*把 STM32 的优先级分组的 4 个位均设成抢占优先级，也就是说完全放弃亚优先级。为何要这么设置？
其实这得怪 FreeRTOS 机构里面被驴踢过的逗逼，这些逗逼为了自己省事，直接默认不使用亚优先级   */
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//但是在移植 FreeRTOS 时必须要修改成优先级分组方式 4	 
	delay_init(72);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // 不使用JTAG调试，对应的IO口作为普通IO口使用
//	LED_Init();	LED = 1;
	
    TTS_Init( );
	uart1_init(9600);//语音				USART1_TX PA.9  RX  PA.10
    Speech_bound(9600); //设置通讯波特率命令(初始波特率为9600bps)
//	uart1_init(19200);//语音					USART1_TX PA.9  RX  PA.10
//	TIM2_Int_Init(1000-1,720-1);// 10ms中断
//	uart2_init(115200);//蓝牙 Android		PA2 TXD2        PA3 RXD2   #&0001%
//	uart3_init(9600);//舵机控制板			PB10  TXD3      PB11 RXD3 

//    OLED_Init();//初始化OLED                                            		+f   SCL--PA5             SDA--PA7
//    OLED_DrawBMP(0,0,128,8,BMP1);//默认  
//	/* 中断/溢出  时间 Tout us= ((arr+1)*(psc+1))/Tclk  */
//	// TIM2 中断/溢出  时间 Tout us=   1000 * 72 / 72 = 1000 us
//	runActionGroup(0,2); //
	while(1) 
    {
//		if(USART2_led==1)
//		{
//			yuying_Android( );//语音对话
//			USART2_led=0; 
		Speech("[v16],msgd",0 );
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
//		}
    }
}


