#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "usb_hid_usr.h"
#include "string.h"

#include "GPIO_JTAG.h"
#include "debug.h"

//USB������ ʵ�� -�⺯���汾
/**�޼� ��Dģʽ��Xģʽ��Dģʽ����ͨģʽ��ÿ֡���ݴ���8���ֽڣ�Xģʽ��xbox�ֱ�ģʽ��
 * �и������Ϣ��ÿ֡����15���ֽڣ���Ϊ�����������ݣ���*/

USBH_HOST  USB_Host; // �������Ͷ���
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;

/* ������ ./USB/STM32_USB_HOST_Library/Class/HID/src/usbh_hid_core.c */
extern HID_Machine_TypeDef HID_Machine; // HID ����

/*
������ʱ������ʾһЩ��ʾ��Ϣ�� Ȼ���ʼ�� USB HOST����������ѯ��
����⵽ USB ���/���̵Ĳ������ʾ�豸���ͣ�����ʾ�豸�������ݣ�

����� USB ��꣺����ʾ����ƶ������꣨X�� Y ���꣩�����ֹ�����ֵ��Z ���꣩�Լ�
�����������ң���

����� USB ���̣�����ʾ�������������/��ĸ�����ݣ��������а�����֧�֣����ְ���
û��������֧�֣����� F1~F12����

��󣬻����� DS0 ��ʾ�����������С� */

/*HID ��������*/
void USBH_HID_Reconnect(void);

/*USB������
���³�ʼ��!!! 
��⵽USB�豸����! 
��λ�豸...
����(LS)USB�豸!*/
int main(void)
{
	u32 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ��  
//	delay_ms(1000);
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
//	JTAG_SWD_GPIO_Config();
	LED_Init();					//��ʼ��LED

	//USB OTG �ײ�IO��ʼ��
	//pdev:USB OTG�ں˽ṹ��ָ��
	USB_OTG_BSP_Init(&USB_OTG_Core_dev);

	//USB����Ӳ���Ͷ�ջ��ʼ��
	USBH_Init( &USB_OTG_Core_dev, USB_OTG_FS_CORE_ID/*ʹ���ں˵�ȫ��ģʽ*/,
			   &USB_Host, &HID_cb/*��ص��ṹ��ַ*/, &USR_Callbacks/*�û��ص�����*/);

	while(1)
	{
		t++;
		if(t % 300000 == 0) {
			LED0=!LED0;
		}
		
		USBH_Process(&USB_OTG_Core_dev, &USB_Host); // ������ѯ���
		if(UsbDeviceState == 1)//���ӽ�����
		{
			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//���USB HIDͨ��,�Ƿ�����?
			{
				_debug_log_info("USB HID �쳣������")
				USBH_HID_Reconnect();//����
			}
		}
		else	//����δ������ʱ��,���
		{
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
	USBH_Init( &USB_OTG_Core_dev, USB_OTG_FS_CORE_ID/*�ӻ�ģʽ*/,
			   &USB_Host, &HID_cb, &USR_Callbacks);
}

