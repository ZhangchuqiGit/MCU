
#ifndef __SERIAL_BUS_H
#define __SERIAL_BUS_H

#include "sys.h"
#include "usbh_hid_Logitech.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void USART1_SBUS_Init(void);

void sbus_out(HID_Logitech_Data_Analyze *Logitech_data);


#endif



