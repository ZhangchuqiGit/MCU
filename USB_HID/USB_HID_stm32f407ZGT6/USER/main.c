#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usb_hid_usr.h"
#include "string.h"

#include "GPIO_JTAG.h"
#include "debug.h"

//USB鼠标键盘 实验 -库函数版本
/**罗技 有D模式和X模式，D模式是普通模式，每帧数据传输8个字节；X模式是xbox手柄模式，
 * 有更多的信息，每帧传输15个字节（因为有陀螺仪数据）。*/

USBH_HOST  USB_Host; // 主机类型定义
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;

/* 定义在 ./USB/STM32_USB_HOST_Library/Class/HID/src/usbh_hid_core.c */
extern HID_Machine_TypeDef HID_Machine; // HID 流程

/*
开机的时候先显示一些提示信息， 然后初始化 USB HOST，并不断轮询。
当检测到 USB 鼠标/键盘的插入后，显示设备类型，并显示设备输入数据，

如果是 USB 鼠标：将显示鼠标移动的坐标（X， Y 坐标），滚轮滚动数值（Z 坐标）以及
按键（左中右）。

如果是 USB 键盘：将显示键盘输入的数字/字母等内容（不是所有按键都支持，部分按键
没有做解码支持，比如 F1~F12）。

最后，还是用 DS0 提示程序正在运行。 */

/*HID 重新连接*/
void USBH_HID_Reconnect(void);

/*USB无连接
重新初始化!!! 
检测到USB设备插入! 
复位设备...
低速(LS)USB设备!*/
int main(void)
{
	u32 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化  
//	delay_ms(1000);
	uart_init(115200);	//串口初始化波特率为115200
//	JTAG_SWD_GPIO_Config();
	LED_Init();					//初始化LED

	//USB OTG 底层IO初始化
	//pdev:USB OTG内核结构体指针
	USB_OTG_BSP_Init(&USB_OTG_Core_dev);

	//USB主机硬件和堆栈初始化
	USBH_Init( &USB_OTG_Core_dev, USB_OTG_FS_CORE_ID/*使能内核的全速模式*/,
			   &USB_Host, &HID_cb/*类回调结构地址*/, &USR_Callbacks/*用户回调函数*/);

	while(1)
	{
		t++;
		if(t % 300000 == 0) {
			LED0=!LED0;
		}
		
		USBH_Process(&USB_OTG_Core_dev, &USB_Host); // 不断轮询检测
		if(UsbDeviceState == 1)//连接建立了
		{
			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//检测USB HID通信,是否还正常?
			{
				_debug_log_info("USB HID 异常，重连")
				USBH_HID_Reconnect();//重连
			}
		}
		else	//连接未建立的时候,检测
		{
			if(USBH_Check_EnumeDead(&USB_Host))	//检测USB HOST 枚举是否死机了?死机了,则重新初始化 
			{
				_debug_log_info("枚举死机，重连")
				USBH_HID_Reconnect();//重连
			}
		}
	}
}

/*HID 重新连接*/
void USBH_HID_Reconnect(void)
{
	_debug_log_info("HID重新连接")

	/* 关闭之前的连接 */
	USBH_DeInit(&USB_OTG_Core_dev,&USB_Host);	//复位 USB HOST
	USB_OTG_StopHost(&USB_OTG_Core_dev);		//停止USBhost
	if(USB_Host.usr_cb->DeviceDisconnected)		//存在,才禁止
	{
		USB_Host.usr_cb->DeviceDisconnected(); 	//关闭USB连接
		USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);
		USB_Host.usr_cb->DeInit();//重新初始化
		USB_Host.class_cb->DeInit(&USB_OTG_Core_dev,&USB_Host.device_prop);
	}
	USB_OTG_DisableGlobalInt(&USB_OTG_Core_dev);//关闭所有中断

	/* 重新复位USB */
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS 复位
	delay_ms(5);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//复位结束

	memset(&USB_OTG_Core_dev,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));

	//重新 USB主机硬件和堆栈初始化
	USBH_Init( &USB_OTG_Core_dev, USB_OTG_FS_CORE_ID/*从机模式*/,
			   &USB_Host, &HID_cb, &USR_Callbacks);
}

