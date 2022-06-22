#ifndef __KEYSCAN_H_
#define __KEYSCAN_H_

//---头文件---//

#include<reg51_typedef.h>

//---定义的IO口---//

sbit K1=P1^0;
sbit K2=P1^1;
sbit K3=P1^2;
sbit K4=P1^3;

//---函数---//
void delay12ms(void);   //误差 0us
u8 KEYx_Scan(u8 mode,u8 Kx);//mode==1  支持连按		  

#endif
