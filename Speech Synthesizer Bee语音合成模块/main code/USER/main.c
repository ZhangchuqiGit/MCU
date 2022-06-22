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
/*�� STM32 �����ȼ������ 4 ��λ�������ռ���ȼ���Ҳ����˵��ȫ���������ȼ���Ϊ��Ҫ��ô���ã�
��ʵ��ù� FreeRTOS �������汻¿�߹��Ķ��ƣ���Щ����Ϊ���Լ�ʡ�£�ֱ��Ĭ�ϲ�ʹ�������ȼ�   */
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//��������ֲ FreeRTOS ʱ����Ҫ�޸ĳ����ȼ����鷽ʽ 4	 
	delay_init(72);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // ��ʹ��JTAG���ԣ���Ӧ��IO����Ϊ��ͨIO��ʹ��
//	LED_Init();	LED = 1;
	
    TTS_Init( );
	uart1_init(9600);//����				USART1_TX PA.9  RX  PA.10
    Speech_bound(9600); //����ͨѶ����������(��ʼ������Ϊ9600bps)
//	uart1_init(19200);//����					USART1_TX PA.9  RX  PA.10
//	TIM2_Int_Init(1000-1,720-1);// 10ms�ж�
//	uart2_init(115200);//���� Android		PA2 TXD2        PA3 RXD2   #&0001%
//	uart3_init(9600);//������ư�			PB10  TXD3      PB11 RXD3 

//    OLED_Init();//��ʼ��OLED                                            		+f   SCL--PA5             SDA--PA7
//    OLED_DrawBMP(0,0,128,8,BMP1);//Ĭ��  
//	/* �ж�/���  ʱ�� Tout us= ((arr+1)*(psc+1))/Tclk  */
//	// TIM2 �ж�/���  ʱ�� Tout us=   1000 * 72 / 72 = 1000 us
//	runActionGroup(0,2); //
	while(1) 
    {
//		if(USART2_led==1)
//		{
//			yuying_Android( );//�����Ի�
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


