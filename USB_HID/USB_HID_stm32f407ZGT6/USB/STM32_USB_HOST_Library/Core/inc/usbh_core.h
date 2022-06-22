/**
  ******************************************************************************
  * @file    usbh_core.h
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   Header file for usbh_core.c
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
#ifndef __USBH_CORE_H
#define __USBH_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usb_hcd.h"
#include "usbh_def.h"
#include "usb_hid_conf_usr.h"

#ifndef __debug_H
#include "debug.h" 
#endif

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_CORE
  * @brief This file is the Header file for usbh_core.c
  * @{
  */

/** @defgroup USBH_CORE_Exported_Defines
  * @{
  */

#define MSC_CLASS                         0x08
#define HID_CLASS                         0x03
#define MSC_PROTOCOL                      0x50
#define CBI_PROTOCOL                      0x01

#define USBH_MAX_ERROR_COUNT                            2
#define USBH_DEVICE_ADDRESS_DEFAULT                     0
#define USBH_DEVICE_ADDRESS                             1

/* @defgroup USBH_CORE_Exported_Types USB 状态 */
typedef enum {
	USBH_OK   = 0,
	USBH_BUSY,
	USBH_FAIL,
	USBH_NOT_SUPPORTED,
	USBH_UNRECOVERED_ERROR,
	USBH_ERROR_SPEED_UNKNOWN,
	USBH_APPLY_DEINIT
} USBH_Status;

/* Following states are used for gState 主机状态 */
typedef enum {
	HOST_IDLE =0,			//主机空闲
	HOST_DEV_ATTACHED,		//设备已插入
	HOST_DEV_DISCONNECTED,  //设备已拔除
	HOST_DETECT_DEVICE_SPEED,//
	HOST_ENUMERATION,		//枚举中
	HOST_CLASS_REQUEST,  	//为发送设备所属类的请求做准备
	HOST_CLASS,				//处理各种类请求
	HOST_CTRL_XFER,			//过程控制转移状态机
	HOST_USR_INPUT,			//等待用户输入
	HOST_SUSPENDED,			//挂起
	HOST_ERROR_STATE  		//错误
} HOST_State;

/* Following states are used for EnumerationState 枚举状态 */
typedef enum {
	ENUM_IDLE = 0,			//枚举空闲
	ENUM_GET_FULL_DEV_DESC,
	ENUM_SET_ADDR,
	ENUM_GET_CFG_DESC,//获取标准配置描述符
	ENUM_GET_FULL_CFG_DESC,	//设备配置描述符
	ENUM_GET_MFC_STRING_DESC,//制造商字符串描述符
	ENUM_GET_PRODUCT_STRING_DESC,//产品字符串索引
	ENUM_GET_SERIALNUM_STRING_DESC,//设备序列号索引
	ENUM_SET_CONFIGURATION,
	ENUM_DEV_CONFIGURED
} ENUM_State;

/* Following states are used for Ctrl Xfer State Machine */
typedef enum {
	CTRL_IDLE =0,		//控制空闲
	CTRL_SETUP,			//设置
	CTRL_SETUP_WAIT,
	CTRL_DATA_IN,
	CTRL_DATA_IN_WAIT,
	CTRL_DATA_OUT,
	CTRL_DATA_OUT_WAIT,
	CTRL_STATUS_IN,
	CTRL_STATUS_IN_WAIT,
	CTRL_STATUS_OUT,
	CTRL_STATUS_OUT_WAIT,
	CTRL_ERROR,
	CTRL_STALLED,
	CTRL_COMPLETE
} CTRL_State;			// 控制状态

/* USB USR 状态 */
typedef enum {
	USBH_USR_NO_RESP   = 0,
	USBH_USR_RESP_OK = 1,
} USBH_USR_Status;

/* Following states are used for RequestState 请求状态 */
typedef enum {
	CMD_IDLE =0,		//请求空闲
	CMD_SEND,			//请求发送
	CMD_WAIT			//请求等待
} CMD_State;

typedef struct _Ctrl
{
	uint8_t               hc_num_in;
	uint8_t               hc_num_out;
	uint8_t               ep0size;
	uint8_t               *buff;
	uint16_t              length;
	uint8_t               errorcount;
	uint16_t              timer;
	CTRL_STATUS           status;
	USB_Setup_TypeDef     setup;
	CTRL_State            state;
} USBH_Ctrl_TypeDef;	// 控制类型

typedef struct _DeviceProp // 设备描述符
{
	uint8_t                           address;
	uint8_t                           speed;
	USBH_DevDesc_TypeDef              Dev_Desc;
	USBH_CfgDesc_TypeDef              Cfg_Desc;
	USBH_InterfaceDesc_TypeDef        Itf_Desc[USBH_MAX_NUM_INTERFACES];
	USBH_EpDesc_TypeDef               Ep_Desc[USBH_MAX_NUM_INTERFACES][USBH_MAX_NUM_ENDPOINTS];
	USBH_HIDDesc_TypeDef              HID_Desc;
}USBH_Device_TypeDef;

typedef struct _USBH_Class_cb
{
	USBH_Status  (*Init) (USB_OTG_CORE_HANDLE *pdev , void *phost);
	void         (*DeInit) (USB_OTG_CORE_HANDLE *pdev , void *phost);//重新初始化
	USBH_Status  (*Requests) (USB_OTG_CORE_HANDLE *pdev , void *phost);
	USBH_Status  (*Machine) (USB_OTG_CORE_HANDLE *pdev , void *phost);

} USBH_Class_cb_TypeDef;//类回调结构地址

/** ./USB/USB_APP/usb_hid_usr.c & usb_hid_usr.h **/
typedef struct _USBH_USR_PROP
{
	void (*Init)(void);       /* HostLibInitialized */
	void (*DeInit)(void);       /* HostLibInitialized 重新初始化 */
	void (*DeviceAttached)(void);           /* DeviceAttached */
	void (*ResetDevice)(void);
	void (*DeviceDisconnected)(void);
	void (*OverCurrentDetected)(void);
	void (*DeviceSpeedDetected)(uint8_t DeviceSpeed);          /* DeviceSpeed */
	void (*DeviceDescAvailable)(void *);    /* DeviceDescriptor is available */
	void (*DeviceAddressAssigned)(void);  /* Address is assigned to USB Device */
	void (*ConfigurationDescAvailable)(USBH_CfgDesc_TypeDef *,
									   USBH_InterfaceDesc_TypeDef *,
									   USBH_EpDesc_TypeDef *);
	/* Configuration Descriptor available */
	void (*ManufacturerString)(void *);     /* ManufacturerString*/
	void (*ProductString)(void *);          /* ProductString*/
	void (*SerialNumString)(void *);        /* SerialNubString*/
	void (*EnumerationDone)(void);           /* Enumeration finished */
	USBH_USR_Status (*UserInput)(void);
	int  (*UserApplication) (void);
	void (*DeviceNotSupported)(void); /* Device is not supported*/
	void (*UnrecoveredError)(void);

} USBH_Usr_cb_TypeDef;//用户回调结构地址

typedef struct _Host_TypeDef // 主机类型定义
{
	HOST_State            gState;       /* 主机状态 Host State Machine Value */
	HOST_State            gStateBkp;    /* 先前的主机状态备份 backup of previous State machine value */
	ENUM_State            EnumState;    /* 枚举状态 Enumeration state Machine */
	CMD_State             RequestState; /* 请求状态 */
	USBH_Ctrl_TypeDef     Control;		/* 控制类型 */
	USBH_Device_TypeDef   device_prop;	/* USB 设备描述符 */
	USBH_Class_cb_TypeDef *class_cb; 	//类回调结构地址
	USBH_Usr_cb_TypeDef   *usr_cb; 		//用户回调结构地址
} USBH_HOST, *pUSBH_HOST;

/** @defgroup USBH_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBH_CORE_Exported_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup USBH_CORE_Exported_FunctionsPrototype
  * @{
  */

/*主机硬件和堆栈初始化*/
void USBH_Init(USB_OTG_CORE_HANDLE *pdev,
			   USB_OTG_CORE_ID_TypeDef coreID,
			   USBH_HOST *phost,
			   USBH_Class_cb_TypeDef *class_cb, //类回调结构地址
			   USBH_Usr_cb_TypeDef *usr_cb); // 用户回调结构地址

/*重新初始化主机*/
USBH_Status USBH_DeInit(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

/*核心主状态机过程*/
void USBH_Process(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

void USBH_ErrorHandle(USBH_HOST *phost, USBH_Status errType);


#ifdef __cplusplus
{
#endif

#endif /* __USBH_CORE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



