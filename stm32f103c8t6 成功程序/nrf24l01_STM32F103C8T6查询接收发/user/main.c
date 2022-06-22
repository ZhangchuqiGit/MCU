#include "sys.h"
#include "delay.h"
#include "usart1.h"	
#include "usart2.h"
#include "usart3.h"
#include "key.h"                         
#include "soft_timer.h"    
#include "multi_button.h"                    
#include "NRF24L01.h"               
							     
									    
u8 ll ;
/*2222222222222222*/
void JTAG_SWD_GPIO_Config(void);//stm32��PB3 PB4 PA15���ſ�����ʹ��SWD�������¼��������ͷų�������Ϊ��ͨIO��ʹ��        

//���ڼ���printf��sys.c�����ã�Ĭ�ϴ���1����printf 
int main()                    
{	
	u64 xitong_haomiao_printf_old;  

	delay_init();          
    JTAG_SWD_GPIO_Config( );//stm32��PB3 PB4 PA15���ſ�����ʹ��SWD�������¼��������ͷų�������Ϊ��ͨIO��ʹ��

	USART1_Init(115200);
//	USART2_Init(115200);
//	USART3_Init(115200);

	NRF24L01_Test();
	while(1)
	{
	}
	
	
	SoftTimer_Init();
	Multi_Button_Init();                        
	Key_Scan_Init();
	
	while(1)
	{
		if(USART1_RX_STA&0x8000)
		{		
			USART1_printf("USART1_Read %3d��:%s\r\n",(USART1_RX_STA&0x7fff),USART1_RX_BUF);
			USART1_RX_STA=0;
		}
		if(USART2_RX_STA&0x8000)
		{		
			USART2_printf("USART2_Read %3d��:%s\r\n",(USART2_RX_STA&0x7fff),USART2_RX_BUF);
			USART2_RX_STA=0;
		}
		if(USART3_RX_STA&0x8000)
		{		
			USART3_printf("USART3_Read %3d��:%s\r\n",(USART3_RX_STA&0x7fff),USART3_RX_BUF);
			USART3_RX_STA=0;
		}
		if(xitong_haomiao-xitong_haomiao_printf_old>=1000)
		{
			xitong_haomiao_printf_old=xitong_haomiao;
			printf("USART1_ms=%llu\r\n",xitong_haomiao/1000);
			USART2_printf("USART2_ms=%llu\r\n",xitong_haomiao/1000);
			USART3_printf("USART3_ms=%llu\r\n",xitong_haomiao/1000);
		}
		if(Key!=KEY_NO_PRES)
		{
			if(Key==KEY0_PRES)
			{
				printf("key0\r\n");
			}
			Key=KEY_NO_PRES;
		}
	}
}


//	JTAG_SWD_GPIO_Config();//��ʹ��JTAG���ԣ���Ӧ��IO����PB3,PB4,PA15������Ϊ��ͨIO��ʹ�ã�������ʹ��SWD�������¼��������ͷų���
void JTAG_SWD_GPIO_Config(void)//stm32��PB3 PB4 PA15���ſ�����ʹ��SWD�������¼��������ͷų�������Ϊ��ͨIO��ʹ��
{
 /*   //����ӳ��ʱ�Ӻ�USART��ӳ����I/O������ʱ��
    //ʹ�ܶ�ӦIO�ڵ�ʱ�ӣ���Ϊ�Ǹ��ù������Ի�Ҫʹ��AFIOʱ��    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//ʹ��GPIOAʱ�� //��ʱ�������д򿪸���ʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //��PA13���Ž�����ӳ��
    
  //  AFIO->MAPR=0;
    AFIO->MAPR&=0XF8FFFFFF;
    AFIO->MAPR|=0x0C ;*/
    
    /*��γ�ʼ�������������IO��ʼ����󣡣���������
    ����PB3 PB4 PA15(JTAG=SWD+ PB3+PB4+PA15)��ʼ����֮������ٴε���GPIOA GPIOB�ӿڵ�ʱ�Ӵ���Ļ���
	��ʹ��PB3 PA15���ű��JTAG�����ż���ʼ����֮������ִ�����´��룬PB3 PA15���Ż���JTAG�����ţ����޷���Ϊ��ͨIOʹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;  
  
    //����ӳ��ʱ�Ӻ�USART��ӳ����I/O������ʱ��
    //ʹ�ܶ�ӦIO�ڵ�ʱ�ӣ���Ϊ�Ǹ��ù������Ի�Ҫʹ��AFIOʱ��    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//��ӳ�䣬��ʹ��JTAG���ԣ���Ӧ��IO����Ϊ��ͨIO��ʹ��	
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);//��ӳ�䣬��ʹ��JTAG���ԣ���Ӧ��IO����Ϊ��ͨIO��ʹ��	

/********------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);//���� 0  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);  
}


