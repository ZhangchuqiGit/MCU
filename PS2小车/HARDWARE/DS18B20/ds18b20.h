#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
#include "delay.h"	

//IO方向设置
#define DS18B20_IO_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x8fffffff;}
#define DS18B20_IO_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x3fffffff;}
////IO操作函数											   
#define	DS18B20_DQ_OUT PAout(7) //数据端口	
#define	DS18B20_DQ_IN  PAin(7)  //数据端口	

extern short ds18b20_value;//2788 温度值 27.88C

u8 DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20    

#endif















