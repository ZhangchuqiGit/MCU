/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  : UltrasonicWave.c
 * ����    �����������ģ�飬UltrasonicWave_Configuration��������
             ��ʼ������ģ�飬UltrasonicWave_StartMeasure��������
			 ������࣬������õ�����ͨ������1��ӡ����         

*********************************************************************************/
#include "ultrasonic7.h"
#include "usart1.h"
#include "time_test.h"
#include "stm32f10x_gpio.h"

#define	TRIG_PORT      GPIOB		//TRIG       
#define	ECHO_PORT      GPIOB		//ECHO 
#define	TRIG_PIN       GPIO_Pin_6   //TRIG     
#define	ECHO_PIN       GPIO_Pin_7	//ECHO 

unsigned short int Distance7;      //������ľ���

/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void Ultrasonic7_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  	       
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
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
void Ultrasonic7_Calculate(void)
{
   Distance7=TIM_GetCounter(TIM2)*5*34/2000;
}

/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void Ultrasonic7_Measure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG_PIN); 		  //��>10US�ĸߵ�ƽ
  DelayTime_us(20);		                      //��ʱ20US
  GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
  
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	             //�ȴ��ߵ�ƽ
  TIM_Cmd(TIM2, ENABLE);                                             //����ʱ��
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //�ȴ��͵�ƽ
  TIM_Cmd(TIM2, DISABLE);			                                 //��ʱ��2ʧ��
  Ultrasonic7_Calculate();									 //�������
  TIM_SetCounter(TIM2,0);

	if(Distance7 < 400)
	{
		printf("%d%d_",Distance7/256,Distance7%256);	
	}		
	else
	{
		printf("0000_");
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

