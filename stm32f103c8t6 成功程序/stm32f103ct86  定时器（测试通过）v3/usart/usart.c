//**************
/***************STM32F103C8T6**********************
 * �ļ���  ��usart1.c
 * ����    : uart����
 * ʵ��ƽ̨��STM32F103C8T6
 * ��ע    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ�������
 * �ӿ�    ��PA9--TX  PA10--RX

****************STM32F103C8T6**********************/

#include "usart.h"

/*���罫����2��ӳ��ʱ�����ǣ�
AFIO->MAPR|=GPIO_Remap_USART2;
Ҫȡ����ӳ�䣬���ǣ�
AFIO->MAPR&=~GPIO_Remap_USART2;
GPIO_Remap_USART2��stm32f10x_GPIO.h�����ж���ģ�AFIO->MAPR�Ĵ�������λ�Ķ����ڲο��ֲ�������ϸ������*/

u8 USART_RX_BUF1[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART_RX_BUF2[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u8  Uart1_Buffer[USART_REC_LEN];   
u8  Uart2_Buffer[USART_REC_LEN];   

u8 USART1_led=0; //���յ�USART1   ����   ����
u8 USART2_led=0;

u16 USART_RX_STA=0,Uart1_Rx=0,Uart1_Tx=0;       //����״̬���	  
u16 num_1=0;
u16 Uart1_Len=0; //����   
u16 Uart1_Sta=0; //��־λ   
u16 tx1=0;

u16 Uart2_Rx=0,Uart2_Tx=0;       //����״̬���	  
u16 num_2=0;
u16 Uart2_Len=0; //����   
u16 Uart2_Sta=0; //��־λ   
u16 tx2=0;

//////////////////////////////
void USART1_init(u32 bound)
{
    //GPIO�˿�����
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

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);    //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

   //USART ��ʼ������    
    USART_InitStructure.USART_BaudRate = bound;         //һ������Ϊ9600;       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //һ��ֹͣλ 
    USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //�շ�ģʽ     
    USART_Init(USART1, &USART_InitStructure);//��ʼ������
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�  
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   //���������ж�  
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���
}
//******
void USART2_init( u32 bound )
{
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO�˿�����
    NVIC_InitTypeDef NVIC_InitStructure;//�����ж�  NVIC�˿�����
    USART_InitTypeDef USART_InitStructure;//USART 1-3�˿�����
 
    /*��������ʱ��*/      
    /* Enable the USART2 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 
    
    //PA3 RXD2  
    /* Configure USART2 Rx (PA.03) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART_DeInit(USART2);  //��λ����2

    //PA2 TXD2  
    /* Configure USART2 Tx (PA.02) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* Enable the USART2 Interrupt */ //Usart2 NVIC ���� 
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);    	//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 

   //USART ��ʼ������    
    USART_InitStructure.USART_BaudRate = bound;         //һ������Ϊ9600;       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //һ��ֹͣλ 
    USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //�շ�ģʽ     
    USART_Init(USART2, &USART_InitStructure);//��ʼ������
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���������ж�  
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   //���������ж�  
    USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���     /* Enable USART2 */
}

//******
void USART3_init( u32 bound )
{
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO�˿�����
//    NVIC_InitTypeDef NVIC_InitStructure;//�����ж�  NVIC�˿�����
    USART_InitTypeDef USART_InitStructure;//USART 1-3�˿�����
 
    /*��������ʱ��*/      
    /* Enable the USART3 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
    
    //PB11 RXD3  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART_DeInit(USART3);  //��λ����3

    //PB10  TXD3  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* Enable the USART3 Interrupt */ //Usart3 NVIC ���� 
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�2
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
//    NVIC_Init(&NVIC_InitStructure);    	//����ָ���Ĳ�����ʼ��VIC�Ĵ��� 

   //USART ��ʼ������    
    USART_InitStructure.USART_BaudRate = bound;         //һ������Ϊ9600;       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //һ��ֹͣλ 
    USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //�շ�ģʽ     
    USART_Init(USART3, &USART_InitStructure);//��ʼ������
    
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//���������ж�  
//    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);   //���������ж�  
    USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���     /* Enable USART3 */
}



//�жϷ������**************
void USART1_IRQHandler(void)//����1�жϷ������                   
{   
/*  ���ݲ������ݰ�����ʽ���գ����պ����ڻ�������ͨ���ж�  ����ͷ������β���ɱ䣩 
    ���ж����ݵġ���������Ч�ԣ��жϺ������ڽ������ݺ�
    �ж�ͷβ�Լ����ݰ����ȣ���һ������main������ִ�У������͸ö����ݡ�
�ŵ㣺�ʺϴ�����䣬�ȶ��ԺͿɿ��Ժ��б�֤�������ⷢ�ͣ��Զ���ѡ��Ч���ݡ�
ȱ�㣺���������ݳ���Ҫ���ݡ������������趨�� Ҫ�Ƕ�ν��պ���ͷ��β��
    ����ͷ��β����һ������ǡ�ÿ�Խ��������ǰ�����λ��ʱ�����ܵ��±������ݶ�ʧ��
    ���������������û�п��ܡ�*/    
    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //�жϲ���   
    {   
        USART_ClearITPendingBit(USART1,USART_IT_RXNE); //����жϱ�־  
        USART_RX_BUF1[Uart1_Rx] = USART_ReceiveData(USART1);   
        Uart1_Rx++;   
        Uart1_Rx &= 0xFF;   
    }   
    if(USART_RX_BUF1[Uart1_Rx-1] == 0xff) //ͷ   
        Uart1_Tx = Uart1_Rx-1;   
    
    if( (USART_RX_BUF1[Uart1_Tx] == 0xff)&&(USART_RX_BUF1[Uart1_Rx-1] == 0xee) ) //��⵽ͷ������¼�⵽β   
    {   
        USART1_led=1; 
        Uart1_Len = Uart1_Rx-1- Uart1_Tx; //����   
        Uart1_Sta=1; //��־λ   
    }   
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET) //���   
    {   
        USART_ClearFlag(USART1,USART_FLAG_ORE); //��SR   
        USART_ReceiveData(USART1); //��DR   
    }      
    if(Uart1_Sta) 
    {   
        for(tx1=0;tx1 <= Uart1_Len;tx1++,Uart1_Tx++)   
        UARTSendByte(USART2,USART_RX_BUF1[Uart1_Tx]); //��������   

        Uart1_Rx = 0; //��ʼ��   
        Uart1_Tx = 0;   
        Uart1_Sta = 0;   
    }  
//*********************************************************
//     if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)  
//        {      
//            USART_ClearFlag(USART1,USART_FLAG_RXNE); //��SR   
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
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�����ж�(���յ������ݱ�����0x0d 0x0a��β) 
//    {
//        if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//        {
//            Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������

//            if((USART_RX_STA&0x8000)==0)//����δ���
//            {
//                if(USART_RX_STA&0x4000)//���յ���0x0d
//                {
//                    if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//                    else USART_RX_STA|=0x8000;	//��������� 
//                }
//                else //��û�յ�0X0D
//                {	
//                    if(Res==0x0d)USART_RX_STA|=0x4000;
//                    else
//                    {
//                        USART_RX_BUF1[USART_RX_STA&0X3FFF]=Res ;
//                        USART_RX_STA++;
//                        if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//                    }		 
//                }
//             }   		 
//        } 
//    }
//*********************************************************************
}


//**+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void USART2_IRQHandler(void)   //����2�жϷ������
{  
//     if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
//        {       
//            USART_ClearFlag(USART2,USART_FLAG_RXNE);    
//            USART_SendData(USART2, USART_ReceiveData(USART2));             
//        } 
    
//****************************************************************************************
/*  ���ݲ������ݰ�����ʽ���գ����պ����ڻ�������ͨ���ж�  ����ͷ������β���ɱ䣩 
    ���ж����ݵġ���������Ч�ԣ��жϺ������ڽ������ݺ�
    �ж�ͷβ�Լ����ݰ����ȣ���һ������main������ִ�У������͸ö����ݡ�
�ŵ㣺�ʺϴ�����䣬�ȶ��ԺͿɿ��Ժ��б�֤�������ⷢ�ͣ��Զ���ѡ��Ч���ݡ�
ȱ�㣺���������ݳ���Ҫ���ݡ������������趨�� Ҫ�Ƕ�ν��պ���ͷ��β��
    ����ͷ��β����һ������ǡ�ÿ�Խ��������ǰ�����λ��ʱ�����ܵ��±������ݶ�ʧ��
    ���������������û�п��ܡ�*/    
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //�жϲ���   
    {   
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����жϱ�־  
        USART_RX_BUF2[Uart2_Rx] = USART_ReceiveData(USART2);   
        Uart2_Rx++;   
        Uart2_Rx &= 0xFF;   
    }   
    if(USART_RX_BUF2[Uart2_Rx-1] == 0xff) //ͷ   
        Uart2_Tx = Uart2_Rx-1;   
    
    if( (USART_RX_BUF2[Uart2_Tx] == 0xff)&&(USART_RX_BUF2[Uart2_Rx-1] == 0xee) ) //��⵽ͷ������¼�⵽β   
    {   
        USART2_led=1; 
        Uart2_Len = Uart2_Rx-1- Uart2_Tx; //����   
        Uart2_Sta=1; //��־λ   
    }   
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //���   
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR   
        USART_ReceiveData(USART2); //��DR   
    }      
    if(Uart2_Sta) 
    {   
        for(tx2=0;tx2 <= Uart2_Len;tx2++,Uart2_Tx++)   
        UARTSendByte(USART3,USART_RX_BUF2[Uart2_Tx]); //��������   

        Uart2_Rx = 0; //��ʼ��   
        Uart2_Tx = 0;   
        Uart2_Sta = 0;   
    }  

} 
//**+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void USART3_IRQHandler(void)   //����3�жϷ������
{  
     if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)  
        {       
              USART_ClearFlag(USART3,USART_FLAG_RXNE);    
              USART_SendData(USART3, USART_ReceiveData(USART3));             
        }   
} 
/* ����posix��׼��һ��������Ӧ��*_t����Ϊ��
1�ֽ�     uint8_t
2�ֽ�     uint16_t
4�ֽ�     uint32_t
8�ֽ�     uint64_t*/

 /*����һ���ֽ�����*/
void UARTSendByte(USART_TypeDef* USARTx,unsigned char SendData)
{	   
        USART_SendData(USARTx,SendData);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	    
} 

/*����һ���ֽ�����*/
unsigned char UARTGetByte(USART_TypeDef* USARTx,unsigned char* GetData)
{   	   
    if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET) return 0;//û���յ����� 
    *GetData = USART_ReceiveData(USARTx); 
    return 1;//�յ�����
}

void USART_Send_String(USART_TypeDef* USARTx,char *pucString)
{
   while(*pucString!='\0')
   {
       UARTSendByte(USARTx,*pucString);
	   pucString++;
   }
}

 /* ����  ���ض���c�⺯��printf��USART1*/ 
int fputc(int ch, FILE *f)
{
/* ��Printf���ݷ������� */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

void USART_text(void)
{
    u8 len=0,t=0;
    if(USART_RX_STA&0x8000)
    {					   
        len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
        printf("\r\n�����͵���ϢΪ:\r\n\r\n");
        for(t=0;t<len;t++)
        {
            USART_SendData(USART1, USART_RX_BUF1[t]);//�򴮿�1��������
            while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
        }
        printf("\r\n\r\n");//���뻻��
        printf("\r\n%d\r\n\r\n",len);
        USART_RX_STA=0;
    }else
    {
            printf("\r\nս��STM32������ ����ʵ��\r\n");
            printf("����ԭ��@ALIENTEK\r\n\r\n");
    }
}

/*STM32�����жϽ��շ�ʽ��ϸ�Ƚ�
2014��10��17�� 14:54:55
�Ķ�����9925
������ͨ��PC���Ĵ��ڵ������ֽ����ݷ�����STM32��STM32ͨ��SP3232оƬ�����жϽ��շ�ʽ��ɣ�Ȼ��������ݺ������յ������ַ�����PC����
ʵ��һ��

[c] view plain copy
void USART1_IRQHandler(u8 GetData)  
{   
    u8 BackData;  
  
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�жϲ���  
    {    
        USART_ClearITPendingBit(USART1,USART_IT_RXNE); //����жϱ�־.  
        GetData = UART1_GetByte(BackData);   //Ҳ��GetData=USART1->DR;     
        USART1_SendByte(GetData);      //��������  
  
        GPIO_SetBits(GPIOE, GPIO_Pin_8 ); //LED��˸�����ճɹ��������  
        delay(1000);  
        GPIO_ResetBits(GPIOE, GPIO_Pin_8 );   
    }   
}    
����������ģ������ݽ�����ɺ��ַ��ͳ�ȥ�����պͷ������жϺ�����ִ�У�main������������Ҫ����ġ�

�ŵ㣺�򵥣��ʺϺ��������ݴ��䡣
ȱ�㣺�޻����������Ҷ����ݵ���ȷ��û���жϣ��������Դ���ܵ������ݶ�ʧ ��
ʵ������ 
[c] view plain copy
void USART2_IRQHandler()    
{   
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //�жϲ���  
    {    
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����жϱ�־  
        USART_RX_BUF1[Uart2_Rx_Num] = USART_ReceiveData(USART2);  
        Uart2_Rx_Num++;   
    }  
  
    if((USART_RX_BUF1[0] == 0x5A)&&(USART_RX_BUF1[Uart2_Rx_Num-1] == 0xA5))  //�ж������յ������Ƿ�Ϊ�趨ֵ��ȷ��������ȷ��  
        Uart1_Sta=1;  
      
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //���  
    {  
        USART_ClearFlag(USART2,USART_FLAG_ORE);  //��SR  
        USART_ReceiveData(USART2); //��DR    
    }       
}  
     
if( Uart1_Sta )   
{  
    for(Uart2_Tx_Num=0;Uart2_Tx_Num < Uart2_Rx_Num;Uart2_Tx_Num++)   
        USART2_SendByte(USART_RX_BUF1[Uart2_Tx_Num]); //��������  
  
    Uart2_Rx_Num = 0; //��ʼ��  
    Uart2_Tx_Num = 0;  
    Uart1_Sta = 0;  
}  
���Ǽ�������ͷ������β�Ľ��շ�ʽ������ͷ��β�ĸ��������ӣ��˴�ֻ���ڵ���֮�á��жϺ������ڽ��������Լ��ж����ݵ�ͷβ���ڶ���������main�����ﰴ�ղ�ѯ��ʽִ�С�
�ŵ㣺�ϼ򵥣����û��������գ���������ݵ���ȷ����һ���ĸ��� ��
ȱ�㣺Ҫ�ǵ�һ�����ݽ��մ��󣬻ز�����ʼ��״̬�����븴λ���� ��
ʵ������

[c] view plain copy
void USART2_IRQHandler()   
{   
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //�жϲ���   
    {   
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����жϱ�־.   
        USART_RX_BUF1[USART_RX_STA] = USART_ReceiveData(USART2);   
        USART_RX_STA++;   
        USART_RX_STA &= 0x3F; //�ж��Ƿ���������  
    }   
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //���   
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR   
        USART_ReceiveData(USART2); //��DR   
    }   
}  
    
if( Uart1_Tx != USART_RX_STA )   
{      
    USART2_SendByte(USART_RX_BUF1[Uart1_Tx]); //��������   
    Uart1_Tx++;   
    Uart1_Tx &= 0x3F; //�ж��Ƿ���������  
}    
����FIFO��ʽ�������ݣ���0x3F��֪�˴���������Ϊ64�����ɱ䣬�жϺ���ֻ�����գ���һ������main������ִ�У�FIFO��ʽ���͡�
�ŵ㣺���ͺͽ��ն������ɣ��ж�ռ��ʱ���٣�������MCU����������
ȱ�㣺�����ݵ���ȷ��û���жϣ�һ��ȫ�����ա�
ʵ���ģ� 
[c] view plain copy
void USART2_IRQHandler()   
{   
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //�жϲ���   
    {   
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����жϱ�־  
        USART_RX_BUF1[USART_RX_STA] = USART_ReceiveData(USART2);   
        USART_RX_STA++;   
        USART_RX_STA &= 0xFF;   
    }   
    if(USART_RX_BUF1[USART_RX_STA-1] == 0x5A) //ͷ   
        Uart1_Tx = USART_RX_STA-1;   
    if((USART_RX_BUF1[Uart1_Tx] == 0x5A)&&(USART_RX_BUF1[USART_RX_STA-1] == 0xA5)) //��⵽ͷ������¼�⵽β   
    {   
        Uart1_Len = USART_RX_STA-1- Uart1_Tx; //����   
        Uart1_Sta=1; //��־λ   
    }   
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //���   
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR   
        USART_ReceiveData(USART2); //��DR   
    }   
}   
  
if( Uart1_Sta )   
{   
    for(tx2=0;tx2 <= Uart1_Len;tx2++,Uart1_Tx++)   
        USART2_SendByte(USART_RX_BUF1[Uart1_Tx]); //��������   
          
        USART_RX_STA = 0; //��ʼ��   
        Uart1_Tx = 0;   
        Uart1_Sta = 0;   
}  
���ݲ������ݰ�����ʽ���գ����պ����ڻ�������ͨ���ж�����ͷ������β���ɱ䣩���ж����ݵġ���������Ч�ԣ��жϺ������ڽ������ݺ��ж�ͷβ�Լ����ݰ����ȣ���һ������main������ִ�У������͸ö����ݡ�
�ŵ㣺�ʺϴ�����䣬�ȶ��ԺͿɿ��Ժ��б�֤�������ⷢ�ͣ��Զ���ѡ��Ч���ݡ�
ȱ�㣺���������ݳ���Ҫ���ݡ������������趨�� Ҫ�Ƕ�ν��պ���ͷ��β������ͷ��β����һ������ǡ�ÿ�Խ��������ǰ�����λ��ʱ�����ܵ��±������ݶ�ʧ�����������������û�п���*/
