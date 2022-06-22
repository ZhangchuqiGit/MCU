#include "SysTick.h"

void Delay_ms(u16 time)
	{
	  SysTick->LOAD=9000*time;							//���ü�������ʼֵ
	  SysTick->VAL=0;									//��ռ�����ֵ
	  SysTick->CTRL=0x00000001;					   	    //ʹ�ܼ����������жϣ�����ʱ��
	  
	  while(!(SysTick->CTRL&0x00010000));			    //�ȴ���������С��0
	  
	  SysTick->CTRL=0;									//��ռ�����
	  SysTick->VAL=0;	  							    //�رռ�����
	}

void Delay_us(u16 time)
	{
	  SysTick->LOAD=9*time;								//���ü�������ʼֵ
	  SysTick->VAL=0;									//��ռ�����ֵ
	  SysTick->CTRL=0x00000001;					   	    //ʹ�ܼ����������жϣ�����ʱ��
	  
	  while(!(SysTick->CTRL&0x00010000));			    //�ȴ���������С��0
	  
	  SysTick->CTRL=0;									//��ռ�����
	  SysTick->VAL=0;	  							    //�رռ�����
	}
