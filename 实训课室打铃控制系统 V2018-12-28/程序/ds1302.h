#ifndef __DS1302_H_
#define __DS1302_H_

//---����ͷ�ļ�---//

#include<reg51_typedef.h>

//---����ds1302ʹ�õ�IO��---//

sbit DSIO=P3^4;
sbit RST=P3^5;
sbit SCLK=P3^6;

//---����ȫ�ֺ���---//

/*  ��DS1302�����ַ+���ݣ�  */
void Ds1302Write(uchar addr, uchar dat);
/*  ��ȡһ����ַ������  */
uchar Ds1302Read(uchar addr);
/*  ��ʼ��DS1302.  */
void Ds1302Init(void);//�ꡢ�¡��ա����ڡ�ʱ���֡���  
/*  ��ȡʱ����Ϣ  */
void Ds1302ReadTime(void);
	
//---����ȫ�ֱ���--//

extern uchar TIME[7];	//����ȫ�ֱ���

#endif