
//*************************
#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>// 添加printf的头文件
#include <stdarg.h>
#include "misc.h"
#include "stm32f10x_it.h"
#include "led.h"
#include "timer.h"
#include "misc.h"
#include "sys.h"
#include "key.h"
#include "exti.h"
#include "delay.h"

#define USART_REC_LEN  			30 	//"FF 00 01 01 EE " =15定义最大接收字节数 300
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

extern u8  USART_RX_BUF1[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART_RX_BUF2[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

extern u16 Uart1_Tx,Uart1_Rx,Uart2_Tx,Uart3_Rx;         		//接收状态标记	
extern u16 Uart1_Sta,Uart2_Sta; //标志位   
extern u16 Uart1_Len,Uart2_Len; //长度   
extern u8  Uart1_Buffer[USART_REC_LEN];   
extern u8  Uart2_Buffer[USART_REC_LEN];   
extern u16 tx1,tx2;
extern u16 num_1,num_2;         		//接收状态标记	

extern u8 USART1_led; //接收到USART1   数据   灯闪
extern u8 USART2_led;
    

/****************STM32F103C8T6**********************/

/*比如将串口2重映射时，就是：
AFIO->MAPR|=GPIO_Remap_USART2;
要取消重映射，就是：
AFIO->MAPR&=~GPIO_Remap_USART2;
GPIO_Remap_USART2在stm32f10x_GPIO.h中是有定义的，AFIO->MAPR寄存器各个位的定义在参考手册中有详细描述。*/



//////////////////////////////
void USART1_init(u32 bound);
void USART2_init(u32 bound);
void USART3_init(u32 bound);

 /*发送一个字节数据*/
void UARTSendByte(USART_TypeDef* USARTx,unsigned char SendData);

/*接收一个字节数据*/
unsigned char UARTGetByte(USART_TypeDef* USARTx,unsigned char* GetData);

void USART_Send_String(USART_TypeDef* USARTx,char *pucString);

 /* 描述  ：重定向c库函数printf到USART1*/ 
int fputc(int ch, FILE *f);

//中断服务程序**************
void USART1_IRQHandler(void) ; //串口1中断服务程序
void USART2_IRQHandler(void)  ;
void USART3_IRQHandler(void)  ;

void USART_text(void);

#endif /* __USART1_H */

