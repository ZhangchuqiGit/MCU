#ifndef DHT11_H
#define DHT11_H

#include "sys.h"
#include "delay.h"

#define DHT11_IO_IN()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0xf8ffffff;}//8<<11;}
#define DHT11_IO_OUT() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0xf3ffffff;}//3<<11;} 
//IO��������   
#define	DHT11_DQ_OUT PBout(14)
#define	DHT11_DQ_IN  PBin(14) 

extern u8 humi;//6952 ʪ��ֵ 69.52%

u8 DHT11_Init(void);//��ʼ��DHT11
//u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ����
u8 DHT11_Read_Data(u8 *buf)  ;  
u8 DHT11_Read_Byte(void);//��ȡһ���ֽ�
u8 DHT11_Read_Bit(void);//��ȡһλ
u8 DHT11_Check(void);//���DHT11

void DHT11_Rst(void);//��λDHT11   

u8 DHT11_humi_Data(void);// humi:ʪ��ֵ(��Χ:20%~90%), return 6952 = 69.52%

#endif
