//
// Created by zcq on 2021/5/3.
//

#include "usbh_hid_Logitech.h"

static void Logitech_Init(void);
static void Logitech_Decode(uint8_t *data); // Logitech Extreme 3D 飞行摇杆 数据处理

HID_cb_TypeDef HID_Logitech_cb = {  // 用户 USB 设备 初始化、驱动
		Logitech_Init,
		Logitech_Decode
};

static void Logitech_Init(void)
{
	/* Call User Init*/
	USR_Logitech_Init();
}

/* 定义在 ./USB/STM32_USB_HOST_Library/Class/HID/src/usbh_hid_core.c */
extern HID_Machine_TypeDef HID_Machine;

HID_Logitech_Data_Analyze  Logitech_Data;

/** 数据格式    			*data  
	uint8_t X_L;			data[0]
	uint8_t Y_L:6;			data[1]
	uint8_t X_H:2;
	uint8_t Hat_Switch:4;	data[2]
	uint8_t Y_H:4;
	uint8_t RZ;				data[3]
	uint8_t button_1_8;		data[4]
	uint8_t Slider;			data[5]
	uint8_t zero:4;			data[6]
	uint8_t button_9_12:4;  **/
static void Logitech_Decode(uint8_t *data/*所求数据*/) // Logitech Extreme 3D 飞行摇杆 数据处理
{
	Logitech_Data.X = ((data[1] & 0x03) << 8) | data[0];
	Logitech_Data.Y = ((data[2] & 0x0f) << 6) | (data[1] >> 2);
	Logitech_Data.RZ = data[3];
	Logitech_Data.Slider = data[5];
	Logitech_Data.Hat_Switch = data[2] >> 4;
	Logitech_Data.button = data[6] << 8 | data[4];

	USR_Logitech_ProcessData (&Logitech_Data);
}


