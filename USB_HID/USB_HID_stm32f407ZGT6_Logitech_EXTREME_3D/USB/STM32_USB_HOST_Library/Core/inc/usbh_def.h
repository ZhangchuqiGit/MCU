/**
  ******************************************************************************
  * @file    usbh_def.h
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   Definitions used in the USB host library
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

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_LIB_CORE
* @{
*/

/** @defgroup USBH_DEF
  * @brief This file is includes USB descriptors
  * @{
  */

#ifndef  USBH_DEF_H
#define  USBH_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#ifndef USBH_NULL
#define USBH_NULL ((void *)0)
#endif

#ifndef NULL
#define NULL  0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


#define ValBit(VAR,POS)                               (VAR & (1 << POS))
#define SetBit(VAR,POS)                               (VAR |= (1 << POS))
#define ClrBit(VAR,POS)                               (VAR &= ((1 << POS)^255))

#define  LE16(addr)             (((u16)(*((u8 *)(addr))))\
                                + (((u16)(*(((u8 *)(addr)) + 1))) << 8))

#define LE24(addr)        (((uint32_t)(addr)[0]) | \
                           (((uint32_t)(addr)[1]) << 8) | \
                           (((uint32_t)(addr)[2]) << 16))

#define LE32(addr)        (((uint32_t)(addr)[0]) | \
                           (((uint32_t)(addr)[1]) << 8) | \
                           (((uint32_t)(addr)[2]) << 16) | \
                           (((uint32_t)(addr)[3]) << 24))

#define LE64(addr)        (((uint64_t)(addr)[0]) | \
                           (((uint64_t)(addr)[1]) << 8) | \
                           (((uint64_t)(addr)[2]) << 16) | \
                           (((uint64_t)(addr)[3]) << 24) | \
                           (((uint64_t)(addr)[4]) << 32) | \
                           (((uint64_t)(addr)[5]) << 40) | \
                           (((uint64_t)(addr)[6]) << 48) | \
                           (((uint64_t)(addr)[7]) << 56))

#define LE16S(addr)       ((int16_t)(LE16((addr))))
#define LE24S(addr)       ((int32_t)(LE24((addr))))
#define LE32S(addr)       ((int32_t)(LE32((addr))))
#define LE64S(addr)       ((int64_t)(LE64((addr))))


#define  USB_LEN_DESC_HDR                               0x02
#define  USB_LEN_DEV_DESC                               0x12
#define  USB_LEN_CFG_DESC                               0x09
#define  USB_LEN_IF_DESC                                0x09
#define  USB_LEN_EP_DESC                                0x07
#define  USB_LEN_OTG_DESC                               0x03
#define  USB_LEN_SETUP_PKT                              0x08

/* bmRequestType :D7 Data Phase Transfer Direction  */
#define  USB_REQ_DIR_MASK                               0x80
#define  USB_H2D                                        0x00
#define  USB_D2H                                        0x80

/* bmRequestType D6..5 Type */
#define  USB_REQ_TYPE_STANDARD                          0x00
#define  USB_REQ_TYPE_CLASS                             0x20
#define  USB_REQ_TYPE_VENDOR                            0x40
#define  USB_REQ_TYPE_RESERVED                          0x60

/* bmRequestType D4..0 Recipient */
#define  USB_REQ_RECIPIENT_DEVICE                       0x00
#define  USB_REQ_RECIPIENT_INTERFACE                    0x01
#define  USB_REQ_RECIPIENT_ENDPOINT                     0x02
#define  USB_REQ_RECIPIENT_OTHER                        0x03

/* Table 9-4. Standard Request Codes  */
/* bRequest , Value */
#define  USB_REQ_GET_STATUS                             0x00
#define  USB_REQ_CLEAR_FEATURE                          0x01
#define  USB_REQ_SET_FEATURE                            0x03
#define  USB_REQ_SET_ADDRESS                            0x05
#define  USB_REQ_GET_DESCRIPTOR                         0x06
#define  USB_REQ_SET_DESCRIPTOR                         0x07
#define  USB_REQ_GET_CONFIGURATION                      0x08
#define  USB_REQ_SET_CONFIGURATION                      0x09
#define  USB_REQ_GET_INTERFACE                          0x0A
#define  USB_REQ_SET_INTERFACE                          0x0B
#define  USB_REQ_SYNCH_FRAME                            0x0C

/* Table 9-5. Descriptor Types of USB Specifications */
#define  USB_DESC_TYPE_DEVICE                              1
#define  USB_DESC_TYPE_CONFIGURATION                       2
#define  USB_DESC_TYPE_STRING                              3
#define  USB_DESC_TYPE_INTERFACE                           4
#define  USB_DESC_TYPE_ENDPOINT                            5
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    6
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           7
#define  USB_DESC_TYPE_INTERFACE_POWER                     8
#define  USB_DESC_TYPE_HID                                 0x21
#define  USB_DESC_TYPE_HID_REPORT                          0x22


#define USB_DEVICE_DESC_SIZE                               18
#define USB_CONFIGURATION_DESC_SIZE                        9
#define USB_HID_DESC_SIZE                                  9
#define USB_INTERFACE_DESC_SIZE                            9
#define USB_ENDPOINT_DESC_SIZE                             7

/* Descriptor Type and Descriptor Index  */
/* Use the following values when calling the function USBH_GetDescriptor  */
#define  USB_DESC_DEVICE                    ((USB_DESC_TYPE_DEVICE << 8) & 0xFF00)
#define  USB_DESC_CONFIGURATION             ((USB_DESC_TYPE_CONFIGURATION << 8) & 0xFF00)
#define  USB_DESC_STRING                    ((USB_DESC_TYPE_STRING << 8) & 0xFF00)
#define  USB_DESC_INTERFACE                 ((USB_DESC_TYPE_INTERFACE << 8) & 0xFF00)
#define  USB_DESC_ENDPOINT                  ((USB_DESC_TYPE_INTERFACE << 8) & 0xFF00)
#define  USB_DESC_DEVICE_QUALIFIER          ((USB_DESC_TYPE_DEVICE_QUALIFIER << 8) & 0xFF00)
#define  USB_DESC_OTHER_SPEED_CONFIGURATION ((USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION << 8) & 0xFF00)
#define  USB_DESC_INTERFACE_POWER           ((USB_DESC_TYPE_INTERFACE_POWER << 8) & 0xFF00)
#define  USB_DESC_HID_REPORT                ((USB_DESC_TYPE_HID_REPORT << 8) & 0xFF00)
#define  USB_DESC_HID                       ((USB_DESC_TYPE_HID << 8) & 0xFF00)


#define  USB_EP_TYPE_CTRL                               0x00
#define  USB_EP_TYPE_ISOC                               0x01
#define  USB_EP_TYPE_BULK                               0x02
#define  USB_EP_TYPE_INTR                               0x03

#define  USB_EP_DIR_OUT                                 0x00
#define  USB_EP_DIR_IN                                  0x80
#define  USB_EP_DIR_MSK                                 0x80

/* supported classes */
#define USB_MSC_CLASS                                   0x08
#define USB_HID_CLASS                                   0x03

/* Interface Descriptor field values for HID Boot Protocol */
#define HID_BOOT_CODE            0x01
#define HID_Logitech_BOOT_CODE   0x00 // 适配 Logitech Extreme 3D 飞行摇杆
#define HID_KEYBRD_BOOT_CODE     0x01
#define HID_MOUSE_BOOT_CODE      0x02

/* As per USB specs 9.2.6.4 :Standard request with data request timeout: 5sec
   Standard request with no data stage timeout : 50ms */
#define DATA_STAGE_TIMEOUT                              5000
#define NODATA_STAGE_TIMEOUT                            50

#define USBH_CONFIGURATION_DESCRIPTOR_SIZE (USB_CONFIGURATION_DESC_SIZE \
                                           + USB_INTERFACE_DESC_SIZE\
                                           + (USBH_MAX_NUM_ENDPOINTS * USB_ENDPOINT_DESC_SIZE))

#define CONFIG_DESC_wTOTAL_LENGTH (ConfigurationDescriptorData.ConfigDescfield.\
                                          ConfigurationDescriptor.wTotalLength)

/*  This Union is copied from usb_core.h  */
typedef union
{
	uint16_t w;
	struct BW
	{
		uint8_t msb;
		uint8_t lsb;
	}
			bw;
} uint16_t_uint8_t;

typedef union _USB_Setup
{
	uint8_t d8[8];

	struct _SetupPkt_Struc
	{
		uint8_t           bmRequestType;
		uint8_t           bRequest;
		uint16_t_uint8_t  wValue;
		uint16_t_uint8_t  wIndex;
		uint16_t_uint8_t  wLength;
	} b;
} USB_Setup_TypeDef;

typedef  struct  _DescHeader
{
	uint8_t  bLength;
	uint8_t  bDescriptorType;
} USBH_DescHeader_t;

typedef struct _DeviceDescriptor // 标准的设备描述符
{
	uint8_t   bLength; 			//设备描述符的字节数
	uint8_t   bDescriptorType;	//设备描述符类型编号
	uint16_t  bcdUSB;       	//USB版本号
	uint8_t   bDeviceClass;		//USB分配的设备类
	uint8_t   bDeviceSubClass;	//USB分配的设备子类
	uint8_t   bDeviceProtocol; 	//USB分配的设备协议代码
	uint8_t   bMaxPacketSize;	//端点的最大包大小
	uint16_t  idVendor;      /* 厂商编号（由USB Org分配）Vendor ID (Assigned by USB Org) */
	uint16_t  idProduct;     /* 产品编号（由制造商分配） Product ID (Assigned by Manufacturer) */
	uint16_t  bcdDevice;     /* 设备出厂编号 Device Release Number */
	uint8_t   iManufacturer;  /* 设备厂商字符串索引 */
	uint8_t   iProduct;       /* 产品字符串索引 */
	uint8_t   iSerialNumber;  /* 设备序列号索引 */
	uint8_t   bNumConfigurations; /* 可能的配置数量 Number of Possible Configurations */
} USBH_DevDesc_TypeDef;
/** USB键盘类描述符实例 		_DeviceDescriptor // 标准的设备描述符
{
    0x12,			  //设备描述符的字节数大小
    DEVICE_DESCRIPTOR,//设备描述符类型编号
    0x0002,           //USB版本号
    0x00,             //USB分配的设备类代码
    0x00,             //USB分配的子类代码
    0x00,             //USB分配的设备协议代码
    0x40,             //端点0的最大包大小
    0ff0,             //厂商编号VID，这里只是作为测试用，请使用自己的VID
    0xff03,           //产品编号PID，这里只是作为测试用，请使用自己的PID
    0x0001,           //设备出厂编号
    0x01,             //设备厂商字符串的索引
    0x02,             //描述产品字符串的索引
    0x03,             //描述设备序列号字符串的索引
    0x01              //可能的配置数
}
** USB鼠标
	sizeof(Device_Descriptor), //设备描述符的字节数大小
	DEVICE_DESCRIPTOR,         //设备描述符类型编号
	SWAP16(0x0110),            //USB版本号,usb1.1
	0x00,           //USB分配的设备类代码
	0x00,           //USB分配的子类代码
	0x00,           //USB分配的设备协议代码
	0x10,           //端点0的最大包大小
	SWAP16(0x8888), //厂商编号VID，这里只是作为测试用，请使用自己的VID
	SWAP16(0x0002), //产品编号PID，这里只是作为测试用，请使用自己的PID
	SWAP16(0x0100), //设备出厂编号
	0x01,           //设备厂商字符串的索引
	0x02,           //描述产品字符串的索引
	0x03,           //描述设备序列号字符串的索引
	0x01            //可能的配置数量
};      */

typedef struct _ConfigurationDescriptor // 配置描述符
{
	uint8_t bLength; 			//配置描述符 的字节数
	uint8_t bDescriptorType;	//配置描述符类型编号
	uint16_t wTotalLength;   	//此配置返回的所有数据大小
	uint8_t bNumInterfaces;		//此配置支持的接口数量
	uint8_t bConfigurationValue;// Set_Configuration 命令所需要的参数
	uint8_t iConfiguration;		//描述该配置的字符串索引
	uint8_t bmAttributes;		//供电模式的选择
	uint8_t bMaxPower;		    //设备从总线获取的最大电流
} USBH_CfgDesc_TypeDef;

typedef struct _HIDDescriptor
{
	uint8_t   bLength;
	uint8_t   bDescriptorType;
	uint16_t  bcdHID;               /* indicates what endpoint this descriptor is describing */
	uint8_t   bCountryCode;        /* specifies the transfer type. */
	uint8_t   bNumDescriptors;     /* specifies the transfer type. */
	uint8_t   bReportDescriptorType;    /* Maximum Packet Size this endpoint is capable of sending or receiving */
	uint16_t  wItemLength;          /* is used to specify the polling interval of certain transfers. */
} USBH_HIDDesc_TypeDef;

typedef struct _InterfaceDescriptor // 接口描述符
{
	uint8_t	bLength;		    //接口描述符的字节数
	uint8_t	bDescriptorType;	//接口描述符的类型编号
	uint8_t bInterfaceNumber;	//该接口的编号
	uint8_t bAlternateSetting;	//备用的接口描述符的编号
	uint8_t bNumEndpoints;		//该接口使用 的端点数，不包括端点0
	uint8_t bInterfaceClass;	//接口类
	uint8_t bInterfaceSubClass;	//接口子类
	uint8_t bInterfaceProtocol;	//接口类协议
	uint8_t iInterface;		    //描述该接口的字符串索引值
} USBH_InterfaceDesc_TypeDef;

typedef struct _EndpointDescriptor // 端点描述符
{
	uint8_t bLength;		    //端点描述符字节数
	uint8_t bDescriptorType;	//端点描述符类型编号
	uint8_t bEndpointAddress;	//端点地址及输入输出类型
	uint8_t bmAttributes;		//端点的传输类型
	uint16_t wMaxPacketSize;    //端点收发的最大包大小
	uint8_t bInterval;          //主机查询端点的时间间隔
} USBH_EpDesc_TypeDef;

#ifdef __cplusplus
{
#endif

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

