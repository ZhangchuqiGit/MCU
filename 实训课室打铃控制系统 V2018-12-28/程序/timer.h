#ifndef __TIMER_H_
#define __TIMER_H_

//---ͷ�ļ�---//

#include<reg51_typedef.h>
 
sbit beep=P1^5;	//�ܽŽ���

extern u8 N_TH0,N_TL0;
extern u8 N_TH1,N_TL1;
extern u8 Tim_on,Tim_on1;

//---����---//
void Timer0_Init( u8 mode,u16 T);		

#endif
