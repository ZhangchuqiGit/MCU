#include "usb_hid_usr.h"
#include "usb_hcd_int.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "usbh_hid_Logitech.h"
#include "delay.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//USBH-USR 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   

//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 UsbDeviceState=0;		//默认没有连接

/* USB HOST 用户回调函数 */
USBH_Usr_cb_TypeDef USR_Callbacks = {
		USBH_USR_Init,
		USBH_USR_DeInit,
		USBH_USR_DeviceAttached,
		USBH_USR_ResetDevice,
		USBH_USR_DeviceDisconnected,
		USBH_USR_OverCurrentDetected,
		USBH_USR_DeviceSpeedDetected,
		USBH_USR_Device_DescAvailable,
		USBH_USR_DeviceAddressAssigned,
		USBH_USR_Configuration_DescAvailable,
		USBH_USR_Manufacturer_String,
		USBH_USR_Product_String,
		USBH_USR_SerialNum_String,
		USBH_USR_EnumerationDone,
		USBH_USR_UserInput,
		NULL,
		USBH_USR_DeviceNotSupported,
		USBH_USR_UnrecoveredError
};


/************************************************************/
extern USB_OTG_CORE_HANDLE USB_OTG_Core_dev;

/*  USB信息显示
msgx:0,USB无连接
     1,USB键盘
     2,USB鼠标
     3,不支持的USB设备 */
void USBH_Msg_Show(u8 msgx)
{
	switch(msgx)
	{
		case 1:	//USB无连接
		_debug_log_info("USB无连接")
			break;
		case 2:	//USB键盘
		_debug_log_info("USB键盘")
			break;
		case 3:	//USB鼠标
		_debug_log_info("USB鼠标")
			break;
		case 4:	//USB Logitech Extreme 3D 飞行摇杆
		_debug_log_info("Logitech Extreme 3D 飞行摇杆")
			break;
		default:	//不支持的USB设备`
		_debug_log_info("不支持的USB设备")
			break;
	}
}

u8 USB_FIRST_PLUGIN_FLAG=0;	//USB第一次插入标志,如果为1,说明是第一次插入
/************************************************************/


//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void) // 主机检测到设备插入状态会被置位
{
	USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}

//USB HOST 初始化 
void USBH_USR_Init(void)
{
	_debug_log_info("USB OTG FS MSC Host");
	_debug_log_info("> USB Host library started.");
	_debug_log_info("  USB Host Library v2.1.0\r\n");

}
//检测到U盘插入
void USBH_USR_DeviceAttached(void)//U盘插入
{
	_debug_log_info("检测到USB设备插入")
}
//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void)//U盘移除
{
	_debug_log_info("USB设备拔出!");
	UsbDeviceState=0;	//USB设备拔出了
	USBH_Msg_Show(1);//USB无连接
}
//复位从机
void USBH_USR_ResetDevice(void)
{
	_debug_log_info("复位USB设备");
}
//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
		_debug_log_info("高速(HS)USB设备!");
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
		_debug_log_info("全速(FS)USB设备!");
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
		_debug_log_info("低速(LS)USB设备!");
	}
	else
	{
		_debug_log_info("设备错误!");
	}
}

//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
	USBH_DevDesc_TypeDef *hs = DeviceDesc;

	/** 标准的设备描述符 **/
	printf("\r\n标准的设备描述符\tstruct _DeviceDescriptor{}\r\n");

	printf("设备描述符的字节数\t0x%x\tuint8_t bLength\r\n", hs->bLength);
	printf("设备描述符类型编号\t0x%x\tuint8_t bDescriptorType\r\n", hs->bDescriptorType);
	printf("USB版本号\t0x%x\tuint16_t bcdUSB\r\n", hs->bcdUSB);
	printf("USB分配的设备类\t0x%x\tuint8_t bDeviceClass\r\n", hs->bDeviceClass);
	printf("USB分配的设备子类\t0x%x\tuint8_t bDeviceSubClass\r\n", hs->bDeviceSubClass);
	printf("USB分配的设备协议代码\t0x%x\tuint8_t bDeviceProtocol\r\n", hs->bDeviceProtocol);
	printf("端点的最大包大小\t0x%x\tuint8_t bMaxPacketSize\r\n", hs->bMaxPacketSize);
	printf("厂商编号（由USB Org分配）\t0x%x\tuint16_t idVendor\r\n", hs->idVendor);
	printf("产品编号（由制造商分配）\t0x%x\tuint16_t idProduct\r\n", hs->idProduct);
	printf("设备出厂编号\t0x%x\tuint16_t bcdDevice\r\n", hs->bcdDevice);
	printf("设备厂商字符串索引\t0x%x\tuint8_t iManufacturer\r\n", hs->iManufacturer);
	printf("产品字符串索引\t0x%x\tuint8_t iProduct\r\n", hs->iProduct);
	printf("设备序列号索引\t0x%x\tuint8_t 设备序列号索引\r\n", hs->iSerialNumber);
	printf("可能的配置数量\t0x%x\tuint8_t bNumConfigurations\r\n\r\n", hs->bNumConfigurations);
}

//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void)
{
	_debug_log_info("从机地址分配成功!");
}

//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,// 配置描述符
										  USBH_InterfaceDesc_TypeDef *itfDesc,// 接口描述符
										  USBH_EpDesc_TypeDef *epDesc)// 端点描述符
{
	if((*itfDesc).bInterfaceClass==0x08)//接口类
	{
		_debug_log_info("可移动存储器设备!");
	}
	else if((*itfDesc).bInterfaceClass==0x03)
	{
		_debug_log_info("HID 设备!");
	}
	else _debug_log_info("其它 设备: %x", (*itfDesc).bInterfaceClass);

	/** 配置 描述符 **/
	printf("\r\n配置 描述符\tstruct _ConfigurationDescriptor{}\r\n");

	printf("设备描述符的字节数\t0x%x\tuint8_t bLength\r\n", cfgDesc->bLength);
	printf("配置描述符类型编号\t0x%x\tuint8_t bDescriptorType\r\n", cfgDesc->bDescriptorType);
	printf("此配置返回的所有数据大小\t0x%x\tuint16_t wTotalLength\r\n", cfgDesc->wTotalLength);
	printf("此配置支持的接口数量\t0x%x\tuint8_t bNumInterfaces\r\n", cfgDesc->bNumInterfaces);
	printf("命令所需要的参数\t0x%x\tuint8_t bConfigurationValue\r\n", cfgDesc->bConfigurationValue);
	printf("描述该配置的字符串索引\t0x%x\tuint8_t iConfiguration\r\n", cfgDesc->iConfiguration);
	printf("供电模式的选择\t0x%x\tuint8_t bmAttributes\r\n", cfgDesc->bmAttributes);
	printf("设备从总线获取的最大电流\t0x%x\tuint8_t bMaxPower\r\n", cfgDesc->bMaxPower);

	/** 接口 描述符 **/
	printf("\r\n接口 描述符\tstruct _InterfaceDescriptor{}\r\n");

	printf("接口描述符的字节数\t0x%x\tuint8_t bLength\r\n", itfDesc->bLength);
	printf("接口描述符的类型编号\t0x%x\tuint8_t bDescriptorType\r\n", itfDesc->bDescriptorType);
	printf("该接口的编号\t0x%x\tuint8_t wTotalLength\r\n", itfDesc->bInterfaceNumber);
	printf("备用的接口描述符的编号\t0x%x\tuint8_t bNumInterfaces\r\n", itfDesc->bAlternateSetting);
	printf("该接口使用 的端点数，不包括端点0\t0x%x\tuint8_t bConfigurationValue\r\n", itfDesc->bNumEndpoints);
	printf("接口类\t0x%x\tuint8_t iConfiguration\r\n", itfDesc->bInterfaceClass);
	printf("接口子类\t0x%x\tuint8_t bmAttributes\r\n", itfDesc->bInterfaceSubClass);
	printf("接口类协议\t0x%x\tuint8_t bMaxPower\r\n", itfDesc->bInterfaceProtocol);
	printf("描述该接口的字符串索引值\t0x%x\tuint8_t bMaxPower\r\n", itfDesc->iInterface);

	/** 端点 描述符 **/
	printf("\r\n端点 描述符\tstruct _EndpointDescriptor{}\r\n");

	printf("端点描述符字节数\t0x%x\tuint8_t bLength\r\n", epDesc->bLength);
	printf("端点描述符类型编号\t0x%x\tuint8_t bDescriptorType\r\n", epDesc->bDescriptorType);
	printf("端点地址及输入输出类型\t0x%x\tuint8_t wTotalLength\r\n", epDesc->bEndpointAddress);
	printf("端点的传输类型\t0x%x\tuint8_t bNumInterfaces\r\n", epDesc->bmAttributes);
	printf("端点收发的最大包大小\t0x%x\tuint16_t iConfiguration\r\n", epDesc->wMaxPacketSize);
	printf("主机查询端点的时间间隔\t0x%x\tuint8_t bmAttributes\r\n", epDesc->bInterval);
}

//获取 制造商字符串描述符
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	_debug_log_info("制造商字符串描述符 Manufacturer: %s",(char *)ManufacturerString);
}

//获取 产品/设备 名
void USBH_USR_Product_String(void *ProductString)
{
	_debug_log_info("Product: %s",(char *)ProductString);
}

//获取到设备SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
	_debug_log_info("Serial Number: %s",(char *)SerialNumString);
}

//设备USB枚举完成
void USBH_USR_EnumerationDone(void)
{
	_debug_log_info("设备枚举完成!\r\n");
}

//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{
	USBH_Msg_Show(0);//无法识别的USB设备
	//_debug_log_info("无法识别的USB设备!\r\n");    
}

//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{
	_debug_log_info("跳过用户确认步骤!");
	UsbDeviceState=1;//USB设备已经连接成功
	return USBH_USR_RESP_OK;
}

//USB接口电流过载
void USBH_USR_OverCurrentDetected (void)
{
	_debug_log_info("端口电流过大!!!");
}

//重新初始化
void USBH_USR_DeInit(void)
{
	_debug_log_info("重新初始化!!!");
}

//无法恢复的错误!!  
void USBH_USR_UnrecoveredError (void)
{
	_debug_log_info("无法恢复的错误!!!\r\n");
}

//////////////////////////////////////////////////////////////////////////////////////////
//下面两个函数,为ALIENTEK添加,以防止USB死机

//USB HID通信死机检测,防止USB通信死机(暂时仅针对:DTERR,即Data toggle error)
//pcore:USB_OTG_Core_dev_HANDLE结构体指针
//phidm:HID_Machine_TypeDef结构体指针
//返回值:0,没有死机
//       1,死机了,外部必须重新启动USB连接.
u8 USBH_Check_HIDCommDead(USB_OTG_CORE_HANDLE *pcore,HID_Machine_TypeDef *phidm)
{
	if(pcore->host.HC_Status[phidm->hc_num_in]==HC_DATATGLERR)//检测到DTERR错误,直接重启USB.
	{
		return 1;
	}
	return 0;
}

//USB枚举状态死机检测,防止USB枚举失败导致的死机
//phost:USB_HOST结构体指针
//返回值:0,没有死机
//       1,死机了,外部必须重新启动USB连接.
u8 USBH_Check_EnumeDead(USBH_HOST *phost)
{
	static u16 errcnt=0;
	//这个状态,如果持续存在,则说明USB死机了.
	if(phost->gState==HOST_CTRL_XFER &&
	   (phost->EnumState==ENUM_IDLE || phost->EnumState==ENUM_GET_FULL_DEV_DESC))
	{
		errcnt++;
		if(errcnt>2000)//死机了
		{
			errcnt=0;
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS 复位
			delay_ms(5);
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//复位结束  
			return 1;
		}
	}else errcnt=0;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
/* Logitech Extreme 3D 飞行摇杆 初始化 */
void  USR_Logitech_Init (void) // 适配 Logitech Extreme 3D 飞行摇杆
{
	USBH_Msg_Show(4);		//USB Logitech Extreme 3D 飞行摇杆
	USB_FIRST_PLUGIN_FLAG=1;//标记第一次插入
}

//鼠标初始化
void USR_MOUSE_Init	(void)
{
	USBH_Msg_Show(3);		//USB 鼠标
	USB_FIRST_PLUGIN_FLAG=1;//标记第一次插入
}

//键盘初始化
void  USR_KEYBRD_Init(void)
{
	USBH_Msg_Show(2);		//USB 键盘
	USB_FIRST_PLUGIN_FLAG=1;//标记第一次插入
}

/* Logitech Extreme 3D 飞行摇杆 数据处理 */
void  USR_Logitech_ProcessData (HID_Logitech_Data_TypeDef *data)
{
	uint16_t X = data->X_H << 8 | data->X_L;
	uint16_t Y = data->Y_H << 6 | data->Y_L;
	printf("X: %d\tY: %d\t", X, Y);
	printf("Hat_Switch: %d\t", data->Hat_Switch);
	printf("RZ: %d\t", data->RZ);
	printf("button: ");
	for (uint16_t i=1, j=1; i & 0xff ; i = i<<1, ++j) {
		if (data->button_1_8 & i) printf("%d ", j);
	}
	for (uint8_t i=1, j=9; i & 0x0f ; i = i<<1, ++j) {
		if (data->button_9_12 & i) printf("%d ", j);
	}
	printf("\tSlider: %d\r\n", data->Slider);
}

//零时数组,用于存放鼠标坐标/键盘输入内容(4.3屏,最大可以输入2016字节)
__align(4) u8 tbuf[2017];

//USB鼠标数据处理
//data:USB鼠标数据结构体指针
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{
	static signed short x,y,z;
	if(USB_FIRST_PLUGIN_FLAG)//第一次插入,将数据清零
	{
		USB_FIRST_PLUGIN_FLAG=0;
		x=y=z=0;
	}
	x+=(signed char)data->x;
	if(x>9999)x=9999;
	if(x<-9999)x=-9999;
	y+=(signed char)data->y;
	if(y>9999)y=9999;
	if(y<-9999)y=-9999;
	z+=(signed char)data->z;
	if(z>9999)z=9999;
	if(z<-9999)z=-9999;
	sprintf((char*)tbuf,"BUTTON:");
	if(data->button&0X01)strcat((char*)tbuf,"LEFT");
	if((data->button&0X03)==0X02)strcat((char*)tbuf,"RIGHT");
	else if((data->button&0X03)==0X03)strcat((char*)tbuf,"+RIGHT");
	if((data->button&0X07)==0X04)strcat((char*)tbuf,"MID");
	else if((data->button&0X07)>0X04)strcat((char*)tbuf,"+MID");
	_debug_log_info("btn,X,Y,Z:0x%x,%d,%d,%d",
					data->button,(signed char)data->x,(signed char)data->y,(signed char)data->z);
}

//USB键盘数据处理
//data:USB鼠标数据结构体指针
void  USR_KEYBRD_ProcessData (uint8_t data)
{
	static u16 pos;
	static u16 maxinputchar;

	u8 buf[4];
	if(USB_FIRST_PLUGIN_FLAG)//第一次插入,将数据清零
	{
		USB_FIRST_PLUGIN_FLAG=0;
	}
	sprintf((char*)buf,"%02X",data);
	if(data>=' '&&data<='~')
	{
		tbuf[pos++]=data;
		tbuf[pos]=0;		//添加结束符. 
		if(pos>maxinputchar)pos=maxinputchar;//最大输入这么多
	}else if(data==0X0D)	//退格键
	{
		if(pos)pos--;
		tbuf[pos]=0;		//添加结束符.  
	}
	if(pos<=maxinputchar)	//没有超过显示区
	{
	}
	_debug_log_info("KEY Board Value:%02X",data);
	_debug_log_info("KEY Board Char:%c",data);
}
