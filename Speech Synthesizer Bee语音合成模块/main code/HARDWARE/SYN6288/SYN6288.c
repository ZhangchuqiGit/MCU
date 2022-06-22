#include "SYN6288.h"

/*	与SYN6288采用串口通讯方式
TX-->SYN6288_RX (芯片RX引脚需要外接个电平转换,我画的测试板上已经有转换电路了)
RX<--SYN6288_TX
PA8<---> SYN6288_BUSY  (可不接)               */
	
//       数据包头（0xFD(帧头) + 2字节长度(高字节在前低字节在后) + 1字节命令字 + 1字节命令参数)
unsigned char head[] = {0xfd,	0x00,		0x00,					0x01,			0x00};
/*   text   文本长度必须小于等于200个字节。实际发送的长度大于200字节，芯片会报接收失败*/

//	char text1[]={"[v16]当前时间为："};//[v0-16]设置前景文本播放音量（含提示音效）
//	char text2[]={"[m16]123456789"};//[m0-16]设置背景音乐音量
//	char text3[]={"[t4]123456789"};//[t0-5]设置词语语速（针对自然朗读方式）
//	char text4[]={"[n0]123456789"};//[n?]设置数字处理策略
//	/*  为0，自动判断; 为1，数字作号码处理; 为2，数字作数值处理  */
//	char text5[]={"[y0]123456789"};//[y?]设置号码中“1”的读法
//	/*  为0，合成号码时“1”读成“幺” ; 为1，合成号码时“1”读成“一” */
//	char text6[]={"[x0]123456789"};//[x?]设置提示音处理策略
//	/*	为0，不使用提示音 ;	为1，默认使用提示音 */
//	char text7[]={"[b0]123456789"};//[b?]设置标点是否读出
//	/* 为0，不读标点;	为1，读标点 */
//	char text8[]={"[o0]123456789"};//[o?]设置文本朗读方式
//	/*  为0，设置为自然朗读方式 ;	为1，设置为Word-By-Word方式 */
//	char text9[]={"123456789"};
//	char text10[]={"[d][v8][m2][t5][y0][x1][o0]"};/*初始化综合举例*/
//	char text11[]={"soundb！soundr"};//提示音 见PDF
//	/*提示音名称前面或后面紧接着是英文字母时，需要使用标点符号、空格、回车等与其他字母隔开，
//	芯片才能够自动识别 */
//	char text12[]={"[2]播报[2]完毕，[r]谢谢"};

/*
强制后一个汉字按姓氏发音     [r] 此控制标记后的1个汉字强制读成姓氏（主要用于多音
字姓氏的处理），若没有紧跟1个汉字，则此控制无效;
强制后两个汉字组成两字词     [2] 此控制标记后的2个汉字强制读成“两字词”，若没有
紧跟2个汉字，则此控制无效;
强制后三个汉字组成三字词     [3] 此控制标记后的3个汉字强制读成“三字词”，若没有
紧跟3个汉字，则此控制无效 ;             */
//Speech( text1, strlen(text1) );
/*	music_sel : 所选背景音乐的编号1~15, 0为无背景音乐
	w_s : 设置文本为编码格式:
			0 :设置文本为：GB2312 编码格式
			1 :设置文本为：GBK 编码格式
			2 :设置文本为：BIG5 编码格式
			3 :设置文本为：UNICODE 编码格式    */

/********************************************************************************************/
//void Speech(char *buf,u8 music_sel,u8 w_s)
void Speech(char *buf,u8 music_sel) 
{
	u8 i = 0;          //循环计数变量
	u8 len=strlen(buf);
	unsigned char SYN6288_xor = 0x00;     //校验码初始化					
	head[2] = len + 3;  //计算正文长度（数据区：1命令字 + 1命令参数 + 文字长度 len + 1校验位）
	head[3]=0x01;//命令字 : 语音合成播放命令
	head[4]= 0xf8 & ( (music_sel&0x0f) << 3) ;//命令参数  
//	head[4]= w_s|(0xf8 & ( (music_sel&0x0f) << 3)) ;//命令参数  

/*发送数据包头（0xFD + 2字节长度 + 1字节命令字 + 1字节命令参数)*/
	for(i = 0; i < 5; i++)
	{
		SYN6288_xor ^= head[i];
		UARTSendByte_TXE(USART1,head[i]); /*发送一个字节数据*/
	}
/*发送文字内容*/
	for(i = 0; i < len; i++)
	{
		SYN6288_xor ^= buf[i];
		UARTSendByte_TXE(USART1,buf[i]);
	}
/*发送校验位*/
	UARTSendByte_TXE(USART1,SYN6288_xor);        //发送校验位
}
/********************************************************************************************/
void Speech_bound( u16 bound )//设置通讯波特率命令(初始波特率为9600bps)
{/*上位机若需改变波特率，在发送完设置波特率命令帧后需间隔 16 毫秒 再发送其它命令帧*/
	u8 i = 0;          //循环计数变量
	unsigned char SYN6288_xor = 0x00;     //校验码初始化	
	head[2] = 0+3;//计算正文长度（数据区：1命令字 + 1命令参数 + 无文本 + 1校验位）
	head[3]=0x31;//命令字 : 设置通讯波特率命令
	switch(bound)//命令参数  
	{/*设置波特率*/
		case 9600:	head[4]=0x00;break;//命令参数  
		case 19200:	head[4]=0x01;break;//命令参数  
		case 38400:	head[4]=0x02;break;//命令参数  
		default:	head[4]=0x00;break;//9600
	}
/*发送数据包头（0xFD + 2字节长度 + 1字节命令字 + 1字节命令参数)*/
	for(i = 0; i < 5; i++)
	{
		SYN6288_xor ^= head[i];
		UARTSendByte_TXE(USART1,head[i]); /*发送一个字节数据*/
	}
/*发送校验位*/
	UARTSendByte_TXE(USART1,SYN6288_xor);        //发送校验位
}
/********************************************************************************************/
void Speech_sel(u8 num)  //命令
{    
	u8 i = 0;          //循环计数变量
	unsigned char SYN6288_xor = 0x00;     //校验码初始化	
	head[2] = 2;//计算正文长度（数据区：1命令字 + 无命令参数 + 无文字长度 + 1校验位）
	switch(num)//命令字 
	{
		case 0:head[3]=0x02;break;//停止合成命令
		case 1:head[3]=0x03;break;//暂停合成命令
		case 2:head[3]=0x04;break;//恢复合成命令
		case 3:head[3]=0x21;break;//芯片状态查询命令
		/*通过该命令来判断TTS 模块是否正常工作，以及获取相应返回参数，
		返回0x4E 表明芯片仍在合成播音中，返回0x4F 表明芯片处于空闲状态*/
		case 4:head[3]=0x88;break;//芯片进入 Power Down 模式命令----休眠命令
/*唤醒后需间隔 16 毫秒 再发送命令数据*/
		default:break;
	}
/*发送数据包头（0xFD + 2字节长度 + 1字节命令字 + 无命令参数)*/
	for(i = 0; i < 4; i++)
	{
		SYN6288_xor ^= head[i];
		UARTSendByte_TXE(USART1,head[i]); /*发送一个字节数据*/
	}
/*发送校验位*/
	UARTSendByte_TXE(USART1,SYN6288_xor);        //发送校验位
}
/********************************************************************************************/
void TTS_Init(void) /* PA8<---> SYN6288_BUSY  (可不接) */
{      
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);            
}


