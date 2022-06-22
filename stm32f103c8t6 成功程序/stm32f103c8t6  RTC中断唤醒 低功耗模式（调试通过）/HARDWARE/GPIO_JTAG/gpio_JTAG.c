#include "GPIO_JTAG.h"

/*最近博主用STM32F103C8T6做了一个温度测控模块，用到PB3，PB4，PA15等引脚控制外设。
发现不管怎么配置，这三个引脚都不能置零。后来发现是包括这三个引脚在内的PB3，PB4，PA13，PA14，PA15是特殊的IO口，
用作JTAG/SWD仿真器的调试接口。其中PA13，PA14分别作为SWD调试的SWIO和SWCLK；PB3，PB4，PA13，PA14，PA15共同用于JTAG
这五个IO引脚非常特殊，正常情况下作为SWJ仿真器的调试引脚，如果要作为普通IO口使用需要特别的配置
************
以PA13引脚为例：
相较与其他的普通IO，PA13的Main function 为JTMS-SWDIO。反而普通IO口的功能在Alternate functions中的remap里。
也就是说PA13要想当做普通IO口使用，就必须使用它复用功能中的重映射。因此就需要这样的两步操作：
一.在时钟配置中打开复用时钟：RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
二.对PA13引脚进行重映射：   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
**********************************************************************************************
复用重映射和调试I/O配置寄存器(AFIO_MAPR) ： 地址偏移： 0x04   复位值： 0x0000 0000
位31:27 保留。
位26:24 SWJ_CFG[2:0]：串行线JTAG配置
这些位可由软件读写，用于配置SWJ和跟踪复用功能的I/O口。 SWJ(串行线JTAG)支持JTAG或
SWD访问Cortex的调试端口。系统复位后的默认状态是启用SWJ但没有跟踪功能，这种状态下
可以通过JTMS/JTCK脚上的特定信号选择JTAG或SW(串行线)模式。
000：完全SWJ(JTAG-DP + SW-DP)：复位状态；
001：完全SWJ(JTAG-DP + SW-DP)但没有JNTRST；
010：关闭JTAG-DP，启用SW-DP；
100：关闭JTAG-DP，关闭SW-DP；
其它组合：禁用。
位23:21 保留。
位20 ADC2_ETRGREG_REMAP： ADC2规则转换外部触发重映射
该位可由软件置’1’或置’0’。它控制与ADC2规则转换外部触发相连的触发输入映像。当该位
置’0’时， ADC2规则转换外部触发与EXTI11相连；当该位置’1’时， ADC2规则转换外部触发与
TIM8_TRGO相连。
位19 ADC2_ETRGINJ_REMAP： ADC2注入转换外部触发重映射
该位可由软件置’1’或置’0’。它控制与ADC2注入转换外部触发相连的触发输入映像。当该位
置’0’时， ADC2注入转换外部触发与EXTI15相连；当该位置’1’时， ADC2注入转换外部触发与
TIM8通道4相连。
位18 ADC1_ETRGREG_REMAP： ADC1规则转换外部触发重映射
该位可由软件置’1’或置’0’。它控制与ADC2规则转换外部触发相连的触发输入映像。当该位
置’0’时， ADC1规则转换外部触发与EXTI11相连；当该位置’1’时， ADC1规则转换外部触发与
TIM8_TRGO相连。
位17 ADC1_ETRGINJ_REMAP： ADC1注入转换外部触发重映射
该位可由软件置’1’或置’0’。它控制与ADC2注入转换外部触发相连的触发输入映像。当该位
置’0’时， ADC2注入转换外部触发与EXTI15相连；当该位置’1’时， ADC1注入转换外部触发与
TIM8通道4相连。
位16 TIM5CH4_IREMAP： TIM5通道4内部重映射
该位可由软件置’1’或置’0’。它控制TIM5通道4内部映像。当该位置’0’时， TIM5_CH4与PA3相
连；当该位置’1’时， LSI内部振荡器与TIM5_CH4相连，目的是对其进行校准。
位15 PD01_REMAP：端口D0/端口D1映像到OSC_IN/OSC_OUT
该位可由软件置’1’或置’0’。它控制PD0和PD1的GPIO功能映像。当不使用主振荡器HSE时(系
统运行于内部的8MHz阻容振荡器)PD0和PD1可以映像到OSC_IN和OSC_OUT引脚。此功能只
能适用于36、 48和64管脚的封装(PD0和PD1出现在TQFP100的封装上，不必重映像)。
0：不进行PD0和PD1的重映像；
1： PD0映像到OSC_IN， PD1映像到OSC_OUT。
位14:13 CAN_REMAP[1:0]： CAN复用功能重映像
这些位可由软件置’1’或置’0’，控制复用功能CAN_RX和CAN_TX的重映像。
00： CAN_RX映像到PA11， CAN_TX映像到PA12；
01：未用组合；
10： CAN_RX映像到PB8， CAN_TX映像到PB9(不能用于36脚的封装)；
11： CAN_RX映像到PD0， CAN_TX映像到PD1(只适用于100脚的封装)。
位12 TIM4_REMAP：定时器4的重映像
该位可由软件置’1’或置’0’，控制将TIM4的通道1-4映射到GPIO端口上。
0：没有重映像(TIM4_CH1/PB6， TIM4_CH2/PB7， TIM4_CH3/PB8， TIM4_CH4/PB9)；
1：完全映像(TIM4_CH1/PD12， TIM4_CH2/PD13， TIM4_CH3/PD14， TIM4_CH4/PD15)。
注：重映像不影响在PE0上的TIM4_ETR。通用和复用功能I/O STM32F10xxx参考手册
位11:10 TIM3_REMAP[1:0]：定时器3的重映像
这些位可由软件置’1’或置’0’，控制定时器3的通道1至4在GPIO端口的映像。
00：没有重映像(CH1/PA6， CH2/PA7， CH3/PB0， CH4/PB1)；
01：未用组合；
10：部分映像(CH1/PB4， CH2/PB5， CH3/PB0， CH4/PB1)；
11：完全映像(CH1/PC6， CH2/PC7， CH3/PC8， CH4/PC9)。
注：重映像不影响在PD2上的TIM3_ETR。
位9:8 TIM2_REMAP[1:0]：定时器2的重映像
这些位可由软件置’1’或置’0’，控制定时器2的通道1至4和外部触发(ETR)在GPIO端口的映像。
00：没有重映像(CH1/ETR/PA0， CH2/PA1， CH3/PA2， CH4/PA3)；
01：部分映像(CH1/ETR/PA15， CH2/PB3， CH3/PA2， CH4/PA3)；
10：部分映像(CH1/ETR/PA0， CH2/PA1， CH3/PB10， CH4/PB11)；
11：完全映像(CH1/ETR/PA15， CH2/PB3， CH3/PB10， CH4/PB11)。
位7:6 TIM1_REMAP[1:0]：定时器1的重映像
这些位可由软件置’1’或置’0’，控制定时器1的通道1至4、 1N至3N、外部触发(ETR)和断线输入
(BKIN)在GPIO端口的映像。
00：没有重映像(ETR/PA12， CH1/PA8， CH2/PA9， CH3/PA10， CH4/PA11， BKIN/PB12，
CH1N/PB13， CH2N/PB14， CH3N/PB15)；
01：部分映像(ETR/PA12， CH1/PA8， CH2/PA9， CH3/PA10， CH4/PA11， BKIN/PA6，
CH1N/PA7， CH2N/PB0， CH3N/PB1)；
10: 未用组合；
11: 完全映像(ETR/PE7， CH1/PE9， CH2/PE11， CH3/PE13， CH4/PE14， BKIN/PE15，
CH1N/PE8， CH2N/PE10， CH3N/PE12)。
位5:4 USART3_REMAP[1:0]： USART3的重映像
这些位可由软件置’1’或置’0’，控制USART3的CTS、 RTS、 CK、 TX和RX复用功能在GPIO端
口的映像。
00: 没有重映像(TX/PB10， RX/PB11， CK/PB12， CTS/PB13， RTS/PB14)；
01: 部分映像(TX/PC10， RX/PC11， CK/PC12， CTS/PB13， RTS/PB14)；
10: 未用组合；
11: 完全映像(TX/PD8， RX/PD9， CK/PD10， CTS/PD11， RTS/PD12)。
位3 USART2_REMAP： USART2的重映像
这些位可由软件置’1’或置’0’，控制USART2的CTS、 RTS、 CK、 TX和RX复用功能在GPIO端
口的映像。
0: 没有重映像(CTS/PA0， RTS/PA1， TX/PA2， RX/PA3， CK/PA4)；
1: 重映像(CTS/PD3， RTS/PD4， TX/PD5， RX/PD6， CK/PD7)；
位2 USART1_REMAP： USART1的重映像
该位可由软件置’1’或置’0’，控制USART1的TX和RX复用功能在GPIO端口的映像。
0: 没有重映像(TX/PA9， RX/PA10)；
1: 重映像(TX/PB6， RX/PB7)。
位1 I2C1_REMAP： I2C1的重映像
该位可由软件置’1’或置’0’，控制I2C1的SCL和SDA复用功能在GPIO端口的映像。
0: 没有重映像(SCL/PB6， SDA/PB7)；
1: 重映像(SCL/PB8， SDA/PB9)。
位0 SPI1_REMAP： SPI1的重映像
该位可由软件置’1’或置’0’，控制SPI1的NSS、 SCK、 MISO和MOSI复用功能在GPIO端口的映
像。
0: 没有重映像(NSS/PA4， SCK/PA5， MISO/PA6， MOSI/PA7)；
1: 重映像(NSS/PA15， SCK/PB3， MISO/PB4， MOSI/PB5 
*/

//	JTAG_SWD_GPIO_Config();//不使用JTAG调试，对应的IO口如PB3,PB4,PA15引脚作为普通IO口使用，可以在使用SWD仿真和烧录的情况下释放出来
void JTAG_SWD_GPIO_Config(void)//stm32的PB3 PB4 PA15引脚可以在使用SWD仿真和烧录的情况下释放出来，作为普通IO口使用
{
 /*   //打开重映射时钟和USART重映射后的I/O口引脚时钟
    //使能对应IO口的时钟，因为是复用功能所以还要使能AFIO时钟    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//使能GPIOA时钟 //在时钟配置中打开复用时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //对PA13引脚进行重映射
    
  //  AFIO->MAPR=0;
    AFIO->MAPR&=0XF8FFFFFF;
    AFIO->MAPR|=0x0C ;*/
    
    /*这段初始化代码放在所有IO初始化最后！！！！！！
    当把PB3 PB4 PA15(JTAG=SWD+ PB3+PB4+PA15)初始化好之后，如果再次调用GPIOA GPIOB接口的时钟代码的话！
	会使得PB3 PA15引脚变回JTAG的引脚即初始化完之后不能再执行如下代码，PB3 PA15引脚会变回JTAG的引脚，而无法作为普通IO使用*/
    GPIO_InitTypeDef GPIO_InitStructure;  
  
    //打开重映射时钟和USART重映射后的I/O口引脚时钟
    //使能对应IO口的时钟，因为是复用功能所以还要使能AFIO时钟    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//重映射，不使用JTAG调试，对应的IO口作为普通IO口使用	
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);//重映射，不使用JTAG调试，对应的IO口作为普通IO口使用	

/********------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推免输出   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);//设置 0  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_3);  

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);  
}


