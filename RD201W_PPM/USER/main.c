
#include <stm32f10x.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "math.h"
#include "stdlib.h"
#include "timer.h"
#include "GPIO_JTAG.h"
#include "led.h"


int main(void)
{
	u16 num=500;
	u8 flag=0;
	SystemInit();//系统时钟设置,配置系统时钟为72M	
	delay_init(72);//延时初始化  
	JTAG_SWD_GPIO_Config();//不使用JTAG调试，对应的IO口如PB3,PB4,PA15引脚作为普通IO口使用，可以在使用SWD仿真和烧录的情况下释放出来
	/*  中断优先级NVIC设置  */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

	uart1_init(115200);//USART1_TX PA.9 ; USART1_RX PA.10 
	LED_Init();
	
/* void TIM3_Int_Init(u16 arr,u16 psc)
   TIM3_Int_Init(5000-1,7200-1);//10Khz的计数频率，计数到5000为50ms 
   中断/溢出  时间 t = ( 72M /(psc+1) ) * ( arr+1 ) = 72M * arr / psc
				   t = ( 72M / 7200 ) * ( 5000 ) = 72M * 5000 / 7200 = 50ms
				   t = ( 72M / 72 ) * ( 20000 ) = 72M * 20 000 / 72 = 20ms  */
//	TIM4_PWM_SG90_MG90_Init();//PB6-9 : TIM4 CH1-4
/* 定时器2 通道2 (PA1) 输入 PPM 捕获配置 */
	TIM2_Capture_PPM_Init();	//以1Mhz的频率计数 
	while(1)
	{
		LED=1;
//		//舵机摆动
//		SG90_MG90_1(num);//500-2500
//		//舵机摆动
//		SG90_MG90_2(num);//500-2500
//		//舵机摆动
//		SG90_MG90_3(num);//500-2500
//		//舵机摆动
//		SG90_MG90_4(num);//500-2500
		delay_ms(200);
		LED=0;
		delay_ms(200);
		printf("zcq : %d\t",num);
		if(flag==0)
		{
			num+=20;
			if(num>=2500)
			{
				flag=1;
				printf("\r\n");
			}
		}
		else
		{
			num-=20;
			if(num<=500)
			{
				flag=0;	
				printf("\r\n");
			}
		}
		if(ppm_rx[0])//成功捕获到了一次上升沿
		{
			printf("左右:%d 前后:%d 油门:%d 航向:%d chanel5:%d chanel6:%d\r\n",
				ppm_rx[1],ppm_rx[2],ppm_rx[3],ppm_rx[4],ppm_rx[5],ppm_rx[6]);
			ppm_rx[0]=0;
		}
	}
}
