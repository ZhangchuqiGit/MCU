#ifndef I2C_H
#define I2C_H

#include "sys.h"

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define I2C_SCL    PBout(6) //SCL
#define I2C_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 


void I2C_Init(void);                //初始化I2C				 
void I2C_Start(void);				//发送开始信号
void I2C_Stop(void);	  			//发送停止信号
void I2C_Send_Byte(u8 txd);			//I2C发送一个字节
u8 I2C_Read_Byte(unsigned char ack);//I2C读取一个字节
u8 I2C_Wait_Ack(void); 				//I2C等待ACK信号
void I2C_Ack(void);					//I2C发送ACK信号
void I2C_NAck(void);				//I2C不发送ACK信号

void I2C_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 I2C_Read_One_Byte(u8 daddr,u8 addr);	 

#endif
