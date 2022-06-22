/***************STM32F103C8T6**********************
 
 * 文件名  ：bh1750fvi.c
 * 描述    ：照度传感器
 * 备注    ：i2c通信
 * 接口    ：i2c

********************LIGEN*************************/

#include "bh1750.h"
#include "delay.h"
#include "i2c.h"

u8 BUF[2];
u16 result;

//写
void BH1750_Write(unsigned char REG_Address)
{
	I2C_Start();//起始信号
	I2C_Send_Byte(SlaveAddress);//发送设备地址+写信号
	while(I2C_Wait_Ack());
	I2C_Send_Byte(REG_Address);//内部寄存器地址
	while(I2C_Wait_Ack());
	I2C_Stop();//停止信号
}

//读
u16 BH1750_Read(void)
{
	I2C_Start();
	I2C_Send_Byte(SlaveAddress+1);//发送设备地址+读信号
	while(I2C_Wait_Ack());
	BUF[0] = I2C_Read_Byte(1);//发送ACK
	BUF[1] = I2C_Read_Byte(0);//不发送ACK
	I2C_Stop();
	Delay_ms(5);
	result = ((BUF[0]<<8)+BUF[1])/1.2;//计算照度值
	return result;
}

//初始化
void BH1750_Init(void)
{
	BH1750_Write(PWN_ON);//上电
	BH1750_Write(Reset);//复位
	BH1750_Write(0x42);//透光率100%
	BH1750_Write(0x65);//透光率100%,详情参考BH1750应用手册
	BH1750_Write(0x10);//连续H分辨率模式
}
