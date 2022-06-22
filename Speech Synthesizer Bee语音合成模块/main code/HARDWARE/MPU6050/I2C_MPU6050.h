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
				float rol;// roll �����
				float pit;//pitch ������
				float yaw;//  yaw �����
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
extern Int16_xyz Accel,Gyro;	   //�����ۺϺ�Ĵ���������
extern Int16_xyz	Acc_Data_Con;  //�˲���ļ��ٶ�
extern Float_angle  Att_Angle;	 //ATT�������������̬��
extern Rc_Rece Receive;





/*MPU6050 ��������	 
  Ҫ���� FWLib�ļ� "stm32f10x_i2c.h"  "stm32f10x_i2c.c"
*/

/* MPU6050 Register Address ------------------------------------------------------------*/
//#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs�Ĵ���,�ɶ�ȡ�汾��,�Ĵ����ֲ�δ�ᵽ
//#define MPU_PROD_ID_REG			0X0C	//prod id�Ĵ���,�ڼĴ����ֲ�δ�ᵽ

#if 0

//�����ĵ�δ�����ļĴ��� ��Ҫ���ڹٷ�DMP����
#define MPU6050_RA_XG_OFFS_TC       0x00 //[bit7] PWR_MODE, [6:1] XG_OFFS_TC, [bit 0] OTP_BNK_VLD
#define MPU6050_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD

//bit7�Ķ���,������Ϊ1,����I2C���߸ߵ�ƽ��VDD��������Ϊ0,����I2C���߸ߵ�ƽ��VLOGIC
#define MPU6050_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
#define MPU6050_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
#define MPU6050_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
#define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS �����Ĵ�������һ��
#define MPU6050_RA_XA_OFFS_L_TC     0x07
#define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS �����Ĵ�������һ��
#define MPU6050_RA_YA_OFFS_L_TC     0x09
#define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS �����Ĵ�������һ��
#define MPU6050_RA_ZA_OFFS_L_TC     0x0B
#define MPU6050_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR �����Ĵ�������һ��
#define MPU6050_RA_XG_OFFS_USRL     0x14
#define MPU6050_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR �����Ĵ�������һ��
#define MPU6050_RA_YG_OFFS_USRL     0x16
#define MPU6050_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR �����Ĵ�������һ��
#define MPU6050_RA_ZG_OFFS_USRL     0x18

#endif

#define MPU_SELF_TESTX_REG		0X0D	//�Լ�Ĵ���X
#define MPU_SELF_TESTY_REG		0X0E	//�Լ�Ĵ���Y
#define MPU_SELF_TESTZ_REG		0X0F	//�Լ�Ĵ���Z
#define MPU_SELF_TESTA_REG		0X10	//�Լ�Ĵ���A

/*�����ǵĲ���Ƶ��*/
/*�������ļĴ������,FIFO���,DMP�������˶���⡢
 *���˶��������������ⶼ�ǻ��ڲ����ʡ�
 *ͨ��SMPLRT_DIV������������ʷ�Ƶ���ɵõ�������
 *������=�����������/ (1 + SMPLRT_DIV)
 *����DLPF�������(DLPF_CFG = 0��7) �������������= 8 khz
 *������DLPF(���Ĵ���26)ʱ�������������= 1 khz
 *���ٶȴ������������1 khz������ζ��,�����ʴ���1 khzʱ,
 *ͬһ�����ٶȴ���������Ʒ���ܻ������뵽FIFO��DMP�ʹ������Ĵ���
 MPU_SAMPLE_RATE_REG [0-7] �����������Ƶ������*/
#define	MPU_SAMPLE_RATE_REG		0x19	//����Ƶ�ʷ�Ƶ��//�����ǲ����ʣ�����ֵ��0x07(125Hz)

/*�����ⲿ���Ų�����DLPF���ֵ�ͨ�˲���*/
#define	MPU_CFG_REG				0x1A	//���üĴ���//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)

#define	MPU_GYRO_CFG_REG		0x1B	//���������üĴ���//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	MPU_ACCEL_CFG_REG		0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

#if 0

/*����������ٶȵ���ֵ
 *����Ĵ���Ϊ�����������ֵ���������á�
 *FF_THR�ĵ�λ��1LSB = 2mg�������ٶȴ������������õ�������ľ���ֵ
 *��С�ڼ����ֵʱ���Ϳ��Բ����������ֵ����������£�(���ٶȼ�ÿ�μ�⵽��+1����,���Ի�Ҫ�������ٶȲ�����)
 *��������ʱ�����������һ�� (�Ĵ���30)������������ʱ��������ﵽ
 *FF_DUR�й涨��ʱ��ʱ���������屻�ж�(�������������ж�)
 **/
#define MPU6050_RA_FF_THR		0x1D

/*
 *����������ٶȵ�ʱ����ֵ
* ����Ĵ���Ϊ��������ʱ����ֵ�������������á�
* ʱ�����Ƶ��Ϊ1 khz,���FF_DUR�ĵ�λ�� 1 LSB = 1���롣
* �����ٶ����������õľ���ֵ��С�ڼ����ֵʱ��
* ��������ʱ�����������һ�Ρ�����������ʱ�������
* �ﵽ�üĴ����Ĺ涨ʱ��ʱ���������屻�жϡ�
* (�������������ж�)
* */
#define MPU6050_RA_FF_DUR		0x1E

#endif

#define MPU_MOTION_DET_REG		0X1F	//�˶���ֵⷧ���üĴ���

#if 0
/*
*�˶����ʱ�����ֵ��
*����Ĵ���Ϊ�˶��жϵ���ֵ���������á�
*ʱ�����������Ƶ��Ϊ1 kHz �����MOT_THR�ĵ�λ�� 1LSB = 1ms��
*�����ٶ����������õľ���ֵ���������˶�������ֵʱ(Register 31)��
*�˶����ʱ�����������һ�Ρ����˶����������ﵽ�üĴ����涨��ʱ��ʱ��
*�˶���ⱻ�жϡ�
 **/
#define MPU6050_RA_MOT_DUR		0x20

/*
*���˶������ٶ���ֵ��
* ����Ĵ���Ϊ���˶��жϼ��������á�
* ZRMOT_THR�ĵ�λ��1LSB = 2mg��
* �����ٶ����������õ�������ľ���ֵ��С�ڼ����ֵʱ��
* �Ϳ��Բ�����˶�����������£����˶�ʱ�����������һ�� (�Ĵ���34)��
* �������˶�ʱ��������ﵽZRMOT_DUR (Register 34)�й涨��ʱ��ʱ�����˶����жϡ�
* ������������˶���ⲻͬ���ǣ������˶��״μ�⵽�Լ������˶���ⲻ��ʱ�����˶���ⶼ���жϡ�
* �����˶�����⵽ʱ,��״̬����MOT_DETECT_STATUS�Ĵ���(�Ĵ���97) ����ʾ������
* ���˶�״̬��Ϊ���˶�״̬����⵽ʱ,״̬λ����Ϊ1�������˶�״̬��Ϊ�˶�״̬����⵽ʱ,
* ״̬λ����Ϊ0��
 **/
#define MPU6050_RA_ZRMOT_THR	0x21

/*
*���˶�����ʱ����ֵ
* ����Ĵ���Ϊ���˶��жϼ�����ʱ������������á�
* ʱ��������ļ���Ƶ��Ϊ16 Hz,���ZRMOT_DUR�ĵ�λ��1 LSB = 64 ms��
* �����ٶ����������õľ���ֵ��С�ڼ��������ֵ(Register 33)ʱ��
* �˶����ʱ�����������һ�Ρ������˶����������ﵽ�üĴ����涨��ʱ��ʱ��
* ���˶���ⱻ�жϡ�
 **/
#define MPU6050_RA_ZRMOT_DUR	0x22

#endif

#define MPU_FIFO_EN_REG			0X23	//FIFOʹ�ܼĴ���
#define MPU_I2CMST_CTRL_REG		0X24	//IIC�������ƼĴ���

#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC�ӻ�0������ַ�Ĵ���
#define MPU_I2CSLV0_REG			0X26	//IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC�ӻ�0���ƼĴ���
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC�ӻ�1������ַ�Ĵ���
#define MPU_I2CSLV1_REG			0X29	//IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC�ӻ�1���ƼĴ���
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC�ӻ�2������ַ�Ĵ���
#define MPU_I2CSLV2_REG			0X2C	//IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC�ӻ�2���ƼĴ���
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC�ӻ�3������ַ�Ĵ���
#define MPU_I2CSLV3_REG			0X2F	//IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC�ӻ�3���ƼĴ���
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC�ӻ�4������ַ�Ĵ���
#define MPU_I2CSLV4_REG			0X32	//IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU_I2CSLV4_DO_REG		0X33	//IIC�ӻ�4д���ݼĴ���
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC�ӻ�4���ƼĴ���
#define MPU_I2CSLV4_DI_REG		0X35	//IIC�ӻ�4�����ݼĴ���

/* IIC�����ӻ�ϵͳ�ж�״̬ */
#define MPU_I2CMST_STA_REG		0X36	//IIC����״̬�Ĵ���
/*�ж��������üĴ���*/
#define MPU_INTBP_CFG_REG		0X37	//�ж�/��·���üĴ���
/*�����ж�ʹ��*/
#define MPU_INT_EN_REG			0X38	//�ж�ʹ�ܼĴ���
/*DMP�ж�ʹ��*/
#define MPU6050_RA_DMP_INT_STATUS   0x39
/*DMP�ж�����*/
#define MPU_INT_STA_REG			0X3A	//�ж�״̬�Ĵ���

#define MPU6050_ACC_OUT         0x3B     //MPU6050���ٶ����ݼĴ�����ַ
/*���ٶ�X���*/
#define MPU_ACCEL_XOUTH_REG		0X3B	//���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG		0X3C	//���ٶ�ֵ,X���8λ�Ĵ���
/*���ٶ�Y���*/
#define MPU_ACCEL_YOUTH_REG		0X3D	//���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG		0X3E	//���ٶ�ֵ,Y���8λ�Ĵ���
/*���ٶ�Z���*/
#define MPU_ACCEL_ZOUTH_REG		0X3F	//���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG		0X40	//���ٶ�ֵ,Z���8λ�Ĵ���

/*�¶�ֵ���*/
#define	TEMP_OUT_H		0x41	//�¶�ֵ�߰�λ�Ĵ���
#define	TEMP_OUT_L		0x42	//�¶�ֵ��8λ�Ĵ���

#define MPU6050_GYRO_OUT	0x43     //MPU6050���������ݼĴ�����ַ
/*������X���*/
#define	GYRO_XOUT_H		0x43	//������ֵ,X���8λ�Ĵ���
#define	GYRO_XOUT_L		0x44	//������ֵ,X���8λ�Ĵ���
/*������Y���*/
#define	GYRO_YOUT_H		0x45	//������ֵ,Y���8λ�Ĵ���
#define	GYRO_YOUT_L		0x46	//������ֵ,Y���8λ�Ĵ���
/*������Z���*/
#define	GYRO_ZOUT_H		0x47	//������ֵ,Z���8λ�Ĵ���
#define	GYRO_ZOUT_L		0x48	//������ֵ,Z���8λ�Ĵ���

/*��IIC�ӻ��ϻ�ȡ��������*/
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

//�˶�����״̬
//bit7  x�ᷴ���˶�����ж�״̬
//bit6  x�������˶�����ж�״̬
//bit5  Y�ᷴ���˶�����ж�״̬
//bit4  Y�������˶�����ж�״̬
//bit3  Z�ᷴ���˶�����ж�״̬
//bit2  Z�������˶�����ж�״̬
//bit1
//bit0  ���˶�����ж�״̬
#define MPU6050_RA_MOT_DETECT_STATUS    0x61

/*д�뵽IIC�ӻ��е�����,ָ����slv�����������*/
#define MPU_I2CSLV0_DO_REG		0X63	//IIC�ӻ�0���ݼĴ���
#define MPU_I2CSLV1_DO_REG		0X64	//IIC�ӻ�1���ݼĴ���
#define MPU_I2CSLV2_DO_REG		0X65	//IIC�ӻ�2���ݼĴ���
#define MPU_I2CSLV3_DO_REG		0X66	//IIC�ӻ�3���ݼĴ���

#define MPU_I2CMST_DELAY_REG	0X67	//IIC������ʱ����Ĵ���
#define MPU_SIGPATH_RST_REG		0X68	//�ź�ͨ����λ�Ĵ���
#define MPU_MDETECT_CTRL_REG	0X69	//�˶������ƼĴ���
#define MPU_USER_CTRL_REG		0X6A	//�û����ƼĴ���

#define	MPU_PWR_MGMT1_REG		0x6B	//��Դ����Ĵ���1//��Դ��������ֵ��0x00(��������)
#define MPU_PWR_MGMT2_REG		0X6C	//��Դ����Ĵ���2 

#if 0
/*�趨DMPģʽ�µ�bank*/
#define MPU6050_RA_BANK_SEL		0x6D
/*�趨DMPģʽ�µ���ʼ��ַ*/
#define MPU6050_RA_MEM_START_ADDR   0x6E
/*һ���ֽڵ�dmp���ݻ���*/
#define MPU6050_RA_MEM_R_W		0x6F
/*DMP���üĴ���1*/
#define MPU6050_RA_DMP_CFG_1	0x70
/*DMP���üĴ���2*/
#define MPU6050_RA_DMP_CFG_2	0x71
#endif

#define MPU_FIFO_CNTH_REG		0X72	//FIFO�����Ĵ����߰�λ
#define MPU_FIFO_CNTL_REG		0X73	//FIFO�����Ĵ����Ͱ�λ
#define MPU_FIFO_RW_REG			0X74	//FIFO��д�Ĵ���
#define	MPU_DEVICE_ID_REG		0x75	//����ID�Ĵ���//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

#define MPU_ADDR			0X68//���AD0��(9��)�ӵ�,IIC��ַΪ0X68(���������λ).
//#define MPU_ADDR			0X69//�����V3.3,��IIC��ַΪ0X69(���������λ).

////��Ϊģ��AD0Ĭ�Ͻ�GND,����תΪ�� д��ַ��,Ϊ0XD1��0XD0(�����VCC,��Ϊ0XD3��0XD2)  
//	#define MPU6050_READ_Address    0XD3
//	#define	MPU6050_WRITE_Address	0xD2	//IICд��ʱ�ĵ�ַ�ֽ�����
#define MPU6050_READ_Address    0XD1	//// AD0Ϊ�ߵ�ʱ���豸��д��ַIIC��ȡʱ�ĵ�ַ�ֽ�����
#define	MPU6050_WRITE_Address	0xD0	//// AD0Ϊ�͵�ʱ���豸��д��ַIICд��ʱ�ĵ�ַ�ֽ�����

void InitMPU6050(void);					//��ʼ��MPU6050
void I2C_MPU6050_Init(void);			//��ʼ��IIC��IO��			
static void I2C_GPIO_Config(void);		// ����  ��I2C1 I/O����
static void I2C_Mode_Config(void);		// ����  ��I2C ����ģʽ����
unsigned int GetData(unsigned char REG_Address);	//���16λ����
short MPU_Get_Temperature(void);//�õ��¶�ֵ

/*дһ���ֽڵ�I2C�豸�Ĵ�����	
	REG_Address �������ݵ�IIC�豸�Ĵ����ĵ�ַ
	REG_data ��д������� */
void I2C_ByteWrite(uint8_t WriteAddr,uint8_t REG_data);		    //IICдһ���ֽ�
/* ��IIC�豸�Ĵ����ж�ȡһ���ֽ�
	REG_Address ��ȡ���ݵļĴ����ĵ�ַ */
uint8_t I2C_ByteRead(uint8_t REG_Address);						//IIC��һ���ֽ�

/*  ��EEPROM����                                                ��ȡһ������ (��ҳ)      
			pBuffer:	��Ŵ�EEPROM��ȡ�����ݵ�  ������ָ��
		REG_Address:	��ȡ���ݵļĴ����ĵ�ַ
	 NumByteToWrite:	Ҫ��EEPROM��ȡ���ֽ���   */
void I2C_EE_BufferRead(u8* pBuffer,  u8 REG_Address,  u8 NumByteToRead);// (��ҳ)��ȡһ������  

/*  ���������е�����д��I2C EEPROM��   						    д��һ������(��ҳ)  
			pBuffer:	��Ŵ�EEPROMд������ݵ� ������ָ��
		REG_Address:	д�����ݵļĴ����ĵ�ַ
	 NumByteToWrite:	Ҫ��EEPROMд����ֽ���    */
u8 I2C_EE_BufferWrite(u8 addr, uint8_t REG_Address , u8 NumByteToWrite,u8* pBuffer);

/*  ��EEPROM��һ�� д ѭ�� ��   ����							(��ҳ)д����ֽڣ�
 ��һ��д����ֽ������ܳ���EEPROM *ҳ* �Ĵ�С��AT24C02ÿ ҳ��8���ֽ�
			pBuffer:	������ָ��
		REG_Address:	д��ַ
	 NumByteToWrite:	д���ֽ���   */
void I2C_EE_PageWrite(u8* pBuffer, uint8_t REG_Address, u8 NumByteToWrite);

// Wait for EEPROM Stand by state 
void I2C_EE_WaitEepromStandbyState(void);


//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
void MPU_Set_Gyro_Fsr(u8 fsr);

//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
void MPU_Set_Accel_Fsr(u8 fsr);

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
void MPU_Set_LPF(u16 lpf);

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
void MPU_Set_Rate(u16 rate);



//extern float ACCEL_X,ACCEL_Y,ACCEL_Z;         	//���ٶ�
//extern float Angel_accX,Angel_accY,Angel_accZ;	//���ٶȻ���ɵĽǶ�
//extern float GYRO_X,GYRO_Y,GYRO_Z;            	//���ٶ�
//extern float Angle_Pitch,Gyro_Pitch;			      //���������սǶȼ����ٶ�
//extern float Angle_Roll,Gyro_Roll;			        //�����ǽ����սǶȼ����ٶ�
//extern float Angle_Yaw,Gyro_Yaw;			          //ƫ���ǽ����սǶȼ����ٶ�

//void I2C_ByteWrite(uint8_t REG_Address,uint8_t REG_data);		//IICдһ���ֽ�
void MPU6050_WriteReg(uint8_t REG_Address,uint8_t REG_data);

void MPU6050_ReadData(uint8_t REG_Address,unsigned char* Read,u8 num);

void MPU6050ReadID(void);//������ID��ȡ
void MPU6050_GetDate(void);//MPU6050���ݻ�ȡ
void MPU6050_DataCon(Int16_xyz *Data_acc,Int16_xyz *Data_gyr);//MPU6050��������


#endif /* __I2C_MMA_H */


