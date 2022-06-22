#ifndef I2C_H
#define I2C_H

#include "sys.h"

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define I2C_SCL    PBout(6) //SCL
#define I2C_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 


void I2C_Init(void);                //��ʼ��I2C				 
void I2C_Start(void);				//���Ϳ�ʼ�ź�
void I2C_Stop(void);	  			//����ֹͣ�ź�
void I2C_Send_Byte(u8 txd);			//I2C����һ���ֽ�
u8 I2C_Read_Byte(unsigned char ack);//I2C��ȡһ���ֽ�
u8 I2C_Wait_Ack(void); 				//I2C�ȴ�ACK�ź�
void I2C_Ack(void);					//I2C����ACK�ź�
void I2C_NAck(void);				//I2C������ACK�ź�

void I2C_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 I2C_Read_One_Byte(u8 daddr,u8 addr);	 

#endif
