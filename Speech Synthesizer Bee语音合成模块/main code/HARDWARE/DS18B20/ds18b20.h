#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   

//IO方向设置
#define DS18B20_IO_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define DS18B20_IO_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}
////IO操作函数											   
#define	DS18B20_DQ_OUT PBout(9) //数据端口	PB9 
#define	DS18B20_DQ_IN  PBin(9)  //数据端口	PB9 
   	
u8 DS18B20_Init(void);//初始化DS18B20
short DS18B20_Get_Temp(void);//获取温度
void DS18B20_Start(void);//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);//读出一个字节
u8 DS18B20_Read_Bit(void);//读出一个位
u8 DS18B20_Check(void);//检测是否存在DS18B20
void DS18B20_Rst(void);//复位DS18B20    

#endif















