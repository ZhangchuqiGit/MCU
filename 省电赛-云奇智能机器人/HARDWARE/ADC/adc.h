#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

void ADC1_Mode_init(void);///stm32_adcת����ģ�������ΪPB0

//ģ�������ΪPA 0 1 2 3/*����ADC1�Ĺ���ģʽΪMDAģʽ  */
void ADC1_Init(void);
/*���ò���ͨ���˿� ʹ��GPIOʱ��	  ����ADC����PA0�˿��ź�*/
void ADC1_GPIO_Config(void);
void ADC_Mode_Config(void);
/*����ADC1�Ĺ���ģʽΪMDAģʽ  */
void DMA_Mode_Config(void);

#endif /* __ADC_H */

