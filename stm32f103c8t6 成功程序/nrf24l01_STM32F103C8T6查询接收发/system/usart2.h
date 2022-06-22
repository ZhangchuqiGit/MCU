#ifndef _USART2_H_
#define _USART2_H_
#include "sys.h" 

#define     USART2_MAX_TX_LEN		    600   //最大发送缓存字节数，刨去\0 最大-1
#define     USART2_MAX_RX_LEN		    200   //最大接收缓存字节数，刨去\0 最大-1
extern volatile u16 USART2_RX_STA;            //接收状态标记
extern u8   USART2_TX_BUF[USART2_MAX_TX_LEN]; //发送缓冲区
extern u8   USART2_RX_BUF[USART2_MAX_RX_LEN]; //接收缓冲区 		             
 
void USART2_Init(u32 bound);
void USART2_printf (char *fmt, ...);
void USART2_Send_Array (u8 *array,u16 num);
void USART2_RX_STA_SET(void);
#endif

