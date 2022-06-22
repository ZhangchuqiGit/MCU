
#include "userr.h"

void NRF_check(void);

/*------------------------------------------------------*/
int main(void)
{                  
	u8 i=0;
	u8 tt_buf[7][4]={0};   
	u8 ti_n5um=0;
	u16 ti_num=0;
	Stm32_Clock_Init(9); //系统时钟设置 
	JTAG_Set(SWD_ENABLE);
	delay_init(); //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
//	uart_init(115200); //串口初始化为115200
#if  ooioio
	LCD_Init();
	LCD_Display_Dir(1);//设置屏幕显示方向	
	LCD_Clear(YELLOW); //清屏
	Picture_Draw(0,0,(u8 *) gImage_0 );//在指定座标范围显示一副图片
 	tp_dev.init();//触摸屏驱动
#endif
 	LED_Init(); //LED端口初始化 LED1,LED2	
#if  1
	NRF24L01_Init();    		//初始化NRF24L01 
	while(NRF24L01_Check())
	{		
		ti_num++;
		if(ti_num>=500)break;
	}
	if(ti_num>=500)
	{        
		nrf_gg=0;
		NRF_Check=1;
	}
	else NRF_Check=2;
	NRF24L01_RX_Mode();//RX模式  
	ti_num=25;
	while(ti_num--)
	{
		LED2=!LED2;
		delay_ms(150);
	}
	ti_num=0;
#endif
	LED2=1;
	LED1=1;
	TX_RX=1;//RX模式
#if  ooioio
	display_main(); 
	POINT_COLOR=MAGENTA;//设置字体 画笔颜色
#endif
#if  0
	for(i=0;i<7;i++)num_char(tt_buf[i],"12345678912345",i);
	while(1)
	{
		cmp_sensor(0,0,tt_buf[0],1000);
		delay_ms(100);
	}
#else
	while(1)
	{
#if  ooioio
		ti_num++; // 0~42,9496,7295
		if(ti_num%20000==0)
		{
			if(NRF24L01_Check()) 
			{
				NRF_Check=1;
			}
			else 
			{
				NRF_Check=2;	
			}
		}
		if(TX_RX==1)//RX模式  
		{
			NRF24L01_RX_Mode();//RX模式  
			TX_RX=3;
		}		    		    				 
		if(TX_RX==2)//TX模式  
		{
			NRF24L01_TX_Mode();//TX模式  
			TX_RX=4;			
		}	
		if(TX_RX==3)	   //RX模式
		{
			RX_();//RX模式  
		}
		if(TX_RX==4)      //TX模式  
		{
			TX_();//TX模式
		}
		rtp_test();//电阻屏测试	
		if( ti_num%10000==0 && flag_func==2 )display_sensor_data();
		if( ti_num%20000==0 && (flag_func==1||flag_func==2) ) NRF_check(); 
		if( flag_func==1||flag_func==3 ) 
		{
			if( ledf==1 )
			{
				LED1=!LED1;
				display_led_data();
				ledf=0;
				TX_RX=2;//TX模式
			}
		}
		if( bnb[0]==1 && bnb[1]==1 )
		{		
			if(ti_n5um==0)
			{
				ti_n5um=1;
				NRF_01(0);//  1:RX模式      ;    0:TX模式  
			}
			if( NRF24L01_TxPacket("-")==TX_OK )
			{
				ti_n5um=0;
				bnb[0]=0;
				bnb[1]=0;
				TX_RX=1;//RX模式
				LED1=1;
				NRF_01(1);//  1:RX模式      ;    0:TX模式  
				switch(flag_func)
				{
					case 1:display_main();
						break;
					case 2:display_sensor_data();
						break;
					case 3:display_col();
						break;
				}
			}
			else {}	
		}
		if( nrf_d==1 )
		{
			nrf_d=0;
			TX_RX=1;//RX模式
		}
#else 
		delay_ms(100);		
#endif
	}
#endif
}

