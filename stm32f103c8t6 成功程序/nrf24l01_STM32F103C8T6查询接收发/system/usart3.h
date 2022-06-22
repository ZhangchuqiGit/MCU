#ifndef _USART3_H_
#define _USART3_H_
#include "sys.h" 

#define     USART3_MAX_TX_LEN		    300   //����ͻ����ֽ�������ȥ\0 ���-1
#define     USART3_MAX_RX_LEN		    200   //�����ջ����ֽ�������ȥ\0 ���-1
extern volatile u16 USART3_RX_STA;            //����״̬���
extern u8   USART3_TX_BUF[USART3_MAX_TX_LEN]; //���ͻ�����
extern u8   USART3_RX_BUF[USART3_MAX_RX_LEN]; //���ջ�����    		             
 
void USART3_Init(u32 bound);
void USART3_printf (char *fmt, ...);
void USART3_Send_Array (u8 *array,u16 num);
void USART3_RX_STA_SET(void);
#endif

