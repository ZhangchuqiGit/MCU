#include"timer.h"

u8 Tim_on=0,Tim_on1=0;
u8 N_TH0=0,N_TL0=0;
u8 N_TH1=0,N_TL1=0;

/***************************************   ��ʱ�� 0   **********************************************/
void Timer0_Init( u8 mode,u16 T)		
{
	u16 N = 0;
	T = (12.0 /*Fosc12MHz*/  /12) * T ;/*  MHz */
//	T = (11.0592 /*Fosc11_0592MHz*/ /12) * T ;
	TMOD &= ~(0x03);//clear bit
	switch(mode)//���ö�ʱ��ģʽ	���ö�ʱ��ֵ
	{
		case 0: TMOD |= 0x00;/* 13bit */
				N = 8191+1-T;
				N_TH0 = (u8)( N>>8)&0xff;	
				N_TL0 = (u8)N &0xff;
		break;
		case 1: TMOD |=  0x01;/* 16bit */
				N = 0xffff+1-T; 
				N_TH0 = (u8)( N>>8)&0xff;	
				N_TL0 = (u8)N &0xff;
		break;
		case 2: TMOD |=  0x02;/* 8bit Auto */
				N = 0xff+1-T;
				N_TH0 = 0;	
				N_TL0 = (u8)N &0xff;
		break;
		case 3: TMOD |=  0x03;
				N = 0xff+1-T;
				N_TH0 = 0;	
				N_TL0 = (u8)N &0xff;
		break;
	}
	TH0 = N_TH0;		
	TL0 = N_TL0;
    ET0 = 1;
	TR0 = 1;//���п���λ
	EA = 1;
}

/**  ��ʱ�� 0 �жϺ���  **/
void Timer0(void) interrupt 1
{
	TH0 = N_TH0;		
	TL0 = N_TL0;
	if(Tim_on==1)beep=!beep;
	if(Tim_on1==1)beep=!beep;
}
