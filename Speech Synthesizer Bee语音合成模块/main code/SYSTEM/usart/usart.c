#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
/*如果使用ucos,则包括下面的头文件即可.   */
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "semphr.h"	 
#endif

/*******-----------******+++++++++++++++++++++++++++*******------------*****/
#if 1   //加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数  
/* 重定向c库函数printf到USART1*/ 
int fputc(int ch, FILE *f)
{      
	/* 将Printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}
#else
/*使用microLib的方法*/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	  
    return ch;
}
int GetKey (void)  
{ 
    while (!(USART1->SR & USART_FLAG_RXNE));
    return ((int)(USART1->DR & 0x1FF));
}
#endif 

char text4_tts[100];
/************************************************** 串口1 *********************************************************/
#if EN_USART1_RX   //如果串口1使能了接收

char USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART1_REC_LEN个字节.
void uart1_init(u32 bound)
{
	//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO端口设置
    NVIC_InitTypeDef NVIC_InitStructure;//开启中断  NVIC端口设置
    USART_InitTypeDef USART_InitStructure;//USART 1-3端口设置
    /*开启外设时钟*/      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟
	USART_DeInit(USART1);  //复位串口1
	//USART1_TX		PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX		PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//Usart  NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口1接收中断
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //开启串口1发送中断  --  TXE中断方式
    //USART_ITConfig(USART1, USART_IT_TC, ENABLE); //开启串口1发送中断  --  TC中断方式
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}
	#if EN_USART_code_key  /* 数据包的形式接收 */
	u8 USART1_RX_head=0;		//接收状态标记 --  数据头
	u8 USART1_RX_num=0;		//接收状态标记 --  数据计数
	u8 USART1_RX_len=0;		//接收状态标记 --  数据包裹长度
	u8 USART1_led=0; //接收到USART1   数据   收到语音

	void USART1_IRQHandler(void)                	//串口1中断服务程序  --  数据包的形式接收
	{
		if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //中断产生   
		{   
			USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志  
			USART1_RX_BUF[USART1_RX_num] = USART_ReceiveData(USART1);   
			USART1_RX_num++;   
			USART1_RX_num &= 0xFF;   
		}   
		if(USART1_RX_BUF[USART1_RX_num-1] == (char)0xff) USART1_RX_head = USART1_RX_num-1;   //头   
		
		if( (USART1_RX_BUF[USART1_RX_head] ==(char)0xff)&&(USART1_RX_BUF[USART1_RX_num-1] ==(char)0xee))
		{   //检测到头的情况下检测到尾   
			USART1_RX_len = USART1_RX_num-1- USART1_RX_head; //长度   
			USART1_RX_num = 0; //初始化   
			USART1_RX_head = 0;  
			USART1_led=1; //收到语音			
		}   
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET) //溢出   
		{   
			USART_ClearFlag(USART1,USART_FLAG_ORE); //读SR   
			USART_ReceiveData(USART1); //读DR   
			memset(USART1_RX_BUF,0,USART1_REC_LEN); //串口接收缓冲区 清 0
		} 
	/* code ...  */	
	}
	#elif 0  //TC中断方式发送字符串
	char *pDataByte ;
	/*******功能:中断方式发送字符串,采用判断TC的方式.即 判断 发送后 中断位.
	输入: 字符串的首地址  ;   输出: 无                                *******/
	void USART_SendDataString(USART_TypeDef* USARTx,char *pucString )
	{
		pDataByte = pucString;
		USART_ClearFlag(USARTx, USART_FLAG_TC);//清除传输完成标志位,否则可能会丢失第1个字节的数据.
		USART_SendData(USARTx, *(pDataByte++) ); //必须要++，不然会把第一个字符t发送两次
	}
	void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
		if( USART_GetITStatus(USART1, USART_IT_TC) == SET  )
		{
			if( *pDataByte == '\0' )//TC需要 读SR+写DR 方可清0,当发送到最后,到’\0’的时候用个if判断关掉
			USART_ClearFlag(USART1, USART_FLAG_TC);//不然TC一直是set,TCIE也是打开的,导致会不停进入中断,clear掉即可,不用关掉TCIE
			else
			USART_SendData(USART1, *pDataByte++ );
		}	
	}
	#elif 0  //TXE中断方式发送字符串
	char *pDataByte ;
	/*******功能:中断方式发送字符串.采用判断TXE的方式.即 判断 发送后中断 位.
	输入:字符串的首地址	; 输出:无	*******/
	void USART_SendDataString(USART_TypeDef* USARTx,char *pucString )
	{
		pDataByte = pucString;
		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//只要发送寄存器为空,就会一直有中断,因此,要是不发送数据时,把发送中断关闭,只在开始发送时，才打开。
	}
	void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
		if(USART_GetITStatus(USART1, USART_IT_TXE) == SET )
		{
			if( *pDataByte == '\0' )//待发送的字节发到末尾NULL了
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);//因为是 发送寄存器空 的中断，所以发完字符串后必须关掉，否则只要空了，就会进中断
			else
			USART_SendData(USART1, *pDataByte++ );
		}	
	}
	#else 
	u16 USART1_RX_STA=0;       //接收状态标记	  
	void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
		u8 Res;
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			Res =USART_ReceiveData(USART1);	//读取接收到的数据	
			if((USART1_RX_STA&0x8000)==0)//接收未完成
			{
				if(USART1_RX_STA&0x4000)//接收到了0x0d
				{
					if(Res!=0x0a)USART1_RX_STA=0;//接收错误,重新开始
					else USART1_RX_STA|=0x8000;	//接收完成了 
				}
				else //还没收到0X0D
				{	
					if(Res==0x0d)USART1_RX_STA|=0x4000;
					else
					{
						USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
						USART1_RX_STA++;
						if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
		} 
	} 
	#endif	

#endif	

/************************************************** 串口2 *********************************************************/
#if EN_USART2_RX   ////如果串口2使能了接收

char USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART2_REC_LEN个字节.
void uart2_init(u32 bound)
{
	//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO端口设置
    NVIC_InitTypeDef NVIC_InitStructure;//开启中断  NVIC端口设置
    USART_InitTypeDef USART_InitStructure;//USART 1-3端口设置
    /*开启外设时钟*/      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    USART_DeInit(USART2);  //复位串口2
    //PA2 TXD2  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// 复用推免输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //PA3 RXD2  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //Usart  NVIC 配置 
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);     //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART2
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口2接受中断
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE); //开启串口2发送中断  
	USART_Cmd(USART2, ENABLE);                    //使能串口2

}
	#if EN_USART_code_key  /* 数据包的形式接收 */
u8 USART2_RX_head=0;		//接收状态标记 --  数据头
u8 USART2_RX_num=0;			//接收状态标记 --  数据计数
u8 USART2_RX_len=0;			//接收状态标记 --  数据包裹长度
u8 USART2_led=0; //收到语音			

void USART2_IRQHandler(void) //串口2中断服务程序  --  数据包的形式接收           
{											            	/*				蓝牙 Android			*/
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //中断产生   
    {   
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志  
        USART2_RX_BUF[USART2_RX_num] = USART_ReceiveData(USART2);   
        USART2_RX_num++;   
        USART2_RX_num &= 0xFF;   
    }  
/*------   数据 形式  "#0020...%"    -------*/	
    if(USART2_RX_BUF[USART2_RX_num-1] == '#') //数据头 "#..."
	{
		USART2_RX_head = USART2_RX_num-1;   //头  -1-					      #0001..................%	
	}									
    if( (USART2_RX_BUF[USART2_RX_head] == '#')&&(USART2_RX_BUF[USART2_RX_num-1] == '%') ) //检测到头的情况下检测到尾   
    {   
        USART2_RX_num = 0;    
		USART2_led=1; //收到语音			
		USART2_RX_len = USART2_RX_num - USART2_RX_head; //长度  			//0123456		
    }   
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //溢出   
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR   
        USART_ReceiveData(USART2); //读DR  
		memset(USART2_RX_BUF,0,USART2_REC_LEN); //串口接收缓冲区 清 0
    } 
/* code ...  */	
}
	#else 
u16 USART2_RX_STA=0;       //接收状态标记	  
void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
    } 
} 
	#endif	
	
#endif	

/************************************************** 串口3 *********************************************************/
#if EN_USART3_RX   ////如果串口2使能了接收

char USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART2_REC_LEN个字节.
void uart3_init(u32 bound)
{
	//GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO端口设置
    NVIC_InitTypeDef NVIC_InitStructure;//开启中断  NVIC端口设置
    USART_InitTypeDef USART_InitStructure;//USART 1-3端口设置
    /*开启外设时钟*/      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
    USART_DeInit(USART3);  //复位串口3
    //PB10  TXD3  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		// 复用推免输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PB11 RXD3  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉输入
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//Usart NVIC 配置 
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);     //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART2
   //USART 初始化设置    
    USART_InitStructure.USART_BaudRate = bound;         //一般设置为9600;       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //一个停止位 
    USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //收发模式     
    USART_Init(USART3, &USART_InitStructure);//初始化串口

	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//开启串口3接受中断
    //USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //开启串口3发送中断  
    USART_Cmd(USART3, ENABLE); 					 //使能串口3   
}
	#if !EN_USART_code_key  /* 数据包的形式接收 */
u8 USART3_RX_head=0;		//接收状态标记 --  数据头
u8 USART3_RX_num=0;		//接收状态标记 --  数据计数
u8 USART3_RX_len=0;		//接收状态标记 --  数据包裹长度
void USART3_IRQHandler(void)                	//串口3中断服务程序  --  数据包的形式接收
{
    if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET) //中断产生   
    {   
        USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除中断标志  
        USART3_RX_BUF[USART3_RX_num] = USART_ReceiveData(USART3);   
        USART3_RX_num++;   
        USART3_RX_num &= 0xFF;   
    }   
    if(USART3_RX_BUF[USART3_RX_num-1] == 0xff) USART3_RX_head = USART3_RX_num-1;   //头   
    
    if( (USART3_RX_BUF[USART3_RX_head] == 0xff)&&(USART3_RX_BUF[USART3_RX_num-1] == 0xee) ) //检测到头的情况下检测到尾   
    {   
        USART3_RX_len = USART3_RX_num-1- USART3_RX_head; //长度   
        USART3_RX_num = 0; //初始化   
        USART3_RX_head = 0;   
    }   
    if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET) //溢出   
    {   
        USART_ClearFlag(USART3,USART_FLAG_ORE); //读SR   
        USART_ReceiveData(USART3); //读DR   
		memset(USART3_RX_BUF,0,USART3_REC_LEN); //串口接收缓冲区 清 0
    } 
/* code ...  */	
}
//	#else 
u16 USART3_RX_STA=0;       //接收状态标记	  
void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART3);	//读取接收到的数据		
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
    } 
} 
	#endif	
	
#endif	

/***********************************************^^^^^^^^^^^^^^^^^^^^*****************************************/
//void USART_Send_String(USART_TypeDef* USARTx,char *pucString);

/*接收一个字节数据*/
unsigned char UARTGetByte(USART_TypeDef* USARTx,unsigned char* Get_Data)
{   	   
    if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET) return 1;//没有收到数据 
    *Get_Data = USART_ReceiveData(USARTx); 
    return 0;//收到数据
}

/*  TXE方式 ：发送一个字节数据*/
void UARTSendByte_TXE(USART_TypeDef* USARTx,unsigned char SendData)
{	   
	USART_SendData(USARTx,SendData);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	    
} 
void USART_Send_String_TXE(USART_TypeDef* USARTx,char *pucString)
{
   while(*pucString!='\0')
   {
       UARTSendByte_TXE(USARTx,*pucString);
	   pucString++;
   }
}

/*  TC方式 ：发送一个字节数据*/
void UARTSendByte_TC(USART_TypeDef* USARTx,unsigned char SendData)
{	   
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);	    
	USART_SendData(USARTx,SendData);
} 
void USART_Send_String_TC(USART_TypeDef* USARTx,char *pucString)
{
   while(*pucString!='\0')
   {
       UARTSendByte_TC(USARTx,*pucString);
	   pucString++;
   }
}

#if 0
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )//  该函数就可以像printf使用可变参数，方便很多
{
	const char *s;
	int d;   
	char buf[16];
	va_list ap;
	va_start(ap, Data);
	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
				break;
				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
				break;
				default:
					Data ++;
				break;
			}			 
		}
		else if ( * Data == '%')
			{									  //
				switch ( *++Data )
				{				
					case 's':										  //字符串
						s = va_arg(ap, const char *);
						for ( ; *s; s++) 
						{
							USART_SendData(USARTx,*s);
							while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
						}
						Data++;
					break;
					case 'd':						
						d = va_arg(ap, int); //十进制		
						itoa(d, buf, 10);			
						for (s = buf; *s; s++) 
						{
							USART_SendData(USARTx,*s);
							while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
						}
						Data++;
					break;
					default:
						Data++;
					break;
				}		 
			}
		else USART_SendData(USARTx, *Data++);
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );
	}
}
int itoa(int num, char *str, int radix)
{
	char string[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* ptr = str;
	int temp;
	int i;
	int j;        
	if (num==0)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return 1;
	}
	if(num < 0)
	{
		*ptr++ = '-';
		num *= -1;
	}
	while (num)
	{
		*ptr++ = string[num % radix];
		num /= radix;
		if (num < radix)
		{
				*ptr++ = string[num];
				*ptr = '\0';
				break;
		}
	}
	j = ptr - str - 1;
	for (i = 0; i < (ptr - str) / 2; i++)
	{
		temp = str[i];
		str[i] = str[j];
		str[j--] = temp;
	}
	return (int)(ptr - str);
}
#endif	

void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	while (len--) 
	{
		while ((USART3->SR & 0x40) == 0);
		USART_SendData(USART3,*buf++);
	}
}
