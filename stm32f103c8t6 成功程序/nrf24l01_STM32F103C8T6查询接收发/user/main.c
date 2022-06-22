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
void JTAG_SWD_GPIO_Config(void);//stm32的PB3 PB4 PA15引脚可以在使用SWD仿真和烧录的情况下释放出来，作为普通IO口使用        

//串口几用printf在sys.c中设置，默认串口1可用printf 
int main()                    
{	
	u64 xitong_haomiao_printf_old;  

	delay_init();          
    JTAG_SWD_GPIO_Config( );//stm32的PB3 PB4 PA15引脚可以在使用SWD仿真和烧录的情况下释放出来，作为普通IO口使用

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
			USART1_printf("USART1_Read %3d个:%s\r\n",(USART1_RX_STA&0x7fff),USART1_RX_BUF);
			USART1_RX_STA=0;
		}
		if(USART2_RX_STA&0x8000)
		{		
			USART2_printf("USART2_Read %3d个:%s\r\n",(USART2_RX_STA&0x7fff),USART2_RX_BUF);
			USART2_RX_STA=0;
		}
		if(USART3_RX_STA&0x8000)
		{		
			USART3_printf("USART3_Read %3d个:%s\r\n",(USART3_RX_STA&0x7fff),USART3_RX_BUF);
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


//	JTAG_SWD_GPIO_Config();//不使用JTAG调试，对应的IO口如PB3,PB4,PA15引脚作为普通IO口使用，可以在使用SWD仿真和烧录的情况下释放出来
void JTAG_SWD_GPIO_Config(void)//stm32的PB3 PB4 PA15引脚可以在使用SWD仿真和烧录的情况下释放出来，作为普通IO口使用
{
 /*   //打开重映射时钟和USART重映射后的I/O口引脚时钟
    //使能对应IO口的时钟，因为是复用功能所以还要使能AFIO时钟    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//使能GPIOA时钟 //在时钟配置中打开复用时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //对PA13引脚进行重映射
    
  //  AFIO->MAPR=0;
    AFIO->MAPR&=0XF8FFFFFF;
    AFIO->MAPR|=0x0C ;*/
    
    /*这段初始化代码放在所有IO初始化最后！！！！！！
    当把PB3 PB4 PA15(JTAG=SWD+ PB3+PB4+PA15)初始化好之后，如果再次调用GPIOA GPIOB接口的时钟代码的话！
	会使得PB3 PA15引脚变回JTAG的引脚即初始化完之后不能再执行如下代码，PB3 PA15引脚会变回JTAG的引脚，而无法作为普通IO使用*/
    GPIO_InitTypeDef GPIO_InitStructure;  
  
    //打开重映射时钟和USART重映射后的I/O口引脚时钟
    //使能对应IO口的时钟，因为是复用功能所以还要使能AFIO时钟    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//重映射，不使用JTAG调试，对应的IO口作为普通IO口使用	
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);//重映射，不使用JTAG调试，对应的IO口作为普通IO口使用	

/********------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推免输出   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);//设置 0  

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


