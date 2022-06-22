/***************STM32F103C8T6**********************/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
#include "math.h"

#include "oled.h"
#include "ds18b20.h"

int main(void)
{ 
	u16 tmp=0;
	u8 num[4];
	delay_init(72);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // ��ʹ��JTAG���ԣ���Ӧ��IO����Ϊ��ͨIO��ʹ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

  	uart1_init(115200);// USART1_TX PA.9     RX PA.10
	
    OLED_Init();//��ʼ��OLED                 SCL--PA5             SDA--PA7
//    OLED_DrawBMP(0,0,128,8,BMP1);//128��64  
	OLED_Clear( ); 
	DS18B20_Init();
		DS18B20_Rst();
// 	while(DS18B20_Init())	//DS18B20��ʼ��	  PB9
//	{
//		printf("DS18B20 Error");
//		delay_ms(500);
//	}   
	OLED_ShowCHinese_16X24(0,5,0);
	OLED_ShowCHinese_16X24(16,5,1);
	OLED_ShowCHinese_16X24(32,5,2);
	
	OLED_ShowCHinese_16X24(16+48,5,3);
	OLED_ShowCHinese_16X24(16+64,5,4);
	OLED_ShowCHinese_16X24(16+80,5,5);
	OLED_ShowCHinese_16X24(16+96,5,6);
	
	OLED_ShowCHinese_16X24(20+76,2,4);
	while(1) 
    {
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
//short DS18B20_Get_Temp(void)
		tmp=DS18B20_Get_Temp( );
		printf("%d\t\t",tmp);	
				
		num[0]=tmp/1000;
		num[1]=tmp/100%10;
		num[2]=tmp/10%10;
		num[3]=tmp%10;
		
		OLED_ShowCHinese_16X24(20+0,2,num[0]+9);
		OLED_ShowCHinese_16X24(20+16,2,num[1]+9);
		OLED_ShowChar(20+32,4,'.',8);
		OLED_ShowCHinese_16X24(20+40,2,num[2]+9);
		OLED_ShowCHinese_16X24(20+56,2,num[3]+9); 
	
		OLED_ShowString(0,0," T: 2018-12-07",16);	   
		
		delay_ms(10);	 	
//		OLED_Clear( ); 
   }
}
/*�� STM32 �����ȼ������ 4 ��λ�������ռ���ȼ���Ҳ����˵��ȫ���������ȼ���Ϊ��Ҫ��ô���ã�
��ʵ��ù� FreeRTOS �������汻¿�߹��Ķ��ƣ���Щ����Ϊ���Լ�ʡ�£�ֱ��Ĭ�ϲ�ʹ�������ȼ�   */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//��������ֲ FreeRTOS ʱ����Ҫ�޸ĳ����ȼ����鷽ʽ 4	 
//	/* �ж�/���  ʱ�� Tout us= ((arr+1)*(psc+1))/Tclk  */
//	// TIM2 �ж�/���  ʱ�� Tout us=   1000 * 72 / 72 = 1000 us
//	TIM2_Int_Init(1000-1,720-1);// 10ms�ж�
//	TIM1_PWM_Init(20000-1,72-1); //����Ŀ���   ��������PWM Ƶ��  20ms = 50hz. 
//	uart1_init(115200);//����                                                           USART1_TX PA.9  RX  PA.10
//	uart2_init(115200);//���� Android   												PA2 TXD2        PA3 RXD2   #&0001%
//	uart3_init(9600);//������ư�                                                       PB10  TXD3      PB11 RXD3 
