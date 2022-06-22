#ifndef __DS1302_H_
#define __DS1302_H_

//---包含头文件---//

#include<reg51_typedef.h>

//---定义ds1302使用的IO口---//

sbit DSIO=P3^4;
sbit RST=P3^5;
sbit SCLK=P3^6;

//---定义全局函数---//

/*  向DS1302命令（地址+数据）  */
void Ds1302Write(uchar addr, uchar dat);
/*  读取一个地址的数据  */
uchar Ds1302Read(uchar addr);
/*  初始化DS1302.  */
void Ds1302Init(void);//年、月、日、星期、时、分、秒  
/*  读取时钟信息  */
void Ds1302ReadTime(void);
	
//---加入全局变量--//

extern uchar TIME[7];	//加入全局变量

#endif