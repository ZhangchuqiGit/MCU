#include "serial_bus.h"

/* SBUS全称serial-bus，是一种串口通信协议，广泛应用于航模遥控器（接收机）中。
只用一根信号线就能传输多达16通道的数据，比多路PWM捕获高效且省资源。

串口配置：
	100kbps(100000 bps，区别于 115200 bps) 波特率，
	8位数据位（在stm32中要选择9位)，
	2位停止位，偶校验（EVEN)，无控流，25字节。

协议格式：  [startbyte] [data1][data2]…[data22] [flags] [endbyte]   : 25字节
	startbyte 	= 0x0f;
	endbyte 	= 0x00; 
	data1…data22: 	LSB(低位在前)，对应16个通道(ch1-ch16)，每个通道 11 bit(22*8 = 16*11)；
	
	flag: 	失控保护标志
	乐迪AT9S在遥控器通上的时候是0x00，断开的时候是0xC0，可以通过查询flag位来采取失控保护。
	flags标志位是用来检测控制器与px4是否断开的标志位。
	flags=1：控制器与接收器保持连接
	flags=0：控制器与接收器断开（失控），px4会控制电机停转。

数据范围
	航模遥控器输出的PWM值是1000~2000，中值为1500，
	sbus 输出的会不一样，跟遥控器厂家有关，例如乐迪AT9S的范围为300 ~ 1700，中值1000。

sbus 的负逻辑
	这个地方一定要万分注意，信号必须硬件取反，软件取反是无效的，
	因为SBUS的信号是采用的负逻辑，也就是 电平相反，不要试图在软件里面取反，
	因为软件里面只能操作数据位（记得串口配置里面的数据位8么），
	你是操作不了停止位、校验位啥的！！如果是自己画板子也很简单。
	
间隔模式
	高速模式：每隔7ms一帧数据，因为两帧的间隔只有超过3ms，才会被接受，
		而根据波特率计算一下，发送25字节需要的时间 + 3~4ms = 7ms。
	普通模式：每隔14ms一帧数据 = 发送25字节需要的时间 + 3~4ms。  

数据范围在0-2047之间，基本上是1102~1927，中值为1500；

ch1的11位=data2的低3位+data1的8位；
例如：data1=00110110，data2=11001111；
这时ch1=111 00110110=1846；通道1 的值就为1846；
ch2=data3的低6位+data2的高5位；
ch3=data5的低1位+data4的8位+data3的高2位；
ch4=4 7;
ch5=7 4;
ch6=2 8 1;
ch7=5 6;
ch8=8 3;
ch9=6 5;
ch10=1 8 2;
ch11=4 7;
ch12=7 4;
ch13=2 8 1;
ch14=5 6;
ch15=8 3;
ch16=6 5;

*/
	
void USART1_SBUS_Init(void) // 只发送
{
#if EN_USART1_RX	
	NVIC_InitTypeDef NVIC_InitStructure ;//定义中断结构体
 #endif
	GPIO_InitTypeDef GPIO_InitStructure;//定义IO初始化结构体
	USART_InitTypeDef USART_InitStructure;//定义串口结构体
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); /*GPIOA9复用为USART1 TX */
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1 RX
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; /* | GPIO_Pin_10 ; */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/*速度*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = 100000;/*波特率设置 100kbps*/
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;/*SBUS在stm32中要选择9位数据格式*/
	USART_InitStructure.USART_StopBits = USART_StopBits_2;/*2个停止位*/
	USART_InitStructure.USART_Parity = USART_Parity_Even;/*偶校验*/
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	/*只发模式*/
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
}

uint16_t sbus_8b(uint8_t num) // 1000 ~ 2000
{
	float val = (num / 255.0f) * 1000.0f;
	uint16_t sbusval = val + 1000.0f + 0.5f;
	return sbusval;
}

uint16_t sbus_16b(uint16_t num) // 1000 ~ 2000
{
	float val = (num / 1023.0f) * 1000.0f;
	uint16_t sbusval = val + 1000.0f + 0.5f;
	return sbusval;
}

//uint16_t button_to_sbus(uint8_t flag)
//{
//	if(flag != 0) return 1800;
//	return 1200;	
//}

#define SBUS_FRAME_SIZE 25
#define SBUS_RANGE_MIN 200.0f
#define SBUS_RANGE_MAX 1800.0f
#define SBUS_TARGET_MIN 1000.0f
#define SBUS_TARGET_MAX 2000.0f 
#define SBUS_SCALE_FACTOR ((SBUS_TARGET_MAX - SBUS_TARGET_MIN)/(SBUS_RANGE_MAX-SBUS_RANGE_MIN))//0.625f = (2000-1000) / (1800-200)
#define SBUS_SCALE_OFFSET (int)(SBUS_TARGET_MIN - (SBUS_SCALE_FACTOR * SBUS_RANGE_MIN + 0.5f)) //874.5f = 1000 - (0.625f * 200)

//合成函数：
//void sbus_out（uint8_t num_values）;//参数为通道数；
//values[16]就是对应的16个通道；

uint16_t values[16]={0}; // 通道

void sbus_send(uint8_t num_values/*通道数*/)
{
	uint16_t value=0;
	
	uint8_t oframe[25];
	memset(oframe, 0, 25);
	oframe[0]=0x0f;
//	oframe[24]=0x00;

	for (uint8_t i=0, byteindex=1, offset=0; (i < num_values) && (i < 16); ++i)
	{
//		value = values[i];
		value = (unsigned short)( ( (values[i]-SBUS_SCALE_OFFSET) / SBUS_SCALE_FACTOR) + 0.5f);
		if (value > 0x07ff)	value = 0x07ff; // 0x07ff = 2047
		while (offset >= 8) {
			++byteindex;
			offset -= 8;
		}		 
		oframe[byteindex] |= (value << (offset)) & 0xff;
		oframe[byteindex + 1] |= (value >> (8 - offset)) & 0xff;
		oframe[byteindex + 2] |= (value >> (16 - offset)) & 0xff;
		offset += 11;
	}
	
	for (uint8_t i=0; i < 25 ; ++i)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	// 等待发送结束
		USART_SendData(USART1, (uint16_t)oframe[i]); // 每次1字节数据发送到串口
	}
#if 0 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	// 等待发送结束
	USART_SendData(USART1, (uint16_t)'\n'); // 每次1字节数据发送到串口
#endif
}

/* 协议格式：[startbyte]  [data1] [data2] … [data22]      [flags] [endbyte]  : 25字节 
   发送       0x0f        X Y RZ Slider Hat_Switch button   0x00    0x00      : 25字节 */

void sbus_out(HID_Logitech_Data_Analyze *Logitech_data) // 发送
{
	values[0] = 2000 - (sbus_8b(Logitech_data->RZ) - 1000); // CH1
	values[1] = sbus_16b(Logitech_data->Y); // CH2
	values[2] = sbus_8b(Logitech_data->Slider); // CH3
	values[3] = sbus_16b(Logitech_data->X); // CH4

	for (uint16_t i=0x0001, j=0; i & 0x0fff ; i = i<<1, ++j) 
	{
//		if (Logitech_data->button & i) retval = button_to_sbus(1);
//		else retval = button_to_sbus(0);
		if (Logitech_data->button & i) values[4+j] = 1800;
		else values[4+j] = 1200;
	}
	sbus_send(16);
}


