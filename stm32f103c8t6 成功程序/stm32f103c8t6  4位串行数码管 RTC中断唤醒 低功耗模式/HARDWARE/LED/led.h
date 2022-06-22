#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED PBout(11)//LED端口定义
#define Human_body PBin(12) // 人体红外

extern u8 adc_q,humi_q,Measure_q,ds18b20c_q,Human_en; 

void LED_Init(void); //端口初始化		 
void Human_infrared_init(void);//人体红外

#endif

















