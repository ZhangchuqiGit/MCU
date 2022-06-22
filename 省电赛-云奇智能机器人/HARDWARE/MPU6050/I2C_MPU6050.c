/******************** (C) COPYRIGHT 2012  **************************
 * 硬件连接：-----------------
 *          |                 |
 *          |  PB6-I2C1_SCL		|
 *          |  PB7-I2C1_SDA   |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
 * 作者    ： Orange 
**********************************************************************************/
#include "I2C_MPU6050.h"

Int16_xyz 	GYRO_OFFSET,ACC_OFFSET;	//零漂
static u8	MPU6050_buf[14];	            //MPU6050基本数据存放区
Int16_xyz GYRO_OFFSET,ACC_OFFSET;			  //零漂
static u8	GYRO_OFFSET_OK = 0;
static u8	ACC_OFFSET_OK  = 0;

#define  EEP_Firstpage      0x00
u8 I2c_Buf_Write[256];
u8 I2c_Buf_Read[256];

/* STM32 I2C 快速模式 */
#define I2C_Speed 		400000//IIC通讯速度400khz
/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C1_OWN_ADDRESS7      0X0A   
/* AT24C01/02每页有8个字节 */
#define I2C_PageSize           8
/* AT24C04/08A/16A每页有16个字节 */
//#define I2C_PageSize           16			

/* 函数名：I2C_GPIO_Config
 * 描述  ：I2C1 I/O配置
 * 调用  ：内部调用 */
static void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 使能与 I2C1 有关的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE ); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
	/* PB6-I2C1_SCL、PB7-I2C1_SDA*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
//	GPIO_SetBits(GPIOC,GPIO_Pin_12|GPIO_Pin_11);	//PB6,PB7 输出高	
}

/* 函数名：I2C_Mode_Config
 * 描述  ：I2C 工作模式配置
 * 调用  ：内部调用 */
static void I2C_Mode_Config(void)
{
	 /* Initialize the I2C1 according to the I2C_InitStructure members */ 
	I2C_InitTypeDef I2C_InitStructure; 
	  /* I2C 配置 */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	
	I2C_InitStructure.I2C_OwnAddress1 = MPU6050_WRITE_Address; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	/* I2C的寻址模式 */
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	/* 通信速率 */
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
	/* I2C1 初始化 */
	I2C_Init(I2C1, &I2C_InitStructure);	   
	/* 使能 I2C1 */
	I2C_Cmd  (I2C1,ENABLE); 
	/*允许应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);   
}
/* 函数名：I2C_MMA_Init
 * 描述  ：I2C 外设(MMA7455)初始化
 * 调用  ：外部调用 */
void I2C_MPU6050_Init(void)
{	   
 	I2C_GPIO_Config();// 描述  ：I2C1 I/O配置
	I2C_Mode_Config();// 描述  ：I2C 工作模式配置
}  

/* 函数名：void InitMPU6050(void)
 * 描述  ：初始化Mpu6050
 * 调用  ：外部调用 */
void InitMPU6050(void)
{
	/*IIC接口初始化*/	/* PB6-I2C1_SCL、PB7-I2C1_SDA*/
	I2C_MPU6050_Init(); //I2C 外设(MMA7455)初始化
	I2C_ByteWrite(MPU_PWR_MGMT1_REG,0x80);//电源管理寄存器1   复位MPU6050
	delay_ms(100);
	I2C_ByteWrite(MPU_PWR_MGMT1_REG,0x00);//电源管理寄存器1//解除休眠状态 唤醒MPU6050

	MPU_Set_Gyro_Fsr(3);	//陀螺仪传感器,配置寄存器  ±2000dps
//	I2C_ByteWrite(MPU_GYRO_CFG_REG,0x18);//陀螺仪配置寄存器

	MPU_Set_Accel_Fsr(0);	//加速度传感器,±2g   加速计自检、测量范围及高通滤波频率
//	I2C_ByteWrite(MPU_ACCEL_CFG_REG,0x01);//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
	
	MPU_Set_Rate(1000);	    //设置采样率500Hz
/* 采样频率 = 角速度传感器(陀螺仪)输出频率 /(1+SMPLRT_DIV)  
    (1Khz或8Khz) / (1+SMPLRT_DIV)=1KHz/(1+7)=125Hz*/
//	I2C_ByteWrite(MPU_SAMPLE_RATE_REG,0x07);//采样频率分频器 采样率125Hz
/*	角速度传感器(陀螺仪)输出频率Fs=1Khz--0x06--延迟19ms--带宽5Hz      */	
//	I2C_ByteWrite(MPU_CFG_REG,0x06);//配置寄存器
	
	I2C_ByteWrite(MPU_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
	I2C_ByteWrite(MPU_PWR_MGMT2_REG,0x00);//电源管理寄存器2//加速度与陀螺仪都工作
	MPU_Set_Rate(1000);	    //设置采样率500Hz
	
}

// Wait for EEPROM Stand by state 
void I2C_EE_WaitEepromStandbyState(void)      
{
	vu16 SR1_Tmp = 0;
	do
	{
		/* Send START condition */
		I2C_GenerateSTART(I2C1, ENABLE);
		/* Read I2C1 SR1 register */
		SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
		/* Send EEPROM address for write */
		I2C_Send7bitAddress(I2C1, MPU6050_WRITE_Address, I2C_Direction_Transmitter);
	}while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
	/* Clear AF flag */
	I2C_ClearFlag(I2C1, I2C_FLAG_AF);
	/* STOP condition */    
	I2C_GenerateSTOP(I2C1, ENABLE); 
}
/*  在EEPROM的一个写循环中可以                              (单页)写多个字节，
 但一次写入的字节数不能超过EEPROM *页* 的大小，AT24C02每 页有8个字节
	pBuffer:	缓冲区指针
	REG_Address:	写地址
	NumByteToWrite:		写的字节数   */
void I2C_EE_PageWrite(u8* pBuffer, uint8_t REG_Address, u8 NumByteToWrite)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008

	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C1,MPU6050_WRITE_Address,I2C_Direction_Transmitter);
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

	/* Send the EEPROM's internal address to write to */    
	I2C_SendData(I2C1, REG_Address);  
	/* Test on EV8 and clear it */
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/* While there is data to be written */
	while(NumByteToWrite--)  
	{
		/* Send the current byte */
		I2C_SendData(I2C1, *pBuffer); 
		/* Point to the next byte to be written */
		pBuffer++; 
		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	/* Send STOP condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}


/*  将缓冲区中的数据写到I2C EEPROM中                      (多页)写入一块数据  
	pBuffer:		存放从EEPROM写入的数据的 缓冲区指针
	REG_Address:		写入数据的寄存器的地址
	NumByteToWrite:			要从EEPROM写入的字节数    */
u8 I2C_EE_BufferWrite(u8 addr, uint8_t REG_Address , u8 NumByteToWrite,u8* pBuffer)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

	Addr = REG_Address % I2C_PageSize;
	count = I2C_PageSize - Addr;
	NumOfPage =  NumByteToWrite / I2C_PageSize;
	NumOfSingle = NumByteToWrite % I2C_PageSize;
	/* If REG_Address is I2C_PageSize aligned  */
	if(Addr == 0) 
	{
		/* If NumByteToWrite < I2C_PageSize */
		if(NumOfPage == 0) 
		{
			I2C_EE_PageWrite(pBuffer, REG_Address, NumOfSingle);
			I2C_EE_WaitEepromStandbyState();
		}
		/* If NumByteToWrite > I2C_PageSize */
		else  
		{
			while(NumOfPage--)
			{
				I2C_EE_PageWrite(pBuffer, REG_Address, I2C_PageSize); 
				I2C_EE_WaitEepromStandbyState();
				REG_Address +=  I2C_PageSize;
				pBuffer += I2C_PageSize;
			}
			if(NumOfSingle!=0)
			{
				I2C_EE_PageWrite(pBuffer, REG_Address, NumOfSingle);
				I2C_EE_WaitEepromStandbyState();
			}
		}
	}
	/* If REG_Address is not I2C_PageSize aligned  */
	else 
	{
	/* If NumByteToWrite < I2C_PageSize */
		if(NumOfPage== 0) 
		{
			I2C_EE_PageWrite(pBuffer, REG_Address, NumOfSingle);
			I2C_EE_WaitEepromStandbyState();
		}
	/* If NumByteToWrite > I2C_PageSize */
		else
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / I2C_PageSize;
			NumOfSingle = NumByteToWrite % I2C_PageSize;	
			if(count != 0)
			{  
				I2C_EE_PageWrite(pBuffer, REG_Address, count);
				I2C_EE_WaitEepromStandbyState();
				REG_Address += count;
				pBuffer += count;
			} 
			while(NumOfPage--)
			{
				I2C_EE_PageWrite(pBuffer, REG_Address, I2C_PageSize);
				I2C_EE_WaitEepromStandbyState();
				REG_Address +=  I2C_PageSize;
				pBuffer += I2C_PageSize;  
			}
			if(NumOfSingle != 0)
			{
				I2C_EE_PageWrite(pBuffer, REG_Address, NumOfSingle); 
				I2C_EE_WaitEepromStandbyState();
			}
		}
	}  
	return 0;
}


/*  从EEPROM里面                                          (多页)读取一块数据       
	pBuffer:		存放从EEPROM读取的数据的缓冲区指针
	REG_Address:		读取数据的寄存器的地址
	NumByteToWrite:			要从EEPROM读取的字节数   */
//   I2C_EE_BufferRead(acc_buf,     MPU6050_ACC_OUT,      6);  u8 acc_buf[6]//加速度
//u8 I2C_EE_BufferRead(u8 addr, uint8_t REG_Address, u8 NumByteToRead,u8* pBuffer)
void I2C_EE_BufferRead(u8* pBuffer,  u8 REG_Address,  u8 NumByteToRead)
{  
	//*((u8 *)0x4001080c) |=0x80; 
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008    

	/* Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	//*((u8 *)0x4001080c) &=~0x80;
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C1, MPU6050_WRITE_Address, I2C_Direction_Transmitter);
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Clear EV6 by setting again the PE bit */
	I2C_Cmd(I2C1, ENABLE);

	/* Send the EEPROM's internal address to write to */
	I2C_SendData(I2C1, REG_Address);  
	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/* Send STRAT condition a second time */  
	I2C_GenerateSTART(I2C1, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send EEPROM address for read */
	I2C_Send7bitAddress(I2C1, MPU6050_WRITE_Address, I2C_Direction_Receiver);
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	/* While there is data to be read */
	while(NumByteToRead)  
	{
		if(NumByteToRead == 1)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C1, DISABLE);
			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C1, ENABLE);
		}
		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
		{      
			/* Read a byte from the EEPROM */
			*pBuffer = I2C_ReceiveData(I2C1);
			/* Point to the next location where the byte read will be saved */
			pBuffer++; 
			/* Decrement the read bytes counter */
			NumByteToRead--;        
		}   
	}
	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/* 函数名：I2C_ByteWrite
 * 描述  ：    到I2C设备寄存器中                         写一个字节    
 * 输入  ：REG_Address 接收数据的IIC设备寄存器的地址 
 *         REG_data 待写入的数据
 * 调用  ：内部调用 */	
void I2C_ByteWrite(uint8_t WriteAddr,uint8_t REG_data)
{
	/* Send STRAT condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	/* Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  
	
	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C1, MPU6050_WRITE_Address, I2C_Direction_Transmitter);
	/* Test on EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Send the EEPROM's internal address to write to */
	I2C_SendData(I2C1, WriteAddr);
	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	/* Send the byte to be written */
	I2C_SendData(I2C1, REG_data);    
	/* Test on EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));  
	
	/* Send STOP condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/* 函数名：I2C_ByteRead
 * 描述  ：从IIC设备寄存器中                         读取一个字节
 * 输入  ：REG_Address 读取数据的寄存器的地址 
 * 调用  ：内部调用 */
uint8_t I2C_ByteRead(uint8_t REG_Address)
{
	uint8_t REG_data;
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1,ENABLE);//起始信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1,MPU6050_WRITE_Address,I2C_Direction_Transmitter);//发送设备地址+写信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//
	
	I2C_Cmd(I2C1,ENABLE);
	I2C_SendData(I2C1,REG_Address);//发送存储单元地址，从0开始
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTART(I2C1,ENABLE);//起始信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1,MPU6050_WRITE_Address,I2C_Direction_Receiver);//发送设备地址+读信号
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	I2C_AcknowledgeConfig(I2C1,DISABLE);
	I2C_GenerateSTOP(I2C1,ENABLE);
	while(!(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)));
	
	REG_data=I2C_ReceiveData(I2C1);//读出寄存器数据
	return REG_data;
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
void MPU_Set_Gyro_Fsr(u8 fsr)
{
	I2C_ByteWrite(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
void MPU_Set_Accel_Fsr(u8 fsr)
{
	I2C_ByteWrite(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
void MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	I2C_ByteWrite(MPU_CFG_REG,data);//设置数字低通滤波器  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
void MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	I2C_ByteWrite(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

/* 函数名：GetData
 * 描述  ：获得16位数据
 * 输入  ：REG_Address 寄存器地址
 * 输出  ：返回寄存器数据
 * 调用  ：外部调用     */
unsigned int GetData(unsigned char REG_Address)
{
	char H,L;
	H=I2C_ByteRead(REG_Address);
	L=I2C_ByteRead(REG_Address+1);
	return (H<<8)+L;   //合成数据
}

//得到温度值
/*温度传感器的值，可以通过读取 0X41（高 8 位）和 0X42（低 8 位）寄存器得到，
温度换算公式为：Temperature = 36.53 + regval/340
其中， Temperature 为计算得到的温度值，单位为℃， regval 为从 0X41 和 0X42 读到的
温度传感器值*/
short MPU_Get_Temperature(void) 
{
	float temp;
	short HL;
	HL=GetData(TEMP_OUT_H);
    temp=36.53+( (double)HL )/340;  
    return temp*10;//324==32.4℃
}


/******************* (C) COPYRIGHT 2012  *****END OF FILE************/

void MPU6050_WriteReg(uint8_t WriteAddr,uint8_t REG_data)
{
     I2C_ByteWrite(  WriteAddr,  REG_data);		    //IIC写一个字节
}

void MPU6050_ReadData(uint8_t REG_Address,u8* pBuffer, u8 NumByteToRead)
{
     I2C_EE_BufferRead( pBuffer,  REG_Address,  NumByteToRead);// (多页)读取一块数据  
}

void MPU6050ReadID(void)//陀螺仪ID读取
{
	u8 Re = 0;
    MPU6050_ReadData(MPU_DEVICE_ID_REG,&Re,1);    //读器件地址
	printf("陀螺仪ID地址 : %d\r\n",Re);
}
#if 1
void MPU6050_GetDate(void)//MPU6050数据获取
{
	u8	acc_buf[6],//加速度
		temp_buf[2],//温度
		gyro_buf[6];//角速度
	
	MPU6050_ReadData(MPU6050_ACC_OUT, acc_buf, 6);		//加速度
	MPU6050_ReadData(TEMP_OUT_H,temp_buf, 2);			//温度
	MPU6050_ReadData(MPU6050_GYRO_OUT, gyro_buf, 6);	//角速度

	MPU6050_buf[0] = acc_buf[0];
	MPU6050_buf[1] = acc_buf[1];
	MPU6050_buf[2] = acc_buf[2];
	MPU6050_buf[3] = acc_buf[3];
	MPU6050_buf[4] = acc_buf[4];
	MPU6050_buf[5] = acc_buf[5];
	
	MPU6050_buf[6] = temp_buf[0];
	MPU6050_buf[7] = temp_buf[1];
	
	MPU6050_buf[8]  = gyro_buf[0];
	MPU6050_buf[9]  = gyro_buf[1];
	MPU6050_buf[10] = gyro_buf[2];
	MPU6050_buf[11] = gyro_buf[3];
	MPU6050_buf[12] = gyro_buf[4];
	MPU6050_buf[13] = gyro_buf[5];	
}
#endif

//**************************************
#if 1
#define 	MPU6050_MAX		 32767 // 32768 = 0111 1111 1111 1111 = 0x7fff
#define		MPU6050_MIN		-32768 //-32768 = 1111 1111 1111 1111 = 0xffff
void MPU6050_DataCon(Int16_xyz *Data_acc,Int16_xyz *Data_gyr)//MPU6050数据整合
{ //vs32=int32_t
	vs32 acc_x,acc_y,acc_z,//加速度
		 gyr_x,gyr_y,gyr_z;//角速度
	
	acc_x=((((int16_t)MPU6050_buf[0]) << 8) | MPU6050_buf[1]) - ACC_OFFSET.X;
	acc_y=((((int16_t)MPU6050_buf[2]) << 8) | MPU6050_buf[3]) - ACC_OFFSET.Y;
	acc_z=((((int16_t)MPU6050_buf[4]) << 8) | MPU6050_buf[5]) - ACC_OFFSET.Z;
	
	gyr_x=((((int16_t)MPU6050_buf[8])  << 8) | MPU6050_buf[9])  - GYRO_OFFSET.X;
	gyr_y=((((int16_t)MPU6050_buf[10]) << 8) | MPU6050_buf[11]) - GYRO_OFFSET.Y;
	gyr_z=((((int16_t)MPU6050_buf[12]) << 8) | MPU6050_buf[13]) - GYRO_OFFSET.Z;
	
	acc_x>MPU6050_MAX ? MPU6050_MAX : acc_x;
	acc_x<MPU6050_MIN ? MPU6050_MIN : acc_x;
	acc_y>MPU6050_MAX ? MPU6050_MAX : acc_y;
	acc_y<MPU6050_MIN ? MPU6050_MIN : acc_y;
	acc_z>MPU6050_MAX ? MPU6050_MAX : acc_z;
	acc_z<MPU6050_MIN ? MPU6050_MIN : acc_z;

	gyr_x>MPU6050_MAX ? MPU6050_MAX : gyr_x;
	gyr_x<MPU6050_MIN ? MPU6050_MIN : gyr_x;
	gyr_y>MPU6050_MAX ? MPU6050_MAX : gyr_y;
	gyr_y<MPU6050_MIN ? MPU6050_MIN : gyr_y;
	gyr_z>MPU6050_MAX ? MPU6050_MAX : gyr_z;
	gyr_z<MPU6050_MIN ? MPU6050_MIN : gyr_z;
	
	Data_acc->X = acc_x;
	Data_acc->Y = acc_y; 
	Data_acc->Z = acc_z;
	Data_gyr->X = gyr_x;
	Data_gyr->Y = gyr_y;
	Data_gyr->Z = gyr_z;
	
	if(!GYRO_OFFSET_OK)//角速度零偏处理
	{
		static int32_t	Gyrox=0,Gyroy=0,Gyroz=0;
		static uint8_t  Count_G=0;
		
		if(Count_G == 0)
		{
			GYRO_OFFSET.X=0;
			GYRO_OFFSET.Y=0;
			GYRO_OFFSET.Z=0;
			Gyrox = 0;
			Gyroy = 0;
			Gyroz = 0;
			Count_G = 1;
		}
		Gyrox += Data_gyr->X;
		Gyroy += Data_gyr->Y;
		Gyroz += Data_gyr->Z;
		
		if(Count_G == 200)
		{
			GYRO_OFFSET.X=Gyrox / Count_G;
			GYRO_OFFSET.Y=Gyroy / Count_G;
			GYRO_OFFSET.Z=Gyroz / Count_G;
			Count_G = 0;
			GYRO_OFFSET_OK = 1;
		}
		Count_G++;
	}
	if(!ACC_OFFSET_OK)//加速度零偏处理
	{
		static int32_t	Accx=0,Accy=0,Accz=0;
		static uint8_t  Count_A=0;
				
		if(Count_A == 0)
		{
			ACC_OFFSET.X = 0;
			ACC_OFFSET.Y = 0;
			ACC_OFFSET.Z = 0;
			Accx = 0;
			Accy = 0;
			Accz = 0;
			Count_A = 1;
		}
		
		Accx += Data_acc->X;
		Accy += Data_acc->Y;
		Accz += Data_acc->Z;
		
		if(Count_A == 200)
		{
			ACC_OFFSET.X=Accx / Count_A;
			ACC_OFFSET.Y=Accy / Count_A;
			ACC_OFFSET.Z=Accz / Count_A;
			Count_A = 0;
			ACC_OFFSET_OK = 1;
		}
		Count_A++;
	}
}

#endif
