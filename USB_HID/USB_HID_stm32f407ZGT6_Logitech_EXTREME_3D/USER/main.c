#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usb_hid_usr.h"
#include "string.h"

//#include "GPIO_JTAG.h"
#include "debug.h"
#include "timer_delay.h"
#include "serial_bus.h"
#include "timer.h"

/**罗技 有D模式和X模式，D模式是普通模式，每帧数据传输8个字节；X模式是xbox手柄模式，
 * 有更多的信息，每帧传输15个字节（因为有陀螺仪数据）。*/
/*
开机的时候先显示一些提示信息， 然后初始化 USB HOST，并不断轮询。
当检测到 USB 鼠标/键盘的插入后，显示设备类型，并显示设备输入数据，

如果是 USB 鼠标：将显示鼠标移动的坐标（X， Y 坐标），滚轮滚动数值（Z 坐标）以及
按键（左中右）。

如果是 USB 键盘：将显示键盘输入的数字/字母等内容（不是所有按键都支持，部分按键
没有做解码支持，比如 F1~F12）。 */

USBH_HOST  USB_Host; // 主机类型定义
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;

/* 定义在 ./USB/STM32_USB_HOST_Library/Class/HID/src/usbh_hid_core.c */
extern HID_Machine_TypeDef HID_Machine; // HID 流程

/*HID 重新连接*/
void USBH_HID_Reconnect(void);

u8  serval = 0;

/*USB无连接
重新初始化!!! 
检测到USB设备插入! 
复位设备...
低速(LS)USB设备!*/
int main(void)
{
	u32 t;
	u8 led_power = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化  
//	delay_ms(1000);
//	JTAG_SWD_GPIO_Config();
	
#if  0
	uart_init(115200); //串口初始化波特率为115200
#else
	USART1_SBUS_Init();
#endif
	LED_Init();					//初始化LED
	timer_delay_init();
// 	TIM3_Int_Init(1400-1,840-1);
	
	//USB主机硬件和堆栈初始化
	USBH_Init(&USB_OTG_Core_dev, USB_OTG_FS_CORE_ID/*使能内核的全速模式*/,
			  &USB_Host, &CLASS_cb/*类回调结构地址*/, &USR_cb/*用户回调结构地址*/);
	
	while(1)
	{
		USBH_Process(&USB_OTG_Core_dev, &USB_Host); // 不断轮询检测
		if(UsbDeviceState == 1)//连接建立了
		{
			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//检测USB HID通信,是否还正常?
			{
				_debug_log_info("USB HID 异常，重连")
//				TIM_Cmd(TIM3,DISABLE); 	
				memset(&Logitech_Data, 0, sizeof(Logitech_Data));
				USR_Logitech_ProcessData (&Logitech_Data);
				
				USBH_HID_Reconnect();//重连				
			}
			t++;
			if(t % 150000 == 0) {  
				led_power = !led_power;
				LED0=!led_power;
				LED1=led_power;
			}
			serval = 1;
		}
		else	//连接未建立的时候,检测
		{
			if (serval != 0) {
				serval = 0;
				//TIM_Cmd(TIM3,DISABLE); 	
				memset(&Logitech_Data, 0, sizeof(Logitech_Data));
				USR_Logitech_ProcessData (&Logitech_Data);
			}
			LED0=LED1=1;
			t = 0;
			
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
	USBH_Init(&USB_OTG_Core_dev, USB_OTG_FS_CORE_ID/*从机模式*/,
			  &USB_Host, &CLASS_cb, &USR_cb);
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET) //溢出中断
	{
		if(serval == 1)//连接建立了
		{
#if 0		
			_debug_log_info_c("X: %d\tY: %d\t", Logitech_Data.X, Logitech_Data.Y);
			_debug_log_info_c("RZ: %d\t", Logitech_Data.RZ);
			_debug_log_info_c("Slider: %d\t", Logitech_Data.Slider);
			_debug_log_info_c("Hat_Switch: %d\t", Logitech_Data.Hat_Switch);
			_debug_log_info_c("button: ");
			for (uint16_t i=0x0001, j=1; i & 0x0fff ; i = i<<1, ++j) {
				if (Logitech_Data.button & i) _debug_log_info_c("%d ", j);
			}
			_debug_log_info_c("\r\n") 
#else		
			sbus_out(&Logitech_Data);
#endif
		}
	}	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
