#ifndef __ADC_H
#define	__ADC_H

#include "sys.h"	 
#include "delay.h"


#define ADC_load    1
#define ADC_Temp_g    1 //打开ADC内部温度传感器

#if ADC_load==0   //单路通道

void ADC1_Mode_init(void);///stm32_adc转换，模拟输入端为PB0

extern __IO u16 ADC_ConvertedValue;  //单路通道
u16 Get_Adc(u8 ch);  //获得ADC值   ch:通道值
u16 Get_Adc_Average(u8 ch,u8 times);

#else

//模拟输入端为PA 0 1 2 3/*配置ADC1的工作模式为MDA模式  */
void ADC1_Init(void);
void ADC1_GPIO_Config(void);
void DMA_Mode_Config(void);
void ADC_Mode_Config(void);

extern __IO u16 ADC_ConvertedValue[5];  //多路通道  存放结果

#endif 

#if  ADC_Temp_g  //打开ADC内部温度传感器
u16 Get_ADC_Temp_Value(u8 ch,u8 times);
int Get_Temperture(void);
#endif 


#endif /* __ADC_H */

