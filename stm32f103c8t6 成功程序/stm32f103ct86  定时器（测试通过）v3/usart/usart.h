
//*************************
#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>// ���printf��ͷ�ļ�
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

#define USART_REC_LEN  			30 	//"FF 00 01 01 EE " =15�����������ֽ��� 300
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

extern u8  USART_RX_BUF1[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART_RX_BUF2[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

extern u16 Uart1_Tx,Uart1_Rx,Uart2_Tx,Uart3_Rx;         		//����״̬���	
extern u16 Uart1_Sta,Uart2_Sta; //��־λ   
extern u16 Uart1_Len,Uart2_Len; //����   
extern u8  Uart1_Buffer[USART_REC_LEN];   
extern u8  Uart2_Buffer[USART_REC_LEN];   
extern u16 tx1,tx2;
extern u16 num_1,num_2;         		//����״̬���	

extern u8 USART1_led; //���յ�USART1   ����   ����
extern u8 USART2_led;
    

/****************STM32F103C8T6**********************/

/*���罫����2��ӳ��ʱ�����ǣ�
AFIO->MAPR|=GPIO_Remap_USART2;
Ҫȡ����ӳ�䣬���ǣ�
AFIO->MAPR&=~GPIO_Remap_USART2;
GPIO_Remap_USART2��stm32f10x_GPIO.h�����ж���ģ�AFIO->MAPR�Ĵ�������λ�Ķ����ڲο��ֲ�������ϸ������*/



//////////////////////////////
void USART1_init(u32 bound);
void USART2_init(u32 bound);
void USART3_init(u32 bound);

 /*����һ���ֽ�����*/
void UARTSendByte(USART_TypeDef* USARTx,unsigned char SendData);

/*����һ���ֽ�����*/
unsigned char UARTGetByte(USART_TypeDef* USARTx,unsigned char* GetData);

void USART_Send_String(USART_TypeDef* USARTx,char *pucString);

 /* ����  ���ض���c�⺯��printf��USART1*/ 
int fputc(int ch, FILE *f);

//�жϷ������**************
void USART1_IRQHandler(void) ; //����1�жϷ������
void USART2_IRQHandler(void)  ;
void USART3_IRQHandler(void)  ;

void USART_text(void);

#endif /* __USART1_H */

