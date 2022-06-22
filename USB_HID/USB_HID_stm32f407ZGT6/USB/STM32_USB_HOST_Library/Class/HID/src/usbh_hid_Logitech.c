//
// Created by zcq on 2021/5/3.
//

#include "usbh_hid_Logitech.h"


static void Logitech_Init(void);
static void Logitech_Decode(uint8_t *data);

HID_Logitech_Data_TypeDef  HID_Logitech_Data;
HID_cb_TypeDef HID_Logitech_cb = {
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

/**
* @brief  MOUSE_Decode
*         Decode Mouse data
* @param  data : Pointer to Mouse HID data buffer
* @retval None       */
static void Logitech_Decode(uint8_t *data)
{
	HID_Logitech_Data.X_L = data[0];
	HID_Logitech_Data.Y_L = (data[1] >> 2) & 0x3f;
	HID_Logitech_Data.X_H = data[1] & 0x03;
	HID_Logitech_Data.Hat_Switch = (data[2] >> 4) & 0x0f;
	HID_Logitech_Data.Y_H = data[2] & 0x0f;
	HID_Logitech_Data.RZ = data[3];
	HID_Logitech_Data.button_1_8 = data[4];
	HID_Logitech_Data.Slider = data[5];
	HID_Logitech_Data.button_9_12 = data[6];
	
	/* Logitech Extreme 3D 飞行摇杆 数据处理 */
	USR_Logitech_ProcessData (&HID_Logitech_Data);
}

