#include"LED.h"

void LED_GPIO_Config(void)
{
  /*定义设置IO口的结构体*/
  GPIO_InitTypeDef GPIO_InitStruct;	 

  /*打开GPIOA的时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

  /*选择需要设置的IO口*/
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;

  /*设置为推挽输出*/
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;

  /*设置IO口速率为50Mhz*/
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  /*调用IO初始化函数*/
  GPIO_Init(GPIOC,&GPIO_InitStruct);
}

