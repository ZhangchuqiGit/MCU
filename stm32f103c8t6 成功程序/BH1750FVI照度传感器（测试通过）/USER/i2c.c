/***************STM32F103C8T6**********************
 
 * 文件名  ：i2c.c
 * 描述    ：i2c通讯接口
 * 备注    ：采用虚拟i2c的形式
 * 接口    ：SCL-PB6，SDA-PB7

********************LIGEN*************************/

#include "i2c.h"
#include "delay.h"

//i2c初始化
void I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//端口定义
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);//PB6,PB7输出高电平
}

//产生I2C起始信号
void I2C_Start(void)
{
	SDA_OUT();//SDA输出模式
	I2C_SDA = 1;//拉高数据线
	I2C_SCL = 1;//拉高时钟线
	Delay_us(4);
	I2C_SDA = 0;//产生下降沿
	Delay_us(4);
	I2C_SCL = 0;//拉低时钟线
}

//产生I2C停止信号
void I2C_Stop(void)
{
	SDA_OUT();
	I2C_SCL = 0;
	I2C_SDA = 0;
	Delay_us(4);
	I2C_SCL = 1;
	I2C_SDA = 1;
	Delay_us(4);
}

//等待应答信号
u8 I2C_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();
	I2C_SDA = 1;Delay_us(1);
	I2C_SCL = 1;Delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL = 0;
	return 0;
}

//产生应答
void I2C_Ack(void)
{
	I2C_SCL = 0;
	SDA_OUT();
	I2C_SDA = 0;
	Delay_us(2);
	I2C_SCL = 1;
	Delay_us(2);
	I2C_SCL = 0;
}

//不产生应答
void I2C_NAck()
{
	I2C_SCL = 0;
	SDA_OUT();
	I2C_SDA = 1;
	Delay_us(2);
	I2C_SCL = 1;
	Delay_us(2);
	I2C_SCL = 0;
}

//I2C发送一个字节
void I2C_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	I2C_SCL = 0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{
		if((txd&0x80)>>7)
			I2C_SDA = 1;
		else
			I2C_SDA = 0;
		txd<<=1;
		Delay_us(2);
		I2C_SCL = 1;
		Delay_us(2);
		I2C_SCL = 0;
		Delay_us(2);
	}
}

//I2C读取一个字节
u8 I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive = 0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		I2C_SCL = 0;
		Delay_us(2);
		I2C_SCL = 1;
		receive<<=1;
		if(READ_SDA)
			receive++;
		Delay_us(1);
	}
	if(!ack)
		I2C_NAck();
	else
		I2C_Ack();
	return receive;
}

