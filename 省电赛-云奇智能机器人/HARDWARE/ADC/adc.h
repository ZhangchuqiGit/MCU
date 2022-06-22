#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

void ADC1_Mode_init(void);///stm32_adc转换，模拟输入端为PB0

//模拟输入端为PA 0 1 2 3/*配置ADC1的工作模式为MDA模式  */
void ADC1_Init(void);
/*配置采样通道端口 使能GPIO时钟	  设置ADC采样PA0端口信号*/
void ADC1_GPIO_Config(void);
void ADC_Mode_Config(void);
/*配置ADC1的工作模式为MDA模式  */
void DMA_Mode_Config(void);

#endif /* __ADC_H */

