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

/**�޼� ��Dģʽ��Xģʽ��Dģʽ����ͨģʽ��ÿ֡���ݴ���8���ֽڣ�Xģʽ��xbox�ֱ�ģʽ��
 * �и������Ϣ��ÿ֡����15���ֽڣ���Ϊ�����������ݣ���*/
/*
������ʱ������ʾһЩ��ʾ��Ϣ�� Ȼ���ʼ�� USB HOST����������ѯ��
����⵽ USB ���/���̵Ĳ������ʾ�豸���ͣ�����ʾ�豸�������ݣ�

����� USB ��꣺����ʾ����ƶ������꣨X�� Y ���꣩�����ֹ�����ֵ��Z ���꣩�Լ�
�����������ң���

����� USB ���̣�����ʾ�������������/��ĸ�����ݣ��������а�����֧�֣����ְ���
û��������֧�֣����� F1~F12���� */

USBH_HOST  USB_Host; // �������Ͷ���
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;

/* ������ ./USB/STM32_USB_HOST_Library/Class/HID/src/usbh_hid_core.c */
extern HID_Machine_TypeDef HID_Machine; // HID ����

/*HID ��������*/
void USBH_HID_Reconnect(void);

u8  serval = 0;

/*USB������
���³�ʼ��!!! 
��⵽USB�豸����! 
��λ�豸...
����(LS)USB�豸!*/
int main(void)
{
	u32 t;
	u8 led_power = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ��  
//	delay_ms(1000);
//	JTAG_SWD_GPIO_Config();
	
#if  0
	uart_init(115200); //���ڳ�ʼ��������Ϊ115200
#else
	USART1_SBUS_Init();
#endif
	LED_Init();					//��ʼ��LED
	timer_delay_init();
// 	TIM3_Int_Init(1400-1,840-1);
	
	//USB����Ӳ���Ͷ�ջ��ʼ��
	USBH_Init(&USB_OTG_Core_dev, USB_OTG_FS_CORE_ID/*ʹ���ں˵�ȫ��ģʽ*/,
			  &USB_Host, &CLASS_cb/*��ص��ṹ��ַ*/, &USR_cb/*�û��ص��ṹ��ַ*/);
	
	while(1)
	{
		USBH_Process(&USB_OTG_Core_dev, &USB_Host); // ������ѯ���
		if(UsbDeviceState == 1)//���ӽ�����
		{
			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//���USB HIDͨ��,�Ƿ�����?
			{
				_debug_log_info("USB HID �쳣������")
//				TIM_Cmd(TIM3,DISABLE); 	
				memset(&Logitech_Data, 0, sizeof(Logitech_Data));
				USR_Logitech_ProcessData (&Logitech_Data);
				
				USBH_HID_Reconnect();//����				
			}
			t++;
			if(t % 150000 == 0) {  
				led_power = !led_power;
				LED0=!led_power;
				LED1=led_power;
			}
			serval = 1;
		}
		else	//����δ������ʱ��,���
		{
			if (serval != 0) {
				serval = 0;
				//TIM_Cmd(TIM3,DISABLE); 	
				memset(&Logitech_Data, 0, sizeof(Logitech_Data));
				USR_Logitech_ProcessData (&Logitech_Data);
			}
			LED0=LED1=1;
			t = 0;
			
			if(USBH_Check_EnumeDead(&USB_Host))	//���USB HOST ö���Ƿ�������?������,�����³�ʼ�� 
			{
				_debug_log_info("ö������������")
				USBH_HID_Reconnect();//����
			}
		}
	}
}
			
/*HID ��������*/
void USBH_HID_Reconnect(void)
{
	_debug_log_info("HID��������")

	/* �ر�֮ǰ������ */
	USBH_DeInit(&USB_OTG_Core_dev,&USB_Host);	//��λ USB HOST
	USB_OTG_StopHost(&USB_OTG_Core_dev);		//ֹͣUSBhost
	if(USB_Host.usr_cb->DeviceDisconnected)		//����,�Ž�ֹ
	{
		USB_Host.usr_cb->DeviceDisconnected(); 	//�ر�USB����
		USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);
		USB_Host.usr_cb->DeInit();//���³�ʼ��
		USB_Host.class_cb->DeInit(&USB_OTG_Core_dev,&USB_Host.device_prop);
	}
	USB_OTG_DisableGlobalInt(&USB_OTG_Core_dev);//�ر������ж�

	/* ���¸�λUSB */
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS ��λ
	delay_ms(5);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//��λ����

	memset(&USB_OTG_Core_dev,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));

	//���� USB����Ӳ���Ͷ�ջ��ʼ��
	USBH_Init(&USB_OTG_Core_dev, USB_OTG_FS_CORE_ID/*�ӻ�ģʽ*/,
			  &USB_Host, &CLASS_cb, &USR_cb);
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)==SET) //����ж�
	{
		if(serval == 1)//���ӽ�����
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}
