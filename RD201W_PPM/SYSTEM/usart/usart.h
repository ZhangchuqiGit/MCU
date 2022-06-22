#ifndef __USART_H
#define __USART_H

#include "stdio.h"	// ���printf��ͷ�ļ�
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
/* EN_USART_code_key   :   ���ݲ������ݰ�����ʽ���գ����պ����ڻ������� 
ͨ�� ����ͷ���ɱ䣩�� ����β���ɱ䣩���ж����ݵġ���������Ч�ԣ�     	 	  
�жϺ������ڽ������� �� �ж�ͷβ �Լ� ���ݰ����ȣ�                 	  	  
�ŵ㣺�ʺϴ�����䣬�ȶ��ԺͿɿ��Ժ��б�֤�������ⷢ�ͣ��Զ���ѡ��Ч���ݡ�
ȱ�㣺���������ݳ���Ҫ���ݡ����������� �趨��Ҫ�Ƕ�ν��պ���ͷ��β������ͷ��β����һ������ǡ�ÿ�Խ��������ǰ�����λ��ʱ��
���ܵ��±������ݶ�ʧ�����������������û�п��ܡ�        				 */    
//����봮���жϽ��գ���ʹ�����º궨��
#define EN_USART1_RX		1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX		0		//ʹ�ܣ�1��/��ֹ��0������2����
#define EN_USART3_RX		0		//ʹ�ܣ�1��/��ֹ��0������3����
//�������ݰ�����ʽ����
#define EN_USART1_code_key		0		//ʹ�ܣ�1��/��ֹ��0��  ���ݰ�����ʽ����
#define EN_USART2_code_key		0		//ʹ�ܣ�1��/��ֹ��0��  ���ݰ�����ʽ����
#define EN_USART3_code_key		0		//ʹ�ܣ�1��/��ֹ��0��  ���ݰ�����ʽ���� 	

/**********************************                          *******************************
extern u8  USARTx_RX_BUF[USARTx_REC_LEN]; 
���ջ���,���USARTx_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�   
ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
����״̬ :
			bit15��		������ɱ�־
			bit14��		���յ�0x0d
			bit13~0��	���յ�����Ч�ֽ���Ŀ  */

#if EN_USART1_RX   ////�������1ʹ���˽���
	#define USART1_REC_LEN		100  //"FF 00 01 01 EE " =15�����������ֽ��� 30
	void uart1_init(u32 bound);/*USART1_TX PA.9 ; USART1_RX PA.10 */
	extern char  USART1_RX_BUF[USART1_REC_LEN]; /*������*///���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	#if EN_USART1_code_key // ���ݰ�����ʽ����
		extern u8 USART1_RX_head;		//����״̬��� --  ����ͷ
		extern u8 USART1_RX_num;		//����״̬��� --  ���ݼ���
		extern u8 USART1_RX_len;		//����״̬��� --  ���ݰ�������
		extern u8 USART1_led; 			//���յ�USART1   ���� �յ�����
	#else
		extern u16 USART1_RX_STA;         		//����״̬���	
//		extern char *pDataByte;
	#endif
#endif
#if EN_USART2_RX   ////�������2ʹ���˽���
	#define USART2_REC_LEN		100  //�����������ֽ��� 
	void uart2_init(u32 bound);
	extern char  USART2_RX_BUF[USART2_REC_LEN];/*������*///���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	#if EN_USART2_code_key // ���ݰ�����ʽ����
		extern u8 USART2_RX_head;		//����״̬��� --  ����ͷ
		extern u8 USART2_RX_num;		//����״̬��� --  ���ݼ���
		extern u8 USART2_RX_len;		//����״̬��� --  ���ݰ�������
		extern u8 USART2_led; //�յ�����			
	#else
		extern u16 USART2_RX_STA;         		//����״̬���	
	#endif
#endif
#if EN_USART3_RX   ////�������2ʹ���˽���
	#define USART3_REC_LEN		100  //�����������ֽ��� 
	void uart3_init(u32 bound);
	extern char  USART3_RX_BUF[USART3_REC_LEN];/*������*///���ջ���,���USART1_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
	#if EN_USART3_code_key // ���ݰ�����ʽ����
		extern u8 USART3_RX_head;		//����״̬��� --  ����ͷ
		extern u8 USART3_RX_num;		//����״̬��� --  ���ݼ���
		extern u8 USART3_RX_len;		//����״̬��� --  ���ݰ�������
	#else
		extern u16 USART3_RX_STA;         		//����״̬���	
	#endif
#endif


/*����һ���ֽ�����*/
unsigned char UARTGetByte(USART_TypeDef* USARTx,unsigned char* Get_Data);
/*  TXE��ʽ ������һ���ֽ�����*/
void UARTSendByte_TXE(USART_TypeDef* USARTx,unsigned char SendData);
void USART_Send_String_TXE(USART_TypeDef* USARTx,char *pucString);

/*******++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++************/
#if 0
/*  TC��ʽ ������һ���ֽ�����*/
void UARTSendByte_TC(USART_TypeDef* USARTx,unsigned char SendData);
void USART_Send_String_TC(USART_TypeDef* USARTx,char *pucString);
int itoa(int num, char *str, int radix);
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... );  //  �ú����Ϳ�����printfʹ�ÿɱ����������ܶ�
void uartWriteBuf(uint8_t *buf, uint8_t len);
#endif
/*******++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++************/

#endif


