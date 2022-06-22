//**************
/***************STM32F103C8T6**********************
 * 文件名  ：usart1.c
 * 描述    : uart串口
 * 实验平台：STM32F103C8T6
 * 备注    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手
 * 接口    ：PA9--TX  PA10--RX

****************STM32F103C8T6**********************/

#include "usart.h"

/*比如将串口2重映射时，就是：
AFIO->MAPR|=GPIO_Remap_USART2;
要取消重映射，就是：
AFIO->MAPR&=~GPIO_Remap_USART2;
GPIO_Remap_USART2在stm32f10x_GPIO.h中是有定义的，AFIO->MAPR寄存器各个位的定义在参考手册中有详细描述。*/

u8 USART_RX_BUF1[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART_RX_BUF2[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u8  Uart1_Buffer[USART_REC_LEN];   
u8  Uart2_Buffer[USART_REC_LEN];   

u8 USART1_led=0; //接收到USART1   数据   灯闪
u8 USART2_led=0;

u16 USART_RX_STA=0,Uart1_Rx=0,Uart1_Tx=0;       //接收状态标记	  
u16 num_1=0;
u16 Uart1_Len=0; //长度   
u16 Uart1_Sta=0; //标志位   
u16 tx1=0;

u16 Uart2_Rx=0,Uart2_Tx=0;       //接收状态标记	  
u16 num_2=0;
u16 Uart2_Len=0; //长度   
u16 Uart2_Sta=0; //标志位   
u16 tx2=0;

//////////////////////////////
void USART1_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX      PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1

   //USART 初始化设置    
    USART_InitStructure.USART_BaudRate = bound;         //一般设置为9600;       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //一个停止位 
    USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //收发模式     
    USART_Init(USART1, &USART_InitStructure);//初始化串口
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收中断  
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   //开启发送中断  
    USART_Cmd(USART1, ENABLE);  //使能串口
}
//******
void USART2_init( u32 bound )
{
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO端口设置
    NVIC_InitTypeDef NVIC_InitStructure;//开启中断  NVIC端口设置
    USART_InitTypeDef USART_InitStructure;//USART 1-3端口设置
 
    /*开启外设时钟*/      
    /* Enable the USART2 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
    
    //PA3 RXD2  
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART_DeInit(USART2);  //复位串口2

    //PA2 TXD2  
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Enable the USART2 Interrupt */ //Usart2 NVIC 配置 
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    	//根据指定的参数初始化VIC寄存器 

   //USART 初始化设置    
    USART_InitStructure.USART_BaudRate = bound;         //一般设置为9600;       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //一个停止位 
    USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //收发模式     
    USART_Init(USART2, &USART_InitStructure);//初始化串口
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启接收中断  
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   //开启发送中断  
    USART_Cmd(USART2, ENABLE);  //使能串口     /* Enable USART2 */
}

//******
void USART3_init( u32 bound )
{
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO端口设置
//    NVIC_InitTypeDef NVIC_InitStructure;//开启中断  NVIC端口设置
    USART_InitTypeDef USART_InitStructure;//USART 1-3端口设置
 
    /*开启外设时钟*/      
    /* Enable the USART3 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
    
    //PB11 RXD3  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART_DeInit(USART3);  //复位串口3

    //PB10  TXD3  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Enable the USART3 Interrupt */ //Usart3 NVIC 配置 
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//子优先级2
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
//    NVIC_Init(&NVIC_InitStructure);    	//根据指定的参数初始化VIC寄存器 

   //USART 初始化设置    
    USART_InitStructure.USART_BaudRate = bound;         //一般设置为9600;       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //一个停止位 
    USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //收发模式     
    USART_Init(USART3, &USART_InitStructure);//初始化串口
    
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启接收中断  
//    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);   //开启发送中断  
    USART_Cmd(USART3, ENABLE);  //使能串口     /* Enable USART3 */
}



//中断服务程序**************
void USART1_IRQHandler(void)//串口1中断服务程序                   
{   
/*  数据采用数据包的形式接收，接收后存放于缓存区，通过判断  数据头和数据尾（可变） 
    来判断数据的“包”及有效性，中断函数用于接收数据和
    判断头尾以及数据包长度，另一函数在main函数里执行，负责发送该段数据。
优点：适合打包传输，稳定性和可靠性很有保证，可随意发送，自动挑选有效数据。
缺点：缓存区数据长度要根据“包裹”长度设定， 要是多次接收后无头无尾，
    到有头有尾的那一段数据恰好跨越缓存区最前和最后位置时，可能导致本次数据丢失，
    不过这种情况几乎没有可能。*/    
    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //中断产生   
    {   
        USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志  
        USART_RX_BUF1[Uart1_Rx] = USART_ReceiveData(USART1);   
        Uart1_Rx++;   
        Uart1_Rx &= 0xFF;   
    }   
    if(USART_RX_BUF1[Uart1_Rx-1] == 0xff) //头   
        Uart1_Tx = Uart1_Rx-1;   
    
    if( (USART_RX_BUF1[Uart1_Tx] == 0xff)&&(USART_RX_BUF1[Uart1_Rx-1] == 0xee) ) //检测到头的情况下检测到尾   
    {   
        USART1_led=1; 
        Uart1_Len = Uart1_Rx-1- Uart1_Tx; //长度   
        Uart1_Sta=1; //标志位   
    }   
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET) //溢出   
    {   
        USART_ClearFlag(USART1,USART_FLAG_ORE); //读SR   
        USART_ReceiveData(USART1); //读DR   
    }      
    if(Uart1_Sta) 
    {   
        for(tx1=0;tx1 <= Uart1_Len;tx1++,Uart1_Tx++)   
        UARTSendByte(USART2,USART_RX_BUF1[Uart1_Tx]); //发送数据   

        Uart1_Rx = 0; //初始化   
        Uart1_Tx = 0;   
        Uart1_Sta = 0;   
    }  
//*********************************************************
//     if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)  
//        {      
//            USART_ClearFlag(USART1,USART_FLAG_RXNE); //读SR   
//            USART_RX_BUF1[USART_RX_STA]=USART_ReceiveData(USART1);
//           
//            if(USART_RX_BUF1[USART_RX_STA] == 0xff)  PBout(8)=1; 

////////                /*&& USART_RX_BUF1[USART_RX_STA+1]==0x46 &&  
////////            USART_RX_BUF1[USART_RX_STA+12]==0x45 && USART_RX_BUF1[USART_RX_STA+13]==0x45 ) //FF ** ** ** EE    
////////*/
////////            USART_SendData(USART2,  USART_RX_BUF1[USART_RX_STA]);     
////////            USART_RX_STA++;   
////////           
////////        }   
//*********************************************************
//        u8 Res;
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收中断(接收到的数据必须是0x0d 0x0a结尾) 
//    {
//        if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//        {
//            Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据

//            if((USART_RX_STA&0x8000)==0)//接收未完成
//            {
//                if(USART_RX_STA&0x4000)//接收到了0x0d
//                {
//                    if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//                    else USART_RX_STA|=0x8000;	//接收完成了 
//                }
//                else //还没收到0X0D
//                {	
//                    if(Res==0x0d)USART_RX_STA|=0x4000;
//                    else
//                    {
//                        USART_RX_BUF1[USART_RX_STA&0X3FFF]=Res ;
//                        USART_RX_STA++;
//                        if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
//                    }		 
//                }
//             }   		 
//        } 
//    }
//*********************************************************************
}


//**+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void USART2_IRQHandler(void)   //串口2中断服务程序
{  
//     if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
//        {       
//            USART_ClearFlag(USART2,USART_FLAG_RXNE);    
//            USART_SendData(USART2, USART_ReceiveData(USART2));             
//        } 
    
//****************************************************************************************
/*  数据采用数据包的形式接收，接收后存放于缓存区，通过判断  数据头和数据尾（可变） 
    来判断数据的“包”及有效性，中断函数用于接收数据和
    判断头尾以及数据包长度，另一函数在main函数里执行，负责发送该段数据。
优点：适合打包传输，稳定性和可靠性很有保证，可随意发送，自动挑选有效数据。
缺点：缓存区数据长度要根据“包裹”长度设定， 要是多次接收后无头无尾，
    到有头有尾的那一段数据恰好跨越缓存区最前和最后位置时，可能导致本次数据丢失，
    不过这种情况几乎没有可能。*/    
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //中断产生   
    {   
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志  
        USART_RX_BUF2[Uart2_Rx] = USART_ReceiveData(USART2);   
        Uart2_Rx++;   
        Uart2_Rx &= 0xFF;   
    }   
    if(USART_RX_BUF2[Uart2_Rx-1] == 0xff) //头   
        Uart2_Tx = Uart2_Rx-1;   
    
    if( (USART_RX_BUF2[Uart2_Tx] == 0xff)&&(USART_RX_BUF2[Uart2_Rx-1] == 0xee) ) //检测到头的情况下检测到尾   
    {   
        USART2_led=1; 
        Uart2_Len = Uart2_Rx-1- Uart2_Tx; //长度   
        Uart2_Sta=1; //标志位   
    }   
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //溢出   
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR   
        USART_ReceiveData(USART2); //读DR   
    }      
    if(Uart2_Sta) 
    {   
        for(tx2=0;tx2 <= Uart2_Len;tx2++,Uart2_Tx++)   
        UARTSendByte(USART3,USART_RX_BUF2[Uart2_Tx]); //发送数据   

        Uart2_Rx = 0; //初始化   
        Uart2_Tx = 0;   
        Uart2_Sta = 0;   
    }  

} 
//**+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void USART3_IRQHandler(void)   //串口3中断服务程序
{  
     if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)  
        {       
              USART_ClearFlag(USART3,USART_FLAG_RXNE);    
              USART_SendData(USART3, USART_ReceiveData(USART3));             
        }   
} 
/* 依照posix标准，一般整形相应的*_t类型为：
1字节     uint8_t
2字节     uint16_t
4字节     uint32_t
8字节     uint64_t*/

 /*发送一个字节数据*/
void UARTSendByte(USART_TypeDef* USARTx,unsigned char SendData)
{	   
        USART_SendData(USARTx,SendData);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	    
} 

/*接收一个字节数据*/
unsigned char UARTGetByte(USART_TypeDef* USARTx,unsigned char* GetData)
{   	   
    if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET) return 0;//没有收到数据 
    *GetData = USART_ReceiveData(USARTx); 
    return 1;//收到数据
}

void USART_Send_String(USART_TypeDef* USARTx,char *pucString)
{
   while(*pucString!='\0')
   {
       UARTSendByte(USARTx,*pucString);
	   pucString++;
   }
}

 /* 描述  ：重定向c库函数printf到USART1*/ 
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

void USART_text(void)
{
    u8 len=0,t=0;
    if(USART_RX_STA&0x8000)
    {					   
        len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
        printf("\r\n您发送的消息为:\r\n\r\n");
        for(t=0;t<len;t++)
        {
            USART_SendData(USART1, USART_RX_BUF1[t]);//向串口1发送数据
            while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
        }
        printf("\r\n\r\n");//插入换行
        printf("\r\n%d\r\n\r\n",len);
        USART_RX_STA=0;
    }else
    {
            printf("\r\n战舰STM32开发板 串口实验\r\n");
            printf("正点原子@ALIENTEK\r\n\r\n");
    }
}

/*STM32串口中断接收方式详细比较
2014年10月17日 14:54:55
阅读数：9925
本例程通过PC机的串口调试助手将数据发送至STM32，STM32通过SP3232芯片采用中断接收方式完成，然后接收数据后将所接收的数据又发送至PC机。
实例一：

[c] view plain copy
void USART1_IRQHandler(u8 GetData)  
{   
    u8 BackData;  
  
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //中断产生  
    {    
        USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清除中断标志.  
        GetData = UART1_GetByte(BackData);   //也行GetData=USART1->DR;     
        USART1_SendByte(GetData);      //发送数据  
  
        GPIO_SetBits(GPIOE, GPIO_Pin_8 ); //LED闪烁，接收成功发送完成  
        delay(1000);  
        GPIO_ResetBits(GPIOE, GPIO_Pin_8 );   
    }   
}    
这是最基本的，将数据接收完成后又发送出去，接收和发送在中断函数里执行，main函数里无其他要处理的。

优点：简单，适合很少量数据传输。
缺点：无缓存区，并且对数据的正确性没有判断，数据量稍大可能导致数据丢失 。
实例二： 
[c] view plain copy
void USART2_IRQHandler()    
{   
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //中断产生  
    {    
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志  
        USART_RX_BUF1[Uart2_Rx_Num] = USART_ReceiveData(USART2);  
        Uart2_Rx_Num++;   
    }  
  
    if((USART_RX_BUF1[0] == 0x5A)&&(USART_RX_BUF1[Uart2_Rx_Num-1] == 0xA5))  //判断最后接收的数据是否为设定值，确定数据正确性  
        Uart1_Sta=1;  
      
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //溢出  
    {  
        USART_ClearFlag(USART2,USART_FLAG_ORE);  //读SR  
        USART_ReceiveData(USART2); //读DR    
    }       
}  
     
if( Uart1_Sta )   
{  
    for(Uart2_Tx_Num=0;Uart2_Tx_Num < Uart2_Rx_Num;Uart2_Tx_Num++)   
        USART2_SendByte(USART_RX_BUF1[Uart2_Tx_Num]); //发送数据  
  
    Uart2_Rx_Num = 0; //初始化  
    Uart2_Tx_Num = 0;  
    Uart1_Sta = 0;  
}  
这是加了数据头和数据尾的接收方式，数据头和尾的个数可增加，此处只用于调试之用。中断函数用于接收数据以及判断数据的头尾，第二个函数在main函数里按照查询方式执行。
优点：较简单，采用缓存区接收，对提高数据的正确行有一定的改善 。
缺点：要是第一次数据接收错误，回不到初始化状态，必须复位操作 。
实例三：

[c] view plain copy
void USART2_IRQHandler()   
{   
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //中断产生   
    {   
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志.   
        USART_RX_BUF1[USART_RX_STA] = USART_ReceiveData(USART2);   
        USART_RX_STA++;   
        USART_RX_STA &= 0x3F; //判断是否计数到最大  
    }   
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //溢出   
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR   
        USART_ReceiveData(USART2); //读DR   
    }   
}  
    
if( Uart1_Tx != USART_RX_STA )   
{      
    USART2_SendByte(USART_RX_BUF1[Uart1_Tx]); //发送数据   
    Uart1_Tx++;   
    Uart1_Tx &= 0x3F; //判断是否计数到最大  
}    
采用FIFO方式接收数据，由0x3F可知此处最大接收量为64个，可变，中断函数只负责收，另一函数在main函数里执行，FIFO方式发送。
优点：发送和接收都很自由，中断占用时间少，有利于MCU处理其它。
缺点：对数据的正确性没有判断，一概全部接收。
实例四： 
[c] view plain copy
void USART2_IRQHandler()   
{   
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //中断产生   
    {   
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //清除中断标志  
        USART_RX_BUF1[USART_RX_STA] = USART_ReceiveData(USART2);   
        USART_RX_STA++;   
        USART_RX_STA &= 0xFF;   
    }   
    if(USART_RX_BUF1[USART_RX_STA-1] == 0x5A) //头   
        Uart1_Tx = USART_RX_STA-1;   
    if((USART_RX_BUF1[Uart1_Tx] == 0x5A)&&(USART_RX_BUF1[USART_RX_STA-1] == 0xA5)) //检测到头的情况下检测到尾   
    {   
        Uart1_Len = USART_RX_STA-1- Uart1_Tx; //长度   
        Uart1_Sta=1; //标志位   
    }   
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //溢出   
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR   
        USART_ReceiveData(USART2); //读DR   
    }   
}   
  
if( Uart1_Sta )   
{   
    for(tx2=0;tx2 <= Uart1_Len;tx2++,Uart1_Tx++)   
        USART2_SendByte(USART_RX_BUF1[Uart1_Tx]); //发送数据   
          
        USART_RX_STA = 0; //初始化   
        Uart1_Tx = 0;   
        Uart1_Sta = 0;   
}  
数据采用数据包的形式接收，接收后存放于缓存区，通过判断数据头和数据尾（可变）来判断数据的“包”及有效性，中断函数用于接收数据和判断头尾以及数据包长度，另一函数在main函数里执行，负责发送该段数据。
优点：适合打包传输，稳定性和可靠性很有保证，可随意发送，自动挑选有效数据。
缺点：缓存区数据长度要根据“包裹”长度设定， 要是多次接收后无头无尾，到有头有尾的那一段数据恰好跨越缓存区最前和最后位置时，可能导致本次数据丢失，不过这种情况几乎没有可能*/
