#include"ds1302.h"

//---DS1302д��Ͷ�ȡʱ����ĵ�ַ����---//
//---���ʱ�������� ���λ��дλ;-------//
//uchar code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
//uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
uchar code READ_RTC_ADDR[7] = {0x8d,0x89,0x87,0x8b,0x85,0x83,0x81}; /*��������ʱ����*/
uchar code WRITE_RTC_ADDR[7] = {0x8c,0x88,0x86,0x8a,0x84,0x82,0x80};/*��������ʱ����*/
uchar TIME[7] = {0x15,0x12,0x05,0x06,0x08,0x44,0x50};/*��������ʱ����*/

/*  ��DS1302�����ַ+���ݣ�  */
void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	RST = 0;
	_nop_();
	SCLK = 0;//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	RST = 1; //Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();
	for (n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01;//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n=0; n<8; n++)//д��8λ����
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();	
	}		 
	RST = 0;//�������ݽ���
	_nop_();
}
/*  ��ȡһ����ַ������  */
uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	RST = 0;
	_nop_();
	SCLK = 0;//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	RST = 1;//Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();
	for(n=0; n<8; n++)//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01;//���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;//DS1302�½���ʱ����������
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)//��ȡ8λ����
	{
		dat1 = DSIO;//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;//DS1302�½���ʱ����������
		_nop_();
	}
	RST = 0;
	_nop_();	//����ΪDS1302��λ���ȶ�ʱ��,����ġ�
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return dat;	
}
/*  ��ʼ��DS1302.  */
void Ds1302Init(void)//�ꡢ�¡��ա����ڡ�ʱ���֡���  
{	
	uchar n;
	Ds1302Write(0x8E,0X00);		 //��ֹд���������ǹر�д��������
	for (n=0; n<7; n++)//д��7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //��д��������
}
/*  ��ȡʱ����Ϣ  */
void Ds1302ReadTime(void)
{
	uchar n;
	for (n=0; n<7; n++)//��ȡ7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}


