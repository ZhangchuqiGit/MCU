#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED PBout(11)//LED�˿ڶ���
#define Human_body PBin(12) // �������

extern u8 adc_q,humi_q,Measure_q,ds18b20c_q,Human_en; 

void LED_Init(void); //�˿ڳ�ʼ��		 
void Human_infrared_init(void);//�������

#endif

















