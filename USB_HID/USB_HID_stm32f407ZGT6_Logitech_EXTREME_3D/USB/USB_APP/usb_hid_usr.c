#include "usb_hid_usr.h"

#include "usb_hcd_int.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"
#include "usbh_hid_Logitech.h"

#include "string.h"
#include "serial_bus.h"

extern USB_OTG_CORE_HANDLE USB_OTG_Core_dev;
u8 USB_FIRST_PLUGIN_FLAG=0;	//USB��һ�β����־,���Ϊ1,˵���ǵ�һ�β���

//��ʾUSB����״̬
//0,û������;
//1,�Ѿ�����;
vu8 UsbDeviceState=0;		//Ĭ��û������

/* USB HOST �û��ص����� */
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


/*  USB��Ϣ��ʾ
msgx:0,USB������
     1,USB����
     2,USB���
     3,��֧�ֵ�USB�豸 */
void USBH_Msg_Show(u8 msgx)
{
#ifndef NO_Debug
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
#endif
}

//USB OTG �жϷ�����
//��������USB�ж�
void OTG_FS_IRQHandler(void) // ������⵽�豸����״̬�ᱻ��λ
{
	USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}

//USB HOST ��ʼ�� 
void USBH_USR_Init(void)
{
#ifndef NO_Debug
	_debug_log_info("\r\nUSB OTG FS MSC Host");
	_debug_log_info("USB Host library started.");
	_debug_log_info("USB Host Library v2.1.0\r\n");
#endif
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
	USBH_Msg_Show(1);//USB������
	UsbDeviceState=0;	//USB�豸�γ���
//	TIM_Cmd(TIM3,DISABLE); 	
}

//��λ�ӻ�
void USBH_USR_ResetDevice(void)
{
#ifndef NO_Debug
	_debug_log_info("��λUSB�豸");
#endif
}

//��⵽�ӻ��ٶ�
//DeviceSpeed:�ӻ��ٶ�(0,1,2 / ����)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
#ifndef NO_Debug
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
#endif
}

//��⵽�ӻ���������
//DeviceDesc:�豸������ָ��
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
#ifndef NO_Debug
	USBH_DevDesc_TypeDef *hs = DeviceDesc;

	/** ��׼���豸������ **/
	_debug_log_info("\r\n��׼���豸������\tstruct _DeviceDescriptor{}\r\n");

	_debug_log_info("�豸���������ֽ���\t0x%x\tuint8_t bLength", hs->bLength);
	_debug_log_info("�豸���������ͱ��\t0x%x\tuint8_t bDescriptorType", hs->bDescriptorType);
	_debug_log_info("USB�汾��\t0x%x\tuint16_t bcdUSB", hs->bcdUSB);
	_debug_log_info("USB������豸��\t0x%x\tuint8_t bDeviceClass", hs->bDeviceClass);
	_debug_log_info("USB������豸����\t0x%x\tuint8_t bDeviceSubClass", hs->bDeviceSubClass);
	_debug_log_info("USB������豸Э�����\t0x%x\tuint8_t bDeviceProtocol", hs->bDeviceProtocol);
	_debug_log_info("�˵��������С\t0x%x\tuint8_t bMaxPacketSize", hs->bMaxPacketSize);
	_debug_log_info("���̱�ţ���USB Org���䣩\t0x%x\tuint16_t idVendor", hs->idVendor);
	_debug_log_info("��Ʒ��ţ��������̷��䣩\t0x%x\tuint16_t idProduct", hs->idProduct);
	_debug_log_info("�豸�������\t0x%x\tuint16_t bcdDevice", hs->bcdDevice);
	_debug_log_info("�豸�����ַ�������\t0x%x\tuint8_t iManufacturer", hs->iManufacturer);
	_debug_log_info("��Ʒ�ַ�������\t0x%x\tuint8_t iProduct", hs->iProduct);
	_debug_log_info("�豸���к�����\t0x%x\tuint8_t �豸���к�����", hs->iSerialNumber);
	_debug_log_info("���ܵ���������\t0x%x\tuint8_t bNumConfigurations", hs->bNumConfigurations);
#endif
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
#ifndef NO_Debug
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
	_debug_log_info("\r\n���� ������\tstruct _ConfigurationDescriptor{}\r\n");

	_debug_log_info("�豸���������ֽ���\t0x%x\tuint8_t bLength", cfgDesc->bLength);
	_debug_log_info("�������������ͱ��\t0x%x\tuint8_t bDescriptorType", cfgDesc->bDescriptorType);
	_debug_log_info("�����÷��ص��������ݴ�С\t0x%x\tuint16_t wTotalLength", cfgDesc->wTotalLength);
	_debug_log_info("������֧�ֵĽӿ�����\t0x%x\tuint8_t bNumInterfaces", cfgDesc->bNumInterfaces);
	_debug_log_info("��������Ҫ�Ĳ���\t0x%x\tuint8_t bConfigurationValue", cfgDesc->bConfigurationValue);
	_debug_log_info("���������õ��ַ�������\t0x%x\tuint8_t iConfiguration", cfgDesc->iConfiguration);
	_debug_log_info("����ģʽ��ѡ��\t0x%x\tuint8_t bmAttributes", cfgDesc->bmAttributes);
	_debug_log_info("�豸�����߻�ȡ��������\t0x%x\tuint8_t bMaxPower", cfgDesc->bMaxPower);

	/** �ӿ� ������ **/
	_debug_log_info("\r\n�ӿ� ������\tstruct _InterfaceDescriptor{}\r\n");

	_debug_log_info("�ӿ����������ֽ���\t0x%x\tuint8_t bLength", itfDesc->bLength);
	_debug_log_info("�ӿ������������ͱ��\t0x%x\tuint8_t bDescriptorType", itfDesc->bDescriptorType);
	_debug_log_info("�ýӿڵı��\t0x%x\tuint8_t wTotalLength", itfDesc->bInterfaceNumber);
	_debug_log_info("���õĽӿ��������ı��\t0x%x\tuint8_t bNumInterfaces", itfDesc->bAlternateSetting);
	_debug_log_info("�ýӿ�ʹ�� �Ķ˵������������˵�0\t0x%x\tuint8_t bConfigurationValue", itfDesc->bNumEndpoints);
	_debug_log_info("�ӿ���\t0x%x\tuint8_t iConfiguration", itfDesc->bInterfaceClass);
	_debug_log_info("�ӿ�����\t0x%x\tuint8_t bmAttributes", itfDesc->bInterfaceSubClass);
	_debug_log_info("�ӿ���Э��\t0x%x\tuint8_t bMaxPower", itfDesc->bInterfaceProtocol);
	_debug_log_info("�����ýӿڵ��ַ�������ֵ\t0x%x\tuint8_t bMaxPower", itfDesc->iInterface);

	/** �˵� ������ **/
	_debug_log_info("\r\n�˵� ������\tstruct _EndpointDescriptor{}\r\n");

	_debug_log_info("�˵��������ֽ���\t0x%x\tuint8_t bLength", epDesc->bLength);
	_debug_log_info("�˵����������ͱ��\t0x%x\tuint8_t bDescriptorType", epDesc->bDescriptorType);
	_debug_log_info("�˵��ַ�������������\t0x%x\tuint8_t wTotalLength", epDesc->bEndpointAddress);
	_debug_log_info("�˵�Ĵ�������\t0x%x\tuint8_t bNumInterfaces", epDesc->bmAttributes);
	_debug_log_info("�˵��շ���������С\t0x%x\tuint16_t iConfiguration", epDesc->wMaxPacketSize);
	_debug_log_info("������ѯ�˵��ʱ����\t0x%x\tuint8_t bmAttributes", epDesc->bInterval);
#endif
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
	_debug_log_info("�豸ö�����!");
}

//�޷�ʶ���USB�豸
void USBH_USR_DeviceNotSupported(void)
{
	USBH_Msg_Show(0);//�޷�ʶ���USB�豸
	//_debug_log_info("�޷�ʶ���USB�豸!");
}

//�ȴ��û����밴��,ִ����һ������
USBH_USR_Status USBH_USR_UserInput(void)
{
	_debug_log_info("�����û�ȷ�ϲ���!");
	UsbDeviceState=1;//USB�豸�Ѿ����ӳɹ�
//	TIM_Cmd(TIM3, ENABLE); 	
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
	_debug_log_info("�޷��ָ��Ĵ���!!!");
}

/* Logitech Extreme 3D ����ҡ�� ��ʼ�� */
void  USR_Logitech_Init (void) // ���� Logitech Extreme 3D ����ҡ��
{
	USBH_Msg_Show(4);		//USB Logitech Extreme 3D ����ҡ��
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
		if(errcnt>3000)//������
		{
			errcnt=0;
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS ��λ
			USB_OTG_BSP_mDelay(4);
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//��λ����
			return 1;
		}
	} else errcnt=0;
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
