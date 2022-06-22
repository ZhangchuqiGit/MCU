/*PS2�ֱ���������*/	 
#ifndef __PSTWO_H
#define __PSTWO_H

#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "motor.h"

#define DI   PBin(10)           //PB12  ����

//  DO->PB0    CS->PB1       CLK->PB2
#define DO_H PBout(0)=1 //CMD����λ�� 
#define DO_L PBout(0)=0 //CMD����λ��
#define CS_H PBout(1)=1 //CS����
#define CS_L PBout(1)=0 //CS����
#define CLK_H PBout(2)=1 //ʱ������
#define CLK_L PBout(2)=0 //ʱ������


//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16

#define PSB_TRIANGLE    17
#define PSB_CIRCLE      18
#define PSB_CROSS       19
#define PSB_SQUARE      20

//#define PSB_TRIANGLE    13
//#define PSB_CIRCLE      14
//#define PSB_CROSS       15
//#define PSB_SQUARE      26

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5                //��ҡ��X������
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;

extern s16 speed,swerve;	  

void PS2_Init(void); //�����˿ڳ�ʼ��
u8 PS2_RedLight(void);   //�ж��Ƿ�Ϊ���ģʽ
void PS2_ReadData(void); //���ֱ�����
void PS2_Cmd(u8 CMD);		  //���ֱ���������
u8 PS2_DataKey(void);		  ////�ֱ�����ֵ������ 
u8 PS2_AnologData(u8 button); //�õ�һ��ҡ�˵�ģ����
void PS2_ClearData(void);	  //������ݻ�����
void PS2_Vibration(u8 motor2, u8 motor1);//������motor1  0xFF���������أ�motor2  0x40~0xFF
void PS2_EnterConfing(void);	 //��������
void PS2_TurnOnAnalogMode(void); //����ģ����
void PS2_VibrationMode(void);    //������
void PS2_ExitConfing(void);	     //�������
void PS2_SetInit(void);		     //�����˿ڳ�ʼ�������á����̵�ģʽ������ѡ���Ƿ�����޸ģ�������ģʽ

void speed_select(void);//�õ�һ��ҡ�˵�ģ����  ��Χ0~256  ;  �ٶ�: speed -100/+100 ; swerve -100/+100 ;
void key_mode_select(u8 keyput);

#endif





