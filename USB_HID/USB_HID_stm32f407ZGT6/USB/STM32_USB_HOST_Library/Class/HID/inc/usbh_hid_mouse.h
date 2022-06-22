/**
  ******************************************************************************
  * @file    usbh_hid_mouse.h 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file contains all the prototypes for the usbh_hid_mouse.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_HID_MOUSE_H
#define __USBH_HID_MOUSE_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_core.h"


typedef struct _HID_MOUSE_Data
{
	uint8_t              x;
	uint8_t              y;
	uint8_t              z;
	uint8_t              button;
} HID_MOUSE_Data_TypeDef;

extern HID_cb_TypeDef HID_MOUSE_cb;
extern HID_MOUSE_Data_TypeDef	 HID_MOUSE_Data;


///* 因为当前鼠标多了一个滚轮和中键, 所以要修改结构体HID_MOUSE_Info_TypeDef: */
//typedef struct _HID_MOUSE_Info
//{
//	uint8_t type;
//	uint8_t buttons[3];
//	int8_t  x;
//	int8_t  y;
//	int8_t  z;
//	uint8_t key;
//} HID_MOUSE_Info_TypeDef;
//
//extern HID_MOUSE_Info_TypeDef HID_MOUSE_Info;

/** @defgroup USBH_HID_MOUSE_Exported_FunctionsPrototype
  * @{
  */
void  USR_MOUSE_Init (void);
void  USR_MOUSE_ProcessData (HID_MOUSE_Data_TypeDef *data);
/**
  * @}
  */

#endif /* __USBH_HID_MOUSE_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
