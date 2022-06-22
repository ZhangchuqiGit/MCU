#include"stm32f10x.h"
#include"SysTick.h"
#include"TFT_Drive.h"
#include"GUI.h"
#include"Uitr.h"
#include"Time.h"


void Count(void);
void LED_GPIO_Config(void);

uint32_t Distance;
uint16_t Time;
uint8_t ShowData[5]={0,0,0,0,0};
int main(void)		 	
{ 

	TFT_Init();
	Uitr_GPIO_Config();
	LED_GPIO_Config();	 
	GUI_ClearScreen(BLACK);
 	GUI_WriteCHAR(60,55,"www.prechin.com",RED,BLACK);
  	GUI_WriteCNChar(56,26,"普中科技",RED,BLACK);
	GUI_WriteCHAR(0,300,"Distance:",RED,BLACK);
  	TIM4_Config();
  	TIM3_Config();
	while(1)
	{
		while(ECHO_IN != 1);
		TIM_Cmd(TIM4,ENABLE);						  //计数器开始计数
		while(ECHO_IN == 1);
		TIM_Cmd(TIM4,DISABLE);						  //计数器停止计数
		Count();									  //计算距离 

	}			
}

void Count(void)
{
	Time = TIM4->CNT;								  //读计数器值
	TIM4->CNT = 0;									  //清除计数器
	if(Time>23530)									  //判断是否在4M范围，超出显示ERRO
	{
		GUI_WriteCHAR(72,300,"erro",RED,BLACK);
	}
	else
	{
		Distance = (uint32_t)(Time*340/2/1000) ;	  //计算出来的是MM		
		ShowData[0]	= (Distance%10000/1000)+'0';
		ShowData[1]	= (Distance%1000/100)+'0';
		ShowData[2]	= (Distance%100/10)+'0';
		ShowData[3]	= (Distance%10/1)+'0';

		GUI_WriteCHAR(72,300,ShowData,RED,BLACK);	  //显示距离值
	}
	
}



