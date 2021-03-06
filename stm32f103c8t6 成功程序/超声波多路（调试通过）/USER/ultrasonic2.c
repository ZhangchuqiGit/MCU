/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  : UltrasonicWave.c
 * 描述    ：超声波测距模块，UltrasonicWave_Configuration（）函数
             初始化超声模块，UltrasonicWave_StartMeasure（）函数
			 启动测距，并将测得的数据通过串口1打印出来         
 * 实验平台：野火STM32开发板
 * 硬件连接：------------------
 *          | PC8  - TRIG      |
 *          | PC9  - ECHO      |
 *           ------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
*********************************************************************************/
#include "ultrasonic2.h"
#include "usart1.h"
#include "time_test.h"
#include "stm32f10x_gpio.h"

#define	TRIG_PORT      GPIOA		//TRIG       
#define	ECHO_PORT      GPIOA		//ECHO 
#define	TRIG_PIN       GPIO_Pin_3   //TRIG     
#define	ECHO_PIN       GPIO_Pin_2	//ECHO 

unsigned short int Distance2;      //计算出的距离

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void Ultrasonic2_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
  	       
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					 //PC8接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				     //PC9接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		 //设为输入
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);						 //初始化GPIO
}

/*
 * 函数名：UltrasonicWave_CalculateTime
 * 描述  ：计算距离
 * 输入  ：无
 * 输出  ：无	
 */
void Ultrasonic2_Calculate(void)
{
   Distance2=TIM_GetCounter(TIM2)*5*34/2000;
}

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void Ultrasonic2_Measure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG_PIN); 		  //送>10US的高电平
  DelayTime_us(20);		                      //延时20US
  GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
  
  while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	             //等待高电平
  TIM_Cmd(TIM2, ENABLE);                                             //开启时钟
  while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //等待低电平
  TIM_Cmd(TIM2, DISABLE);			                                 //定时器2失能
  Ultrasonic2_Calculate();									 //计算距离
  TIM_SetCounter(TIM2,0);

	if(Distance2 < 400)
	{
		printf("%d%d_",Distance2/256,Distance2%256);	
	}	
	else
	{
		printf("0000_");
	}	
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

