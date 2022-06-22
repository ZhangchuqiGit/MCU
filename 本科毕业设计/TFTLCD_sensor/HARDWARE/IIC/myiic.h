#ifndef __MYIIC_H
#define __MYIIC_H

#include "sys.h"
#include "delay.h"

#define  IIC_I2C   0     /* SHT20 ��ʪ�ȴ������� */


#if IIC_I2C

//IO��������
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define IIC_SCL    PBout(6) 		//SCL
#define IIC_SDA    PBout(7) 		//SDA	 
#define READ_SDA   PBin(7) 	 		//����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

#else   /* SHT20 ��ʪ�ȴ������� */

/* Private define ------------------------------------------------------------*/

#define EEPROM_I2C_SCL_PIN      					 	GPIO_Pin_6   
#define EEPROM_I2C_SCL_GPIO_PORT   	       	GPIOB  
#define EEPROM_I2C_SCL_GPIO_RCC      			  RCC_APB2Periph_GPIOB  
 
#define EEPROM_I2C_SDA_PIN      					 	GPIO_Pin_7   
#define EEPROM_I2C_SDA_GPIO_PORT   	       	GPIOB  
#define EEPROM_I2C_SDA_GPIO_RCC      			  RCC_APB2Periph_GPIOB  

#define SCL_H         	 GPIO_SetBits(EEPROM_I2C_SCL_GPIO_PORT , EEPROM_I2C_SCL_PIN)   /*GPIOB->BSRR = GPIO_Pin_6*/
#define SCL_L            GPIO_ResetBits(EEPROM_I2C_SCL_GPIO_PORT , EEPROM_I2C_SCL_PIN) /*GPIOB->BRR  = GPIO_Pin_6 */
   
#define SDA_H         	 GPIO_SetBits(EEPROM_I2C_SDA_GPIO_PORT , EEPROM_I2C_SDA_PIN)   /*GPIOB->BSRR = GPIO_Pin_7*/
#define SDA_L         	 GPIO_ResetBits(EEPROM_I2C_SDA_GPIO_PORT , EEPROM_I2C_SDA_PIN) /*GPIOB->BRR  = GPIO_Pin_7*/

#define SCL_read       	GPIO_ReadInputDataBit(EEPROM_I2C_SCL_GPIO_PORT , EEPROM_I2C_SCL_PIN)/* GPIOB->IDR  & GPIO_Pin_6   */
#define SDA_read       	GPIO_ReadInputDataBit(EEPROM_I2C_SDA_GPIO_PORT , EEPROM_I2C_SDA_PIN)/*GPIOB->IDR  & GPIO_Pin_7	  */

#define I2C_PageSize  8  

#define ADDR_24C08		0xA0

/* Private function prototypes -----------------------------------------------*/
FunctionalState I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
FunctionalState I2C_WaitAck(void) ;
void I2C_SendByte(u8 SendByte) ;
u8 I2C_ReceiveByte(void)  ;
void I2C_Configuration(void);

#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


#endif 













