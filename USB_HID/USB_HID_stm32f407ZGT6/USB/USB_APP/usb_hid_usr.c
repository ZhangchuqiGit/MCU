#include "usb_hid_usr.h"
#include "usb_hcd_int.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "usbh_hid_Logitech.h"
#include "delay.h"
#include "string.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//USBH-USR ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/7/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//*******************************************************************************
//�޸���Ϣ
//��
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʾUSB����״̬
//0,û������;
//1,�Ѿ�����;
vu8 UsbDeviceState=0;		//Ĭ��û������

/* USB HOST �û��ص����� */
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

/*  USB��Ϣ��ʾ
msgx:0,USB������
     1,USB����
     2,USB���
     3,��֧�ֵ�USB�豸 */
void USBH_Msg_Show(u8 msgx)
{
	switch(msgx)
	{
		case 1:	//USB������
		_debug_log_info("USB������")
			break;
		case 2:	//USB����
		_debug_log_info("USB����")
			break;
		case 3:	//USB���
		_debug_log_info("USB���")
			break;
		case 4:	//USB Logitech Extreme 3D ����ҡ��
		_debug_log_info("Logitech Extreme 3D ����ҡ��")
			break;
		default:	//��֧�ֵ�USB�豸`
		_debug_log_info("��֧�ֵ�USB�豸")
			break;
	}
}

u8 USB_FIRST_PLUGIN_FLAG=0;	//USB��һ�β����־,���Ϊ1,˵���ǵ�һ�β���
/************************************************************/


//USB OTG �жϷ�����
//��������USB�ж�
void OTG_FS_IRQHandler(void) // ������⵽�豸����״̬�ᱻ��λ
{
	USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}

//USB HOST ��ʼ�� 
void USBH_USR_Init(void)
{
	_debug_log_info("USB OTG FS MSC Host");
	_debug_log_info("> USB Host library started.");
	_debug_log_info("  USB Host Library v2.1.0\r\n");

}
//��⵽U�̲���
void USBH_USR_DeviceAttached(void)//U�̲���
{
	_debug_log_info("��⵽USB�豸����")
}
//��⵽U�̰γ�
void USBH_USR_DeviceDisconnected (void)//U���Ƴ�
{
	_debug_log_info("USB�豸�γ�!");
	UsbDeviceState=0;	//USB�豸�γ���
	USBH_Msg_Show(1);//USB������
}
//��λ�ӻ�
void USBH_USR_ResetDevice(void)
{
	_debug_log_info("��λUSB�豸");
}
//��⵽�ӻ��ٶ�
//DeviceSpeed:�ӻ��ٶ�(0,1,2 / ����)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
		_debug_log_info("����(HS)USB�豸!");
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
		_debug_log_info("ȫ��(FS)USB�豸!");
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
		_debug_log_info("����(LS)USB�豸!");
	}
	else
	{
		_debug_log_info("�豸����!");
	}
}

//��⵽�ӻ���������
//DeviceDesc:�豸������ָ��
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
	USBH_DevDesc_TypeDef *hs = DeviceDesc;

	/** ��׼���豸������ **/
	printf("\r\n��׼���豸������\tstruct _DeviceDescriptor{}\r\n");

	printf("�豸���������ֽ���\t0x%x\tuint8_t bLength\r\n", hs->bLength);
	printf("�豸���������ͱ��\t0x%x\tuint8_t bDescriptorType\r\n", hs->bDescriptorType);
	printf("USB�汾��\t0x%x\tuint16_t bcdUSB\r\n", hs->bcdUSB);
	printf("USB������豸��\t0x%x\tuint8_t bDeviceClass\r\n", hs->bDeviceClass);
	printf("USB������豸����\t0x%x\tuint8_t bDeviceSubClass\r\n", hs->bDeviceSubClass);
	printf("USB������豸Э�����\t0x%x\tuint8_t bDeviceProtocol\r\n", hs->bDeviceProtocol);
	printf("�˵��������С\t0x%x\tuint8_t bMaxPacketSize\r\n", hs->bMaxPacketSize);
	printf("���̱�ţ���USB Org���䣩\t0x%x\tuint16_t idVendor\r\n", hs->idVendor);
	printf("��Ʒ��ţ��������̷��䣩\t0x%x\tuint16_t idProduct\r\n", hs->idProduct);
	printf("�豸�������\t0x%x\tuint16_t bcdDevice\r\n", hs->bcdDevice);
	printf("�豸�����ַ�������\t0x%x\tuint8_t iManufacturer\r\n", hs->iManufacturer);
	printf("��Ʒ�ַ�������\t0x%x\tuint8_t iProduct\r\n", hs->iProduct);
	printf("�豸���к�����\t0x%x\tuint8_t �豸���к�����\r\n", hs->iSerialNumber);
	printf("���ܵ���������\t0x%x\tuint8_t bNumConfigurations\r\n\r\n", hs->bNumConfigurations);
}

//�ӻ���ַ����ɹ�
void USBH_USR_DeviceAddressAssigned(void)
{
	_debug_log_info("�ӻ���ַ����ɹ�!");
}

//��������������Ч
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,// ����������
										  USBH_InterfaceDesc_TypeDef *itfDesc,// �ӿ�������
										  USBH_EpDesc_TypeDef *epDesc)// �˵�������
{
	if((*itfDesc).bInterfaceClass==0x08)//�ӿ���
	{
		_debug_log_info("���ƶ��洢���豸!");
	}
	else if((*itfDesc).bInterfaceClass==0x03)
	{
		_debug_log_info("HID �豸!");
	}
	else _debug_log_info("���� �豸: %x", (*itfDesc).bInterfaceClass);

	/** ���� ������ **/
	printf("\r\n���� ������\tstruct _ConfigurationDescriptor{}\r\n");

	printf("�豸���������ֽ���\t0x%x\tuint8_t bLength\r\n", cfgDesc->bLength);
	printf("�������������ͱ��\t0x%x\tuint8_t bDescriptorType\r\n", cfgDesc->bDescriptorType);
	printf("�����÷��ص��������ݴ�С\t0x%x\tuint16_t wTotalLength\r\n", cfgDesc->wTotalLength);
	printf("������֧�ֵĽӿ�����\t0x%x\tuint8_t bNumInterfaces\r\n", cfgDesc->bNumInterfaces);
	printf("��������Ҫ�Ĳ���\t0x%x\tuint8_t bConfigurationValue\r\n", cfgDesc->bConfigurationValue);
	printf("���������õ��ַ�������\t0x%x\tuint8_t iConfiguration\r\n", cfgDesc->iConfiguration);
	printf("����ģʽ��ѡ��\t0x%x\tuint8_t bmAttributes\r\n", cfgDesc->bmAttributes);
	printf("�豸�����߻�ȡ��������\t0x%x\tuint8_t bMaxPower\r\n", cfgDesc->bMaxPower);

	/** �ӿ� ������ **/
	printf("\r\n�ӿ� ������\tstruct _InterfaceDescriptor{}\r\n");

	printf("�ӿ����������ֽ���\t0x%x\tuint8_t bLength\r\n", itfDesc->bLength);
	printf("�ӿ������������ͱ��\t0x%x\tuint8_t bDescriptorType\r\n", itfDesc->bDescriptorType);
	printf("�ýӿڵı��\t0x%x\tuint8_t wTotalLength\r\n", itfDesc->bInterfaceNumber);
	printf("���õĽӿ��������ı��\t0x%x\tuint8_t bNumInterfaces\r\n", itfDesc->bAlternateSetting);
	printf("�ýӿ�ʹ�� �Ķ˵������������˵�0\t0x%x\tuint8_t bConfigurationValue\r\n", itfDesc->bNumEndpoints);
	printf("�ӿ���\t0x%x\tuint8_t iConfiguration\r\n", itfDesc->bInterfaceClass);
	printf("�ӿ�����\t0x%x\tuint8_t bmAttributes\r\n", itfDesc->bInterfaceSubClass);
	printf("�ӿ���Э��\t0x%x\tuint8_t bMaxPower\r\n", itfDesc->bInterfaceProtocol);
	printf("�����ýӿڵ��ַ�������ֵ\t0x%x\tuint8_t bMaxPower\r\n", itfDesc->iInterface);

	/** �˵� ������ **/
	printf("\r\n�˵� ������\tstruct _EndpointDescriptor{}\r\n");

	printf("�˵��������ֽ���\t0x%x\tuint8_t bLength\r\n", epDesc->bLength);
	printf("�˵����������ͱ��\t0x%x\tuint8_t bDescriptorType\r\n", epDesc->bDescriptorType);
	printf("�˵��ַ�������������\t0x%x\tuint8_t wTotalLength\r\n", epDesc->bEndpointAddress);
	printf("�˵�Ĵ�������\t0x%x\tuint8_t bNumInterfaces\r\n", epDesc->bmAttributes);
	printf("�˵��շ���������С\t0x%x\tuint16_t iConfiguration\r\n", epDesc->wMaxPacketSize);
	printf("������ѯ�˵��ʱ����\t0x%x\tuint8_t bmAttributes\r\n", epDesc->bInterval);
}

//��ȡ �������ַ���������
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	_debug_log_info("�������ַ��������� Manufacturer: %s",(char *)ManufacturerString);
}

//��ȡ ��Ʒ/�豸 ��
void USBH_USR_Product_String(void *ProductString)
{
	_debug_log_info("Product: %s",(char *)ProductString);
}

//��ȡ���豸SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
	_debug_log_info("Serial Number: %s",(char *)SerialNumString);
}

//�豸USBö�����
void USBH_USR_EnumerationDone(void)
{
	_debug_log_info("�豸ö�����!\r\n");
}

//�޷�ʶ���USB�豸
void USBH_USR_DeviceNotSupported(void)
{
	USBH_Msg_Show(0);//�޷�ʶ���USB�豸
	//_debug_log_info("�޷�ʶ���USB�豸!\r\n");    
}

//�ȴ��û����밴��,ִ����һ������
USBH_USR_Status USBH_USR_UserInput(void)
{
	_debug_log_info("�����û�ȷ�ϲ���!");
	UsbDeviceState=1;//USB�豸�Ѿ����ӳɹ�
	return USBH_USR_RESP_OK;
}

//USB�ӿڵ�������
void USBH_USR_OverCurrentDetected (void)
{
	_debug_log_info("�˿ڵ�������!!!");
}

//���³�ʼ��
void USBH_USR_DeInit(void)
{
	_debug_log_info("���³�ʼ��!!!");
}

//�޷��ָ��Ĵ���!!  
void USBH_USR_UnrecoveredError (void)
{
	_debug_log_info("�޷��ָ��Ĵ���!!!\r\n");
}

//////////////////////////////////////////////////////////////////////////////////////////
//������������,ΪALIENTEK���,�Է�ֹUSB����

//USB HIDͨ���������,��ֹUSBͨ������(��ʱ�����:DTERR,��Data toggle error)
//pcore:USB_OTG_Core_dev_HANDLE�ṹ��ָ��
//phidm:HID_Machine_TypeDef�ṹ��ָ��
//����ֵ:0,û������
//       1,������,�ⲿ������������USB����.
u8 USBH_Check_HIDCommDead(USB_OTG_CORE_HANDLE *pcore,HID_Machine_TypeDef *phidm)
{
	if(pcore->host.HC_Status[phidm->hc_num_in]==HC_DATATGLERR)//��⵽DTERR����,ֱ������USB.
	{
		return 1;
	}
	return 0;
}

//USBö��״̬�������,��ֹUSBö��ʧ�ܵ��µ�����
//phost:USB_HOST�ṹ��ָ��
//����ֵ:0,û������
//       1,������,�ⲿ������������USB����.
u8 USBH_Check_EnumeDead(USBH_HOST *phost)
{
	static u16 errcnt=0;
	//���״̬,�����������,��˵��USB������.
	if(phost->gState==HOST_CTRL_XFER &&
	   (phost->EnumState==ENUM_IDLE || phost->EnumState==ENUM_GET_FULL_DEV_DESC))
	{
		errcnt++;
		if(errcnt>2000)//������
		{
			errcnt=0;
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS ��λ
			delay_ms(5);
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//��λ����  
			return 1;
		}
	}else errcnt=0;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
/* Logitech Extreme 3D ����ҡ�� ��ʼ�� */
void  USR_Logitech_Init (void) // ���� Logitech Extreme 3D ����ҡ��
{
	USBH_Msg_Show(4);		//USB Logitech Extreme 3D ����ҡ��
	USB_FIRST_PLUGIN_FLAG=1;//��ǵ�һ�β���
}

//����ʼ��
void USR_MOUSE_Init	(void)
{
	USBH_Msg_Show(3);		//USB ���
	USB_FIRST_PLUGIN_FLAG=1;//��ǵ�һ�β���
}

//���̳�ʼ��
void  USR_KEYBRD_Init(void)
{
	USBH_Msg_Show(2);		//USB ����
	USB_FIRST_PLUGIN_FLAG=1;//��ǵ�һ�β���
}

/* Logitech Extreme 3D ����ҡ�� ���ݴ��� */
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

//��ʱ����,���ڴ���������/������������(4.3��,����������2016�ֽ�)
__align(4) u8 tbuf[2017];

//USB������ݴ���
//data:USB������ݽṹ��ָ��
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{
	static signed short x,y,z;
	if(USB_FIRST_PLUGIN_FLAG)//��һ�β���,����������
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

//USB�������ݴ���
//data:USB������ݽṹ��ָ��
void  USR_KEYBRD_ProcessData (uint8_t data)
{
	static u16 pos;
	static u16 maxinputchar;

	u8 buf[4];
	if(USB_FIRST_PLUGIN_FLAG)//��һ�β���,����������
	{
		USB_FIRST_PLUGIN_FLAG=0;
	}
	sprintf((char*)buf,"%02X",data);
	if(data>=' '&&data<='~')
	{
		tbuf[pos++]=data;
		tbuf[pos]=0;		//��ӽ�����. 
		if(pos>maxinputchar)pos=maxinputchar;//���������ô��
	}else if(data==0X0D)	//�˸��
	{
		if(pos)pos--;
		tbuf[pos]=0;		//��ӽ�����.  
	}
	if(pos<=maxinputchar)	//û�г�����ʾ��
	{
	}
	_debug_log_info("KEY Board Value:%02X",data);
	_debug_log_info("KEY Board Char:%c",data);
}
