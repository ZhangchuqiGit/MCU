#ifndef __KEYSCAN_H_
#define __KEYSCAN_H_

//---ͷ�ļ�---//

#include<reg51_typedef.h>

//---�����IO��---//

sbit K1=P1^0;
sbit K2=P1^1;
sbit K3=P1^2;
sbit K4=P1^3;

//---����---//
void delay12ms(void);   //��� 0us
u8 KEYx_Scan(u8 mode,u8 Kx);//mode==1  ֧������		  

#endif
