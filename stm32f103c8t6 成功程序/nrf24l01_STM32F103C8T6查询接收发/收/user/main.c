
#include "includes.h"

int main()
{	
     u8 tmp_buf[3],i;
		  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
     delay_init();	    	 //延时函数初始化
     TIM4_Init(9,7199);//Tout（溢出时间）=（ARR+1)(PSC+1)/Tclk =10*7200/72000000s=1ms
     LED_Init();
	USART1_Init(115200,0);	 //串口初始化为115200
     delay_us(500);
 NRF24L01_Init();    		//初始化NRF24L01 

	while(NRF24L01_Check())
	{
		printf("NRF24L01 Error\r\n");
 		delay_ms(1000);
	}
       printf("NRF24L01 OK\r\n");
delay_ms(1000);
          NRF24L01_RX_Mode(); 
delay_ms(1000);delay_ms(1000);
		while(1)
		{	  		   				 
			while(NRF24L01_RxPacket(tmp_buf)!=RX_OK)
			{
	               // printf("Rx Failed\r\n"); 
			}
                       // printf("Rx ok\r\n");
                         for(i=0;i<3;i++)
                            printf("%d,",tmp_buf[i]);
                         printf("\r\n"); 
//               else
//			{										   		   
//				
//			}
           }




}


