#ifndef __SYN6288_H
#define	__SYN6288_H

#include "sys.h" //#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

extern char text1[];
extern char text2[];
extern char text3[];
extern char text4[];
extern char text5[];
extern char text6[];
extern char text7[];
extern char text8[];
extern char text9[];
extern char text10[];
extern char text11[];
extern char text12[];

#define  TTS_Busy  PAin(8)

void TTS_Init(void);
//void Speech(char *buf, u8 len);
/*	music_sel : ��ѡ�������ֵı��, 0Ϊ�ޱ�������
	w_s : �����ı�Ϊ�����ʽ*/
void Speech(char *buf,u8 music_sel );// ������������
//void Speech(char *buf,u8 music_sel,u8 w_s);// ������������
void Speech_sel(u8 num);  //����
void Speech_bound( u16 bound );//����ͨѶ����������(��ʼ������Ϊ9600bps)


#endif 


