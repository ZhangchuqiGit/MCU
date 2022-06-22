#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
#include "delay.h"	

//IO��������
#define DS18B20_IO_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x8fffffff;}
#define DS18B20_IO_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x3fffffff;}
////IO��������											   
#define	DS18B20_DQ_OUT PAout(7) //���ݶ˿�	
#define	DS18B20_DQ_IN  PAin(7)  //���ݶ˿�	

extern short ds18b20_value;//2788 �¶�ֵ 27.88C

u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    

#endif















