#ifndef _USART2_H_
#define _USART2_H_
#include "sys.h" 

#define     USART2_MAX_TX_LEN		    600   //����ͻ����ֽ�������ȥ\0 ���-1
#define     USART2_MAX_RX_LEN		    200   //�����ջ����ֽ�������ȥ\0 ���-1
extern volatile u16 USART2_RX_STA;            //����״̬���
extern u8   USART2_TX_BUF[USART2_MAX_TX_LEN]; //���ͻ�����
extern u8   USART2_RX_BUF[USART2_MAX_RX_LEN]; //���ջ����� 		             
 
void USART2_Init(u32 bound);
void USART2_printf (char *fmt, ...);
void USART2_Send_Array (u8 *array,u16 num);
void USART2_RX_STA_SET(void);
#endif

