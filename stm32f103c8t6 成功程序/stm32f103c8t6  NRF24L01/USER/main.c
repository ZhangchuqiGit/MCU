#include <stm32f10x.h>
#include "sys.h"
#include "math.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"		

//#include "timer.h"

//#include "oled.h"
#include "ds18b20.h"
#include "dht11.h"
#include "24l01.h" 	 

int main(void)
{
	SystemInit();//ϵͳʱ������,����ϵͳʱ��Ϊ72M	
	delay_init(72);//��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//	JTAG_SWD_GPIO_Config();//��ʹ��JTAG���ԣ���Ӧ��IO����PB3,PB4,PA15������Ϊ��ͨIO��ʹ�ã�������ʹ��SWD�������¼��������ͷų���
	
	uart1_init(115200);// USART1_TX PA.9     RX PA.10

//	OLED_Init();//��ʼ��OLED  
//	DS18B20_Init();
//	DHT11_Init();//��ʼ��DHT11
	
 	NRF24L01_Init();//��ʼ��NRF24L01 
	while(NRF24L01_Check())
	{
		printf(" NRF24L01_Check Error\r\n");
		delay_ms(500);
	}	
	printf(" NRF24L01_Check OK ++ \r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);

	while(1) 
	{    
//		DS18B20_tmp=DS18B20_Get_Temp( );
//		num[0]=DS18B20_tmp/1000;
//		num[1]=DS18B20_tmp/100%10;
//		num[2]=DS18B20_tmp/10%10;
		/**/		
		NRF24L01_TX_Mode();//TXģʽ
		printf(" NRF24L01_TX ģʽ \r\n");	    				 
		while(1)
		{	  		   				 
			if(NRF24L01_TxPacket("asdfghjkl")==TX_OK)//����һ����������
			{
				printf(" RX: asdfghjkl \r\n");
				break;
			}
			else
			{										   	
				printf(" Send Failed \r\n");
				delay_ms(1000);
			}
		}
		/**/
		NRF24L01_RX_Mode();//RXģʽ
		printf(" NRF24L01_RX ģʽ \r\n");	    				 
		while(1)
		{		
			if(NRF24L01_RxPacket(RT_buf_24l01)==RX_OK)////����һ����������
			{
				RT_buf_24l01[32]=0;//�����ַ���������
				printf(" RX: %s \r\n",RT_buf_24l01);
			}
			else
			{
				//delay_us(100);	
			}
			if(RT_buf_24l01[0]=='t')
			{
				break;
			}
		}
		/**/
	}	  
}


//		OLED_DrawBMP(0,4,31,8,BMP1);
//		OLED_ShowCHinese_16X24(0+36,5,num[0]);
//		OLED_ShowCHinese_16X24(16+36,5,num[1]);
//		OLED_ShowChar(32+36,6,'.',16);
//		OLED_ShowCHinese_16X24(36+36,5,num[2]);
//	
//		DHT11_Read_Data(DHT11_tmp);
//			
//		OLED_ShowCHinese_16X16(60,2,10);
//		OLED_ShowCHinese_16X24(80,2,DHT11_tmp[0]/10);
//		OLED_ShowCHinese_16X24(100,2,DHT11_tmp[0]%10);
//		
//		delay_ms(10);
