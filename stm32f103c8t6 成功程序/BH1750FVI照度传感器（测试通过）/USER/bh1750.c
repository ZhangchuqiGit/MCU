/***************STM32F103C8T6**********************
 
 * �ļ���  ��bh1750fvi.c
 * ����    ���նȴ�����
 * ��ע    ��i2cͨ��
 * �ӿ�    ��i2c

********************LIGEN*************************/

#include "bh1750.h"
#include "delay.h"
#include "i2c.h"

u8 BUF[2];
u16 result;

//д
void BH1750_Write(unsigned char REG_Address)
{
	I2C_Start();//��ʼ�ź�
	I2C_Send_Byte(SlaveAddress);//�����豸��ַ+д�ź�
	while(I2C_Wait_Ack());
	I2C_Send_Byte(REG_Address);//�ڲ��Ĵ�����ַ
	while(I2C_Wait_Ack());
	I2C_Stop();//ֹͣ�ź�
}

//��
u16 BH1750_Read(void)
{
	I2C_Start();
	I2C_Send_Byte(SlaveAddress+1);//�����豸��ַ+���ź�
	while(I2C_Wait_Ack());
	BUF[0] = I2C_Read_Byte(1);//����ACK
	BUF[1] = I2C_Read_Byte(0);//������ACK
	I2C_Stop();
	Delay_ms(5);
	result = ((BUF[0]<<8)+BUF[1])/1.2;//�����ն�ֵ
	return result;
}

//��ʼ��
void BH1750_Init(void)
{
	BH1750_Write(PWN_ON);//�ϵ�
	BH1750_Write(Reset);//��λ
	BH1750_Write(0x42);//͸����100%
	BH1750_Write(0x65);//͸����100%,����ο�BH1750Ӧ���ֲ�
	BH1750_Write(0x10);//����H�ֱ���ģʽ
}
