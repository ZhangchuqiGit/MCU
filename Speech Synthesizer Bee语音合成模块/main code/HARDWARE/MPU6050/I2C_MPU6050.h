#ifndef __I2C_MMA_H
#define	__I2C_MMA_H
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

typedef struct{
				int16_t X;
				int16_t Y;
				int16_t Z;}Int16_xyz;
typedef struct{
				float rol;// roll 横滚角
				float pit;//pitch 俯仰角
				float yaw;//  yaw 航向角
				}Float_angle;
typedef struct{
				float X;
				float Y;
				float Z;}Float_xyz;

typedef struct{
				int16_t ADW_ZY;
				int16_t ADW_PIAN;
				int16_t ADW_QH;
				int16_t AD_YM;
				int16_t AD_PZ;
				int16_t AD_ZY;
				int16_t AD_QH;
				int16_t Mode;
				int16_t Fun;}Rc_Rece;
				
extern u8 count;
extern u8 Con_FLAG;
extern Int16_xyz Accel,Gyro;	   //两次综合后的传感器数据
extern Int16_xyz	Acc_Data_Con;  //滤波后的加速度
extern Float_angle  Att_Angle;	 //ATT函数计算出的姿态角
extern Rc_Rece Receive;





/*MPU6050 驱动代码	 
  要求有 FWLib文件 "stm32f10x_i2c.h"  "stm32f10x_i2c.c"
*/

/* MPU6050 Register Address ------------------------------------------------------------*/
//#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs寄存器,可读取版本号,寄存器手册未提到
//#define MPU_PROD_ID_REG			0X0C	//prod id寄存器,在寄存器手册未提到

#if 0

//技术文档未公布的寄存器 主要用于官方DMP操作
#define MPU6050_RA_XG_OFFS_TC       0x00 //[bit7] PWR_MODE, [6:1] XG_OFFS_TC, [bit 0] OTP_BNK_VLD
#define MPU6050_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD

//bit7的定义,当设置为1,辅助I2C总线高电平是VDD。当设置为0,辅助I2C总线高电平是VLOGIC
#define MPU6050_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
#define MPU6050_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
#define MPU6050_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
#define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS 两个寄存器合在一起
#define MPU6050_RA_XA_OFFS_L_TC     0x07
#define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS 两个寄存器合在一起
#define MPU6050_RA_YA_OFFS_L_TC     0x09
#define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS 两个寄存器合在一起
#define MPU6050_RA_ZA_OFFS_L_TC     0x0B
#define MPU6050_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR 两个寄存器合在一起
#define MPU6050_RA_XG_OFFS_USRL     0x14
#define MPU6050_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR 两个寄存器合在一起
#define MPU6050_RA_YG_OFFS_USRL     0x16
#define MPU6050_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR 两个寄存器合在一起
#define MPU6050_RA_ZG_OFFS_USRL     0x18

#endif

#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器X
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器Y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器Z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器A

/*陀螺仪的采样频率*/
/*传感器的寄存器输出,FIFO输出,DMP采样、运动检测、
 *零运动检测和自由落体检测都是基于采样率。
 *通过SMPLRT_DIV把陀螺仪输出率分频即可得到采样率
 *采样率=陀螺仪输出率/ (1 + SMPLRT_DIV)
 *禁用DLPF的情况下(DLPF_CFG = 0或7) ，陀螺仪输出率= 8 khz
 *在启用DLPF(见寄存器26)时，陀螺仪输出率= 1 khz
 *加速度传感器输出率是1 khz。这意味着,采样率大于1 khz时,
 *同一个加速度传感器的样品可能会多次输入到FIFO、DMP和传感器寄存器
 MPU_SAMPLE_RATE_REG [0-7] 陀螺仪输出分频采样率*/
#define	MPU_SAMPLE_RATE_REG		0x19	//采样频率分频器//陀螺仪采样率，典型值：0x07(125Hz)

/*配置外部引脚采样和DLPF数字低通滤波器*/
#define	MPU_CFG_REG				0x1A	//配置寄存器//低通滤波频率，典型值：0x06(5Hz)

#define	MPU_GYRO_CFG_REG		0x1B	//陀螺仪配置寄存器//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	MPU_ACCEL_CFG_REG		0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#if 0

/*自由落体加速度的阈值
 *这个寄存器为自由落体的阈值检测进行配置。
 *FF_THR的单位是1LSB = 2mg。当加速度传感器测量而得的三个轴的绝对值
 *都小于检测阈值时，就可以测得自由落体值。这种情况下，(加速度计每次检测到就+1以下,所以还要依靠加速度采样率)
 *自由落体时间计数器计数一次 (寄存器30)。当自由落体时间计数器达到
 *FF_DUR中规定的时间时，自由落体被中断(或发生自由落体中断)
 **/
#define MPU6050_RA_FF_THR		0x1D

/*
 *自由落体加速度的时间阈值
* 这个寄存器为自由落体时间阈值计数器进行配置。
* 时间计数频率为1 khz,因此FF_DUR的单位是 1 LSB = 1毫秒。
* 当加速度器测量而得的绝对值都小于检测阈值时，
* 自由落体时间计数器计数一次。当自由落体时间计数器
* 达到该寄存器的规定时间时，自由落体被中断。
* (或发生自由落体中断)
* */
#define MPU6050_RA_FF_DUR		0x1E

#endif

#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器

#if 0
/*
*运动检测时间的阈值。
*这个寄存器为运动中断的阈值检测进行配置。
*时间计数器计数频率为1 kHz ，因此MOT_THR的单位是 1LSB = 1ms。
*当加速度器测量而得的绝对值都超过该运动检测的阈值时(Register 31)，
*运动检测时间计数器计数一次。当运动检测计数器达到该寄存器规定的时间时，
*运动检测被中断。
 **/
#define MPU6050_RA_MOT_DUR		0x20

/*
*零运动检测加速度阈值。
* 这个寄存器为零运动中断检测进行配置。
* ZRMOT_THR的单位是1LSB = 2mg。
* 当加速度器测量而得的三个轴的绝对值都小于检测阈值时，
* 就可以测得零运动。这种情况下，零运动时间计数器计数一次 (寄存器34)。
* 当自零运动时间计数器达到ZRMOT_DUR (Register 34)中规定的时间时，零运动被中断。
* 与自由落体或运动检测不同的是，当零运动首次检测到以及当零运动检测不到时，零运动检测都被中断。
* 当零运动被检测到时,其状态将在MOT_DETECT_STATUS寄存器(寄存器97) 中显示出来。
* 当运动状态变为零运动状态被检测到时,状态位设置为1。当零运动状态变为运动状态被检测到时,
* 状态位设置为0。
 **/
#define MPU6050_RA_ZRMOT_THR	0x21

/*
*零运动检测的时间阈值
* 这个寄存器为零运动中断检测进行时间计数器的配置。
* 时间计数器的计数频率为16 Hz,因此ZRMOT_DUR的单位是1 LSB = 64 ms。
* 当加速度器测量而得的绝对值都小于检测器的阈值(Register 33)时，
* 运动检测时间计数器计数一次。当零运动检测计数器达到该寄存器规定的时间时，
* 零运动检测被中断。
 **/
#define MPU6050_RA_ZRMOT_DUR	0x22

#endif

#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机控制寄存器

#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC从机0器件地址寄存器
#define MPU_I2CSLV0_REG			0X26	//IIC从机0数据地址寄存器
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC从机0控制寄存器
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC从机1器件地址寄存器
#define MPU_I2CSLV1_REG			0X29	//IIC从机1数据地址寄存器
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC从机1控制寄存器
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC从机2器件地址寄存器
#define MPU_I2CSLV2_REG			0X2C	//IIC从机2数据地址寄存器
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC从机2控制寄存器
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC从机3器件地址寄存器
#define MPU_I2CSLV3_REG			0X2F	//IIC从机3数据地址寄存器
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC从机3控制寄存器
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC从机4器件地址寄存器
#define MPU_I2CSLV4_REG			0X32	//IIC从机4数据地址寄存器
#define MPU_I2CSLV4_DO_REG		0X33	//IIC从机4写数据寄存器
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC从机4控制寄存器
#define MPU_I2CSLV4_DI_REG		0X35	//IIC从机4读数据寄存器

/* IIC辅助从机系统中断状态 */
#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
/*中断引脚配置寄存器*/
#define MPU_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
/*部分中断使能*/
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
/*DMP中断使能*/
#define MPU6050_RA_DMP_INT_STATUS   0x39
/*DMP中断配置*/
#define MPU_INT_STA_REG			0X3A	//中断状态寄存器

#define MPU6050_ACC_OUT         0x3B     //MPU6050加速度数据寄存器地址
/*加速度X输出*/
#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
/*加速度Y输出*/
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
/*加速度Z输出*/
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

/*温度值输出*/
#define	TEMP_OUT_H		0x41	//温度值高八位寄存器
#define	TEMP_OUT_L		0x42	//温度值低8位寄存器

#define MPU6050_GYRO_OUT	0x43     //MPU6050陀螺仪数据寄存器地址
/*陀螺仪X输出*/
#define	GYRO_XOUT_H		0x43	//陀螺仪值,X轴高8位寄存器
#define	GYRO_XOUT_L		0x44	//陀螺仪值,X轴低8位寄存器
/*陀螺仪Y输出*/
#define	GYRO_YOUT_H		0x45	//陀螺仪值,Y轴高8位寄存器
#define	GYRO_YOUT_L		0x46	//陀螺仪值,Y轴低8位寄存器
/*陀螺仪Z输出*/
#define	GYRO_ZOUT_H		0x47	//陀螺仪值,Z轴高8位寄存器
#define	GYRO_ZOUT_L		0x48	//陀螺仪值,Z轴低8位寄存器

/*从IIC从机上获取到的数据*/
#define MPU6050_RA_EXT_SENS_DATA_00	0x49
#define MPU6050_RA_EXT_SENS_DATA_01	0x4A
#define MPU6050_RA_EXT_SENS_DATA_02	0x4B
#define MPU6050_RA_EXT_SENS_DATA_03	0x4C
#define MPU6050_RA_EXT_SENS_DATA_04 0x4D
#define MPU6050_RA_EXT_SENS_DATA_05 0x4E
#define MPU6050_RA_EXT_SENS_DATA_06 0x4F
#define MPU6050_RA_EXT_SENS_DATA_07 0x50
#define MPU6050_RA_EXT_SENS_DATA_08 0x51
#define MPU6050_RA_EXT_SENS_DATA_09 0x52
#define MPU6050_RA_EXT_SENS_DATA_10 0x53
#define MPU6050_RA_EXT_SENS_DATA_11 0x54
#define MPU6050_RA_EXT_SENS_DATA_12 0x55
#define MPU6050_RA_EXT_SENS_DATA_13 0x56
#define MPU6050_RA_EXT_SENS_DATA_14 0x57
#define MPU6050_RA_EXT_SENS_DATA_15 0x58
#define MPU6050_RA_EXT_SENS_DATA_16 0x59
#define MPU6050_RA_EXT_SENS_DATA_17 0x5A
#define MPU6050_RA_EXT_SENS_DATA_18 0x5B
#define MPU6050_RA_EXT_SENS_DATA_19 0x5C
#define MPU6050_RA_EXT_SENS_DATA_20 0x5D
#define MPU6050_RA_EXT_SENS_DATA_21 0x5E
#define MPU6050_RA_EXT_SENS_DATA_22 0x5F
#define MPU6050_RA_EXT_SENS_DATA_23 0x60

//运动检测的状态
//bit7  x轴反向运动检测中断状态
//bit6  x轴正向运动检测中断状态
//bit5  Y轴反向运动检测中断状态
//bit4  Y轴正向运动检测中断状态
//bit3  Z轴反向运动检测中断状态
//bit2  Z轴正向运动检测中断状态
//bit1
//bit0  零运动检测中断状态
#define MPU6050_RA_MOT_DETECT_STATUS    0x61

/*写入到IIC从机中的数据,指定的slv数据输出容器*/
#define MPU_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器

#define	MPU_PWR_MGMT1_REG		0x6B	//电源管理寄存器1//电源管理，典型值：0x00(正常启用)
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 

#if 0
/*设定DMP模式下的bank*/
#define MPU6050_RA_BANK_SEL		0x6D
/*设定DMP模式下的起始地址*/
#define MPU6050_RA_MEM_START_ADDR   0x6E
/*一个字节的dmp数据缓存*/
#define MPU6050_RA_MEM_R_W		0x6F
/*DMP配置寄存器1*/
#define MPU6050_RA_DMP_CFG_1	0x70
/*DMP配置寄存器2*/
#define MPU6050_RA_DMP_CFG_2	0x71
#endif

#define MPU_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define	MPU_DEVICE_ID_REG		0x75	//器件ID寄存器//IIC地址寄存器(默认数值0x68，只读)

#define MPU_ADDR			0X68//如果AD0脚(9脚)接地,IIC地址为0X68(不包含最低位).
//#define MPU_ADDR			0X69//如果接V3.3,则IIC地址为0X69(不包含最低位).

////因为模块AD0默认接GND,所以转为读 写地址后,为0XD1和0XD0(如果接VCC,则为0XD3和0XD2)  
//	#define MPU6050_READ_Address    0XD3
//	#define	MPU6050_WRITE_Address	0xD2	//IIC写入时的地址字节数据
#define MPU6050_READ_Address    0XD1	//// AD0为高的时候设备的写地址IIC读取时的地址字节数据
#define	MPU6050_WRITE_Address	0xD0	//// AD0为低的时候设备的写地址IIC写入时的地址字节数据

void InitMPU6050(void);					//初始化MPU6050
void I2C_MPU6050_Init(void);			//初始化IIC的IO口			
static void I2C_GPIO_Config(void);		// 描述  ：I2C1 I/O配置
static void I2C_Mode_Config(void);		// 描述  ：I2C 工作模式配置
unsigned int GetData(unsigned char REG_Address);	//获得16位数据
short MPU_Get_Temperature(void);//得到温度值

/*写一个字节到I2C设备寄存器中	
	REG_Address 接收数据的IIC设备寄存器的地址
	REG_data 待写入的数据 */
void I2C_ByteWrite(uint8_t WriteAddr,uint8_t REG_data);		    //IIC写一个字节
/* 从IIC设备寄存器中读取一个字节
	REG_Address 读取数据的寄存器的地址 */
uint8_t I2C_ByteRead(uint8_t REG_Address);						//IIC读一个字节

/*  从EEPROM里面                                                读取一块数据 (多页)      
			pBuffer:	存放从EEPROM读取的数据的  缓冲区指针
		REG_Address:	读取数据的寄存器的地址
	 NumByteToWrite:	要从EEPROM读取的字节数   */
void I2C_EE_BufferRead(u8* pBuffer,  u8 REG_Address,  u8 NumByteToRead);// (多页)读取一块数据  

/*  将缓冲区中的数据写到I2C EEPROM中   						    写入一块数据(多页)  
			pBuffer:	存放从EEPROM写入的数据的 缓冲区指针
		REG_Address:	写入数据的寄存器的地址
	 NumByteToWrite:	要从EEPROM写入的字节数    */
u8 I2C_EE_BufferWrite(u8 addr, uint8_t REG_Address , u8 NumByteToWrite,u8* pBuffer);

/*  在EEPROM的一个 写 循环 中   可以							(单页)写多个字节，
 但一次写入的字节数不能超过EEPROM *页* 的大小，AT24C02每 页有8个字节
			pBuffer:	缓冲区指针
		REG_Address:	写地址
	 NumByteToWrite:	写的字节数   */
void I2C_EE_PageWrite(u8* pBuffer, uint8_t REG_Address, u8 NumByteToWrite);

// Wait for EEPROM Stand by state 
void I2C_EE_WaitEepromStandbyState(void);


//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
void MPU_Set_Gyro_Fsr(u8 fsr);

//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
void MPU_Set_Accel_Fsr(u8 fsr);

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
void MPU_Set_LPF(u16 lpf);

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
void MPU_Set_Rate(u16 rate);



//extern float ACCEL_X,ACCEL_Y,ACCEL_Z;         	//加速度
//extern float Angel_accX,Angel_accY,Angel_accZ;	//加速度换算成的角度
//extern float GYRO_X,GYRO_Y,GYRO_Z;            	//角速度
//extern float Angle_Pitch,Gyro_Pitch;			      //俯仰角最终角度及角速度
//extern float Angle_Roll,Gyro_Roll;			        //翻滚角角最终角度及角速度
//extern float Angle_Yaw,Gyro_Yaw;			          //偏航角角最终角度及角速度

//void I2C_ByteWrite(uint8_t REG_Address,uint8_t REG_data);		//IIC写一个字节
void MPU6050_WriteReg(uint8_t REG_Address,uint8_t REG_data);

void MPU6050_ReadData(uint8_t REG_Address,unsigned char* Read,u8 num);

void MPU6050ReadID(void);//陀螺仪ID读取
void MPU6050_GetDate(void);//MPU6050数据获取
void MPU6050_DataCon(Int16_xyz *Data_acc,Int16_xyz *Data_gyr);//MPU6050数据整合


#endif /* __I2C_MMA_H */


