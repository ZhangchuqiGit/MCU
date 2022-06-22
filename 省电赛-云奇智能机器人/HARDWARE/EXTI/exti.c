/************************************
 * 文件名  ：exti.c
 * 描述    ：采用外部中断的方式进行电机测速  
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接： PB0作为外部中断输入接口 
 * 库版本  ：ST3.0.0 
**********************************************************************************/
#include "exti.h"
#include "usart.h"

void EXTI0_GPIO_Config()//打开PB时钟，设置PB0引脚为输入
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//选择Pin0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出频率最大50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);   
}

void EXTI0_Config() //3
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_ClearITPendingBit(EXTI_Line0);	//清除中断标志

    //在库函数中，配置 GPIO 与中断线的映射关系的函数 GPIO_EXTILineConfig()来实现的：
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);//将中断线 EXTI0线与PB0连接

	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//选择中断线路
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //设置为中断请求，非事件请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //低电平触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //外部中断使能
	EXTI_Init(&EXTI_InitStructure);
}

void EXTI0_NVIC_Config() //2
{
	NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    //打开GPIOB的端口时钟
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//选择中断分组           
        
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//选择中断通道0，Pin几就是通道几
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占式中断优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应中断优先级为0     
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能中断                               
	NVIC_Init(&NVIC_InitStructure);             
}

//总的中断初始化  A
void EXTI0_Init()
{
	EXTI0_GPIO_Config();//打开PB时钟，设置PB0引脚为输入
	EXTI0_NVIC_Config();
	EXTI0_Config();
}
//************************************************
//总的中断初始化  B
void EXTIX_Init(void)
{
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

  //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;//选择中断线路	//KEY2
/**/    
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//设置为中断请求，非事件请求
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//低电平触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//外部中断使能
/**/    
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

 //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;//选择中断线路
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

 //GPIOE.4	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;//选择中断线路
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  //GPIOA.0	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;//选择中断线路
/**/EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//高电平触发
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

//*****************EXTI_Line0中断线路
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//选择中断通道0//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
//*****************EXTI_Line2中断线路
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//选择中断通道2//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
//*****************EXTI_Line3中断线路
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//选择中断通道3//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
//*****************EXTI_Line4中断线路
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//选择中断通道4//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}

///**********************************************

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	Delay_ms(10);//消抖
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)	 	 //WK_UP按键
	{			
        printf(" //外部中断0  服务程序\r\n");  

	}
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}
 
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	Delay_ms(10);//消抖
	if(KEY2==0)	  //按键KEY2
	{


	}		 
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位  
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	Delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1
	{				 


	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}
//外部中断4服务程序
void EXTI4_IRQHandler(void)
{
	Delay_ms(10);//消抖
	if(KEY0==0)	 //按键KEY0
	{

	}		 
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}
 
