#ifndef __LCD_H_
#define __LCD_H_
/**********************************
当使用的是4位数据传输的时候定义，
使用8位取消这个定义
**********************************/
//#define LCD1602_4PINS

//---头文件---//

#include<reg51_typedef.h>

//---定义的IO口---//
#define LCD1602_DATAPINS P0

sbit LCD1602_E=P2^7;
sbit LCD1602_RW=P2^5;
sbit LCD1602_RS=P2^6;

//---函数---//

/*在51单片机12MHZ时钟下的延时函数*/
void Lcd1602_Delay1ms(uint c);   //误差 0us
/*LCD1602写入8位命令子函数*/
void LcdWriteCom(uchar com);
/*LCD1602写入8位数据子函数*/	
void LcdWriteData(uchar dat);
/*LCD1602初始化子程序*/		
void LcdInit();						  
void Lcd_ASCII(unsigned char X,unsigned char Y,char ASCII);
void Lcd_Str(unsigned char X,unsigned char Y,char *Str);

/*************** 单字符 写入 ********************/
void Lcd_ASCII(unsigned char X,unsigned char Y,char ASCII);
/*************** 字符串 写入 ********************/
void Lcd_Str(unsigned char X,unsigned char Y,char *Str);


#endif
