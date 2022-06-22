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
  	GUI_WriteCNChar(56,26,"���пƼ�",RED,BLACK);
	GUI_WriteCHAR(0,300,"Distance:",RED,BLACK);
  	TIM4_Config();
  	TIM3_Config();
	while(1)
	{
		while(ECHO_IN != 1);
		TIM_Cmd(TIM4,ENABLE);						  //��������ʼ����
		while(ECHO_IN == 1);
		TIM_Cmd(TIM4,DISABLE);						  //������ֹͣ����
		Count();									  //������� 

	}			
}

void Count(void)
{
	Time = TIM4->CNT;								  //��������ֵ
	TIM4->CNT = 0;									  //���������
	if(Time>23530)									  //�ж��Ƿ���4M��Χ��������ʾERRO
	{
		GUI_WriteCHAR(72,300,"erro",RED,BLACK);
	}
	else
	{
		Distance = (uint32_t)(Time*340/2/1000) ;	  //�����������MM		
		ShowData[0]	= (Distance%10000/1000)+'0';
		ShowData[1]	= (Distance%1000/100)+'0';
		ShowData[2]	= (Distance%100/10)+'0';
		ShowData[3]	= (Distance%10/1)+'0';

		GUI_WriteCHAR(72,300,ShowData,RED,BLACK);	  //��ʾ����ֵ
	}
	
}



