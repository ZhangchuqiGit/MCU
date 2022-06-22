//
// Created by zcq on 2021/5/3.
//

#ifndef __USBH_HID_LOGITECH_H
#define __USBH_HID_LOGITECH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
# include <string.h>

#include "usbh_hid_core.h"
#include "usb_conf_usr.h"

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
typedef struct
{
	uint16_t X;
	uint16_t Y;
	uint8_t RZ;
	uint8_t Slider;
	uint8_t Hat_Switch;
	uint16_t button;
	
} HID_Logitech_Data_Analyze;
extern HID_Logitech_Data_Analyze  Logitech_Data;

extern HID_cb_TypeDef  HID_Logitech_cb;  // 用户 USB 设备 初始化、驱动

void  USR_Logitech_Init (void);
void  USR_Logitech_ProcessData (HID_Logitech_Data_Analyze *Logitech_data);

#ifdef __cplusplus
}
#endif

#endif //__USBH_HID_LOGITECH_H
