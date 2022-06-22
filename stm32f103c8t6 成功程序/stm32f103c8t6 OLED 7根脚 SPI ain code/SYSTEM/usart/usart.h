#ifndef __USART_H
#define __USART_H

#include "stdio.h"	// 添加printf的头文件
#include "sys.h"    //#include "stm32f10x.h"
#include "string.h"
#include <stdarg.h>

#include "misc.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"  
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"

#include "delay.h"

/**********************************                          ********************************/
/* EN_USART_code_key   :   数据采用数据包的形式接收，接收后存放于缓存区， 
通过 数据头（可变）和 数据尾（可变）来判断数据的“包”及有效性，     	 	  
中断函数用于接收数据 和 判断头尾 以及 数据包长度，                 	  	  
优点：适合打包传输，稳定性和可靠性很有保证，可随意发送，自动挑选有效数据。
缺点：缓存区数据长度要根据“包裹”长度 设定！要是多次接收后无头无尾，到有头有尾的那一段数据恰好跨越缓存区最前和最后位置时，
可能导致本次数据丢失，不过这种情况几乎没有可能。        				 */    
//如果想串口中断接收，请使能以下宏定义
#define EN_USART1_RX		1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX		0		//使能（1）/禁止（0）串口2接收
#define EN_USART3_RX		0		//使能（1）/禁止（0）串口3接收
//采用数据包的形式接收
#define EN_USART1_code_key		0		//使能（1）/禁止（0）  数据包的形式接收
#define EN_USART2_code_key		0		//使能（1）/禁止（0）  数据包的形式接收
#define EN_USART3_code_key		0		//使能（1）/禁止（0）  数据包的形式接收 	

/**********************************                          *******************************
extern u8  USARTx_RX_BUF[USARTx_REC_LEN]; 
接收缓冲,最大USARTx_REC_LEN个字节.末字节为换行符   
注意,读取USARTx->SR能避免莫名其妙的错误   	
接收状态 :
			bit15，		接收完成标志
			bit14，		接收到0x0d
			bit13~0，	接收到的有效字节数目  */

#if EN_USART1_RX   ////如果串口1使能了接收
	#define USART1_REC_LEN		100  //"FF 00 01 01 EE " =15定义最大接收字节数 30
	void uart1_init(u32 bound);/*USART1_TX PA.9 ; USART1_RX PA.10 */
	extern char  USART1_RX_BUF[USART1_REC_LEN]; /*缓存区*///接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
	#if EN_USART1_code_key // 数据包的形式接收
		extern u8 USART1_RX_head;		//接收状态标记 --  数据头
		extern u8 USART1_RX_num;		//接收状态标记 --  数据计数
		extern u8 USART1_RX_len;		//接收状态标记 --  数据包裹长度
		extern u8 USART1_led; 			//接收到USART1   数据 收到语音
	#else
		extern u16 USART1_RX_STA;         		//接收状态标记	
//		extern char *pDataByte;
	#endif
#endif
#if EN_USART2_RX   ////如果串口2使能了接收
	#define USART2_REC_LEN		100  //定义最大接收字节数 
	void uart2_init(u32 bound);
	extern char  USART2_RX_BUF[USART2_REC_LEN];/*缓存区*///接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
	#if EN_USART2_code_key // 数据包的形式接收
		extern u8 USART2_RX_head;		//接收状态标记 --  数据头
		extern u8 USART2_RX_num;		//接收状态标记 --  数据计数
		extern u8 USART2_RX_len;		//接收状态标记 --  数据包裹长度
		extern u8 USART2_led; //收到语音			
	#else
		extern u16 USART2_RX_STA;         		//接收状态标记	
	#endif
#endif
#if EN_USART3_RX   ////如果串口2使能了接收
	#define USART3_REC_LEN		100  //定义最大接收字节数 
	void uart3_init(u32 bound);
	extern char  USART3_RX_BUF[USART3_REC_LEN];/*缓存区*///接收缓冲,最大USART1_REC_LEN个字节.末字节为换行符 
	#if EN_USART3_code_key // 数据包的形式接收
		extern u8 USART3_RX_head;		//接收状态标记 --  数据头
		extern u8 USART3_RX_num;		//接收状态标记 --  数据计数
		extern u8 USART3_RX_len;		//接收状态标记 --  数据包裹长度
	#else
		extern u16 USART3_RX_STA;         		//接收状态标记	
	#endif
#endif


/*接收一个字节数据*/
unsigned char UARTGetByte(USART_TypeDef* USARTx,unsigned char* Get_Data);
/*  TXE方式 ：发送一个字节数据*/
void UARTSendByte_TXE(USART_TypeDef* USARTx,unsigned char SendData);
void USART_Send_String_TXE(USART_TypeDef* USARTx,char *pucString);

/*******++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++************/
#if 0
/*  TC方式 ：发送一个字节数据*/
void UARTSendByte_TC(USART_TypeDef* USARTx,unsigned char SendData);
void USART_Send_String_TC(USART_TypeDef* USARTx,char *pucString);
int itoa(int num, char *str, int radix);
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... );  //  该函数就可以像printf使用可变参数，方便很多
void uartWriteBuf(uint8_t *buf, uint8_t len);
#endif
/*******++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++************/

#endif


