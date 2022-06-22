/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  : UltrasonicWave.c
 * ����    �����������ģ�飬UltrasonicWave_Configuration��������
             ��ʼ������ģ�飬UltrasonicWave_StartMeasure��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�------------------
 *          | PC8  - TRIG      |
 *          | PC9  - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
*********************************************************************************/
#include "ultrasonic3.h"
#include "usart1.h"
#include "time_test.h"
#include "stm32f10x_gpio.h"

#define	TRIG_PORT      GPIOA		//TRIG       
#define	ECHO_PORT      GPIOA		//ECHO 
#define	TRIG_PIN       GPIO_Pin_5   //TRIG     
#define	ECHO_PIN       GPIO_Pin_4	//ECHO 

unsigned short int Distance3;      //������ľ���

/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void Ultrasonic3_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  	       
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					 //PC8��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				     //PC9��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //��Ϊ����
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);						 //��ʼ��GPIO
}

/*
 * ��������UltrasonicWave_CalculateTime
 * ����  ���������
 * ����  ����
 * ���  ����	
 */
void Ultrasonic3_Calculate(void)
{
   Distance3=TIM_GetCounter(TIM2)*5*34/2000;
}

/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void Ultrasonic3_Measure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG_PIN); 		  //��>10US�ĸߵ�ƽ
  DelayTime_us(20);		                      //��ʱ20US
  GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
  
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	             //�ȴ��ߵ�ƽ
  TIM_Cmd(TIM2, ENABLE);                                             //����ʱ��
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //�ȴ��͵�ƽ
  TIM_Cmd(TIM2, DISABLE);			                                 //��ʱ��2ʧ��
  Ultrasonic3_Calculate();									 //�������
  TIM_SetCounter(TIM2,0);

	if(Distance3 < 400)
	{
		printf("%d%d_",Distance3/256,Distance3%256);	
	}		
	else
	{
		printf("0000_");
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
