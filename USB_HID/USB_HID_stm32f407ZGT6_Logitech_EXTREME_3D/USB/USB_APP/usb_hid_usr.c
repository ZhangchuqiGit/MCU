#include "usb_hid_usr.h"

#include "usb_hcd_int.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "usbh_hid_Logitech.h"

#include "string.h"
#include "serial_bus.h"

extern USB_OTG_CORE_HANDLE USB_OTG_Core_dev;
u8 USB_FIRST_PLUGIN_FLAG=0;	//USB第一次插入标志,如果为1,说明是第一次插入

//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 UsbDeviceState=0;		//默认没有连接

/* USB HOST 用户回调函数 */
USBH_Usr_cb_TypeDef USR_cb = {
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


/*  USB信息显示
msgx:0,USB无连接
     1,USB键盘
     2,USB鼠标
     3,不支持的USB设备 */
void USBH_Msg_Show(u8 msgx)
{
#ifndef NO_Debug
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
#endif
}

//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void) // 主机检测到设备插入状态会被置位
{
	USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}

//USB HOST 初始化 
void USBH_USR_Init(void)
{
#ifndef NO_Debug
	_debug_log_info("\r\nUSB OTG FS MSC Host");
	_debug_log_info("USB Host library started.");
	_debug_log_info("USB Host Library v2.1.0\r\n");
#endif
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
	USBH_Msg_Show(1);//USB无连接
	UsbDeviceState=0;	//USB设备拔出了
//	TIM_Cmd(TIM3,DISABLE); 	
}

//复位从机
void USBH_USR_ResetDevice(void)
{
#ifndef NO_Debug
	_debug_log_info("复位USB设备");
#endif
}

//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
#ifndef NO_Debug
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
#endif
}

//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
#ifndef NO_Debug
	USBH_DevDesc_TypeDef *hs = DeviceDesc;

	/** 标准的设备描述符 **/
	_debug_log_info("\r\n标准的设备描述符\tstruct _DeviceDescriptor{}\r\n");

	_debug_log_info("设备描述符的字节数\t0x%x\tuint8_t bLength", hs->bLength);
	_debug_log_info("设备描述符类型编号\t0x%x\tuint8_t bDescriptorType", hs->bDescriptorType);
	_debug_log_info("USB版本号\t0x%x\tuint16_t bcdUSB", hs->bcdUSB);
	_debug_log_info("USB分配的设备类\t0x%x\tuint8_t bDeviceClass", hs->bDeviceClass);
	_debug_log_info("USB分配的设备子类\t0x%x\tuint8_t bDeviceSubClass", hs->bDeviceSubClass);
	_debug_log_info("USB分配的设备协议代码\t0x%x\tuint8_t bDeviceProtocol", hs->bDeviceProtocol);
	_debug_log_info("端点的最大包大小\t0x%x\tuint8_t bMaxPacketSize", hs->bMaxPacketSize);
	_debug_log_info("厂商编号（由USB Org分配）\t0x%x\tuint16_t idVendor", hs->idVendor);
	_debug_log_info("产品编号（由制造商分配）\t0x%x\tuint16_t idProduct", hs->idProduct);
	_debug_log_info("设备出厂编号\t0x%x\tuint16_t bcdDevice", hs->bcdDevice);
	_debug_log_info("设备厂商字符串索引\t0x%x\tuint8_t iManufacturer", hs->iManufacturer);
	_debug_log_info("产品字符串索引\t0x%x\tuint8_t iProduct", hs->iProduct);
	_debug_log_info("设备序列号索引\t0x%x\tuint8_t 设备序列号索引", hs->iSerialNumber);
	_debug_log_info("可能的配置数量\t0x%x\tuint8_t bNumConfigurations", hs->bNumConfigurations);
#endif
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
#ifndef NO_Debug
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
	_debug_log_info("\r\n配置 描述符\tstruct _ConfigurationDescriptor{}\r\n");

	_debug_log_info("设备描述符的字节数\t0x%x\tuint8_t bLength", cfgDesc->bLength);
	_debug_log_info("配置描述符类型编号\t0x%x\tuint8_t bDescriptorType", cfgDesc->bDescriptorType);
	_debug_log_info("此配置返回的所有数据大小\t0x%x\tuint16_t wTotalLength", cfgDesc->wTotalLength);
	_debug_log_info("此配置支持的接口数量\t0x%x\tuint8_t bNumInterfaces", cfgDesc->bNumInterfaces);
	_debug_log_info("命令所需要的参数\t0x%x\tuint8_t bConfigurationValue", cfgDesc->bConfigurationValue);
	_debug_log_info("描述该配置的字符串索引\t0x%x\tuint8_t iConfiguration", cfgDesc->iConfiguration);
	_debug_log_info("供电模式的选择\t0x%x\tuint8_t bmAttributes", cfgDesc->bmAttributes);
	_debug_log_info("设备从总线获取的最大电流\t0x%x\tuint8_t bMaxPower", cfgDesc->bMaxPower);

	/** 接口 描述符 **/
	_debug_log_info("\r\n接口 描述符\tstruct _InterfaceDescriptor{}\r\n");

	_debug_log_info("接口描述符的字节数\t0x%x\tuint8_t bLength", itfDesc->bLength);
	_debug_log_info("接口描述符的类型编号\t0x%x\tuint8_t bDescriptorType", itfDesc->bDescriptorType);
	_debug_log_info("该接口的编号\t0x%x\tuint8_t wTotalLength", itfDesc->bInterfaceNumber);
	_debug_log_info("备用的接口描述符的编号\t0x%x\tuint8_t bNumInterfaces", itfDesc->bAlternateSetting);
	_debug_log_info("该接口使用 的端点数，不包括端点0\t0x%x\tuint8_t bConfigurationValue", itfDesc->bNumEndpoints);
	_debug_log_info("接口类\t0x%x\tuint8_t iConfiguration", itfDesc->bInterfaceClass);
	_debug_log_info("接口子类\t0x%x\tuint8_t bmAttributes", itfDesc->bInterfaceSubClass);
	_debug_log_info("接口类协议\t0x%x\tuint8_t bMaxPower", itfDesc->bInterfaceProtocol);
	_debug_log_info("描述该接口的字符串索引值\t0x%x\tuint8_t bMaxPower", itfDesc->iInterface);

	/** 端点 描述符 **/
	_debug_log_info("\r\n端点 描述符\tstruct _EndpointDescriptor{}\r\n");

	_debug_log_info("端点描述符字节数\t0x%x\tuint8_t bLength", epDesc->bLength);
	_debug_log_info("端点描述符类型编号\t0x%x\tuint8_t bDescriptorType", epDesc->bDescriptorType);
	_debug_log_info("端点地址及输入输出类型\t0x%x\tuint8_t wTotalLength", epDesc->bEndpointAddress);
	_debug_log_info("端点的传输类型\t0x%x\tuint8_t bNumInterfaces", epDesc->bmAttributes);
	_debug_log_info("端点收发的最大包大小\t0x%x\tuint16_t iConfiguration", epDesc->wMaxPacketSize);
	_debug_log_info("主机查询端点的时间间隔\t0x%x\tuint8_t bmAttributes", epDesc->bInterval);
#endif
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
	_debug_log_info("设备枚举完成!");
}

//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{
	USBH_Msg_Show(0);//无法识别的USB设备
	//_debug_log_info("无法识别的USB设备!");
}

//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{
	_debug_log_info("跳过用户确认步骤!");
	UsbDeviceState=1;//USB设备已经连接成功
//	TIM_Cmd(TIM3, ENABLE); 	
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
	_debug_log_info("无法恢复的错误!!!");
}

/* Logitech Extreme 3D 飞行摇杆 初始化 */
void  USR_Logitech_Init (void) // 适配 Logitech Extreme 3D 飞行摇杆
{
	USBH_Msg_Show(4);		//USB Logitech Extreme 3D 飞行摇杆
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
void  USR_Logitech_ProcessData (HID_Logitech_Data_Analyze *Logitech_data)
{
#ifndef NO_Debug	
	_debug_log_info_c("X: %d\tY: %d\t", Logitech_data->X, Logitech_data->Y);
	_debug_log_info_c("RZ: %d\t", Logitech_data->RZ);
	_debug_log_info_c("Slider: %d\t", Logitech_data->Slider);
	_debug_log_info_c("Hat_Switch: %d\t", Logitech_data->Hat_Switch);
	_debug_log_info_c("button: ");
	for (uint16_t i=0x0001, j=1; i & 0x0fff ; i = i<<1, ++j) {
		if (Logitech_data->button & i) _debug_log_info_c("%d ", j);
	}
	_debug_log_info_c("\r\n");
#else
	sbus_out(Logitech_data);
#endif
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
		if(errcnt>3000)//死机了
		{
			errcnt=0;
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS 复位
			USB_OTG_BSP_mDelay(4);
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//复位结束
			return 1;
		}
	} else errcnt=0;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
