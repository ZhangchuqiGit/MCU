#ifndef __ADC_H
#define	__ADC_H

#include "sys.h"	 
#include "delay.h"


#define ADC_load    1
#define ADC_Temp_g    1 //��ADC�ڲ��¶ȴ�����

#if ADC_load==0   //��·ͨ��

void ADC1_Mode_init(void);///stm32_adcת����ģ�������ΪPB0

extern __IO u16 ADC_ConvertedValue;  //��·ͨ��
u16 Get_Adc(u8 ch);  //���ADCֵ   ch:ͨ��ֵ
u16 Get_Adc_Average(u8 ch,u8 times);

#else

//ģ�������ΪPA 0 1 2 3/*����ADC1�Ĺ���ģʽΪMDAģʽ  */
void ADC1_Init(void);
void ADC1_GPIO_Config(void);
void DMA_Mode_Config(void);
void ADC_Mode_Config(void);

extern __IO u16 ADC_ConvertedValue[5];  //��·ͨ��  ��Ž��

#endif 

#if  ADC_Temp_g  //��ADC�ڲ��¶ȴ�����
u16 Get_ADC_Temp_Value(u8 ch,u8 times);
int Get_Temperture(void);
#endif 


#endif /* __ADC_H */

