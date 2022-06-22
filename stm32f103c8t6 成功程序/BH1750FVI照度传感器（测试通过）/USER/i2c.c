/***************STM32F103C8T6**********************
 
 * �ļ���  ��i2c.c
 * ����    ��i2cͨѶ�ӿ�
 * ��ע    ����������i2c����ʽ
 * �ӿ�    ��SCL-PB6��SDA-PB7

********************LIGEN*************************/

#include "i2c.h"
#include "delay.h"

//i2c��ʼ��
void I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;//�˿ڶ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);//PB6,PB7����ߵ�ƽ
}

//����I2C��ʼ�ź�
void I2C_Start(void)
{
	SDA_OUT();//SDA���ģʽ
	I2C_SDA = 1;//����������
	I2C_SCL = 1;//����ʱ����
	Delay_us(4);
	I2C_SDA = 0;//�����½���
	Delay_us(4);
	I2C_SCL = 0;//����ʱ����
}

//����I2Cֹͣ�ź�
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

//�ȴ�Ӧ���ź�
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

//����Ӧ��
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

//������Ӧ��
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

//I2C����һ���ֽ�
void I2C_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	I2C_SCL = 0;//����ʱ�ӿ�ʼ���ݴ���
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

//I2C��ȡһ���ֽ�
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

