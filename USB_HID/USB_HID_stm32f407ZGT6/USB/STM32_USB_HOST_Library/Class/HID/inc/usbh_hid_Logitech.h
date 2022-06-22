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


#include "usbh_hid_core.h"
#include "usb_conf_usr.h"


typedef struct _HID_Logitech_Data
{
	uint8_t X_L;
	uint8_t Y_L:6;
	uint8_t X_H:2;
	uint8_t Hat_Switch:4;
	uint8_t Y_H:4;
	uint8_t RZ;
	uint8_t button_1_8;
	uint8_t Slider;
	//uint8_t zero:4;
	//uint8_t button_9_12:4;
	uint8_t button_9_12;
} HID_Logitech_Data_TypeDef;
extern HID_Logitech_Data_TypeDef  HID_Logitech_Data;

extern HID_cb_TypeDef  HID_Logitech_cb;


void  USR_Logitech_Init (void);
void  USR_Logitech_ProcessData (HID_Logitech_Data_TypeDef *data);

#ifdef __cplusplus
}
#endif

#endif //__USBH_HID_LOGITECH_H
