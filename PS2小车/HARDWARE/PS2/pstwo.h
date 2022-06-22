/*PS2手柄驱动程序*/	 
#ifndef __PSTWO_H
#define __PSTWO_H

#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "motor.h"

#define DI   PBin(10)           //PB12  输入

//  DO->PB0    CS->PB1       CLK->PB2
#define DO_H PBout(0)=1 //CMD命令位高 
#define DO_L PBout(0)=0 //CMD命令位低
#define CS_H PBout(1)=1 //CS拉高
#define CS_L PBout(1)=0 //CS拉低
#define CLK_H PBout(2)=1 //时钟拉高
#define CLK_L PBout(2)=0 //时钟拉低


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
#define PSS_RX 5                //右摇杆X轴数据
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;

extern s16 speed,swerve;	  

void PS2_Init(void); //驱动端口初始化
u8 PS2_RedLight(void);   //判断是否为红灯模式
void PS2_ReadData(void); //读手柄数据
void PS2_Cmd(u8 CMD);		  //向手柄发送命令
u8 PS2_DataKey(void);		  ////手柄按键值捕获处理 
u8 PS2_AnologData(u8 button); //得到一个摇杆的模拟量
void PS2_ClearData(void);	  //清除数据缓冲区
void PS2_Vibration(u8 motor2, u8 motor1);//振动设置motor1  0xFF开，其他关，motor2  0x40~0xFF
void PS2_EnterConfing(void);	 //进入配置
void PS2_TurnOnAnalogMode(void); //发送模拟量
void PS2_VibrationMode(void);    //振动设置
void PS2_ExitConfing(void);	     //完成配置
void PS2_SetInit(void);		     //驱动端口初始化，配置“红绿灯模式”，并选择是否可以修改，开启震动模式

void speed_select(void);//得到一个摇杆的模拟量  范围0~256  ;  速度: speed -100/+100 ; swerve -100/+100 ;
void key_mode_select(u8 keyput);

#endif





