#ifndef		__UITR_H_
#define		__UITR_H_



#include "stm32f10x.h"

/*IO�ܽŶ���*/

/*����IO*/
#define		TRIG_H		GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define		TRIG_L		GPIO_ResetBits(GPIOB,GPIO_Pin_0)
/*����IO*/
#define		ECHO_IN		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)

void Uitr_GPIO_Config(void);


#endif
