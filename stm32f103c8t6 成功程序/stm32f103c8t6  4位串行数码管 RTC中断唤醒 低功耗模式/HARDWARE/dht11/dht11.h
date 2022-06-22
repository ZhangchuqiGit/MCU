#ifndef DHT11_H
#define DHT11_H

#include "sys.h"
#include "delay.h"

#define DHT11_IO_IN()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0xf8ffffff;}//8<<11;}
#define DHT11_IO_OUT() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0xf3ffffff;}//3<<11;} 
//IO操作函数   
#define	DHT11_DQ_OUT PBout(14)
#define	DHT11_DQ_IN  PBin(14) 

extern u8 humi;//6952 湿度值 69.52%

u8 DHT11_Init(void);//初始化DHT11
//u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取数据
u8 DHT11_Read_Data(u8 *buf)  ;  
u8 DHT11_Read_Byte(void);//读取一个字节
u8 DHT11_Read_Bit(void);//读取一位
u8 DHT11_Check(void);//检测DHT11

void DHT11_Rst(void);//复位DHT11   

u8 DHT11_humi_Data(void);// humi:湿度值(范围:20%~90%), return 6952 = 69.52%

#endif
