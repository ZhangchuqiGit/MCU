#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
/*���ʹ��ucos,����������ͷ�ļ�����.   */
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#include "semphr.h"	 
#endif

/*******-----------******+++++++++++++++++++++++++++*******------------*****/
#if 1   //�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����  
/* �ض���c�⺯��printf��USART1*/ 
int fputc(int ch, FILE *f)
{      
	/* ��Printf���ݷ������� */
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}
#else
/*ʹ��microLib�ķ���*/
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
/************************************************** ����1 *********************************************************/
#if EN_USART1_RX   //�������1ʹ���˽���

char USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART1_REC_LEN���ֽ�.
void uart1_init(u32 bound)
{
	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO�˿�����
    NVIC_InitTypeDef NVIC_InitStructure;//�����ж�  NVIC�˿�����
    USART_InitTypeDef USART_InitStructure;//USART 1-3�˿�����
    /*��������ʱ��*/      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��
	USART_DeInit(USART1);  //��λ����1
	//USART1_TX		PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //USART1_RX		PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// ��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//Usart  NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);    //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������1�����ж�
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //��������1�����ж�  --  TXE�жϷ�ʽ
    //USART_ITConfig(USART1, USART_IT_TC, ENABLE); //��������1�����ж�  --  TC�жϷ�ʽ
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}
	#if EN_USART_code_key  /* ���ݰ�����ʽ���� */
	u8 USART1_RX_head=0;		//����״̬��� --  ����ͷ
	u8 USART1_RX_num=0;		//����״̬��� --  ���ݼ���
	u8 USART1_RX_len=0;		//����״̬��� --  ���ݰ�������
	u8 USART1_led=0; //���յ�USART1   ����   �յ�����

	void USART1_IRQHandler(void)                	//����1�жϷ������  --  ���ݰ�����ʽ����
	{
		if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //�жϲ���   
		{   
			USART_ClearITPendingBit(USART1,USART_IT_RXNE); //����жϱ�־  
			USART1_RX_BUF[USART1_RX_num] = USART_ReceiveData(USART1);   
			USART1_RX_num++;   
			USART1_RX_num &= 0xFF;   
		}   
		if(USART1_RX_BUF[USART1_RX_num-1] == (char)0xff) USART1_RX_head = USART1_RX_num-1;   //ͷ   
		
		if( (USART1_RX_BUF[USART1_RX_head] ==(char)0xff)&&(USART1_RX_BUF[USART1_RX_num-1] ==(char)0xee))
		{   //��⵽ͷ������¼�⵽β   
			USART1_RX_len = USART1_RX_num-1- USART1_RX_head; //����   
			USART1_RX_num = 0; //��ʼ��   
			USART1_RX_head = 0;  
			USART1_led=1; //�յ�����			
		}   
		if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET) //���   
		{   
			USART_ClearFlag(USART1,USART_FLAG_ORE); //��SR   
			USART_ReceiveData(USART1); //��DR   
			memset(USART1_RX_BUF,0,USART1_REC_LEN); //���ڽ��ջ����� �� 0
		} 
	/* code ...  */	
	}
	#elif 0  //TC�жϷ�ʽ�����ַ���
	char *pDataByte ;
	/*******����:�жϷ�ʽ�����ַ���,�����ж�TC�ķ�ʽ.�� �ж� ���ͺ� �ж�λ.
	����: �ַ������׵�ַ  ;   ���: ��                                *******/
	void USART_SendDataString(USART_TypeDef* USARTx,char *pucString )
	{
		pDataByte = pucString;
		USART_ClearFlag(USARTx, USART_FLAG_TC);//���������ɱ�־λ,������ܻᶪʧ��1���ֽڵ�����.
		USART_SendData(USARTx, *(pDataByte++) ); //����Ҫ++����Ȼ��ѵ�һ���ַ�t��������
	}
	void USART1_IRQHandler(void)                	//����1�жϷ������
	{
		if( USART_GetITStatus(USART1, USART_IT_TC) == SET  )
		{
			if( *pDataByte == '\0' )//TC��Ҫ ��SR+дDR ������0,�����͵����,����\0����ʱ���ø�if�жϹص�
			USART_ClearFlag(USART1, USART_FLAG_TC);//��ȻTCһֱ��set,TCIEҲ�Ǵ򿪵�,���»᲻ͣ�����ж�,clear������,���ùص�TCIE
			else
			USART_SendData(USART1, *pDataByte++ );
		}	
	}
	#elif 0  //TXE�жϷ�ʽ�����ַ���
	char *pDataByte ;
	/*******����:�жϷ�ʽ�����ַ���.�����ж�TXE�ķ�ʽ.�� �ж� ���ͺ��ж� λ.
	����:�ַ������׵�ַ	; ���:��	*******/
	void USART_SendDataString(USART_TypeDef* USARTx,char *pucString )
	{
		pDataByte = pucString;
		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);//ֻҪ���ͼĴ���Ϊ��,�ͻ�һֱ���ж�,���,Ҫ�ǲ���������ʱ,�ѷ����жϹر�,ֻ�ڿ�ʼ����ʱ���Ŵ򿪡�
	}
	void USART1_IRQHandler(void)                	//����1�жϷ������
	{
		if(USART_GetITStatus(USART1, USART_IT_TXE) == SET )
		{
			if( *pDataByte == '\0' )//�����͵��ֽڷ���ĩβNULL��
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);//��Ϊ�� ���ͼĴ����� ���жϣ����Է����ַ��������ص�������ֻҪ���ˣ��ͻ���ж�
			else
			USART_SendData(USART1, *pDataByte++ );
		}	
	}
	#else 
	u16 USART1_RX_STA=0;       //����״̬���	  
	void USART1_IRQHandler(void)                	//����1�жϷ������
	{
		u8 Res;
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			Res =USART_ReceiveData(USART1);	//��ȡ���յ�������	
			if((USART1_RX_STA&0x8000)==0)//����δ���
			{
				if(USART1_RX_STA&0x4000)//���յ���0x0d
				{
					if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
					else USART1_RX_STA|=0x8000;	//��������� 
				}
				else //��û�յ�0X0D
				{	
					if(Res==0x0d)USART1_RX_STA|=0x4000;
					else
					{
						USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
						USART1_RX_STA++;
						if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
		} 
	} 
	#endif	

#endif	

/************************************************** ����2 *********************************************************/
#if EN_USART2_RX   ////�������2ʹ���˽���

char USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART2_REC_LEN���ֽ�.
void uart2_init(u32 bound)
{
	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO�˿�����
    NVIC_InitTypeDef NVIC_InitStructure;//�����ж�  NVIC�˿�����
    USART_InitTypeDef USART_InitStructure;//USART 1-3�˿�����
    /*��������ʱ��*/      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    USART_DeInit(USART2);  //��λ����2
    //PA2 TXD2  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;// �����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //PA3 RXD2  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// ��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //Usart  NVIC ���� 
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =3;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);     //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART2
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������2�����ж�
    //USART_ITConfig(USART2, USART_IT_TXE, ENABLE); //��������2�����ж�  
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2

}
	#if EN_USART_code_key  /* ���ݰ�����ʽ���� */
u8 USART2_RX_head=0;		//����״̬��� --  ����ͷ
u8 USART2_RX_num=0;			//����״̬��� --  ���ݼ���
u8 USART2_RX_len=0;			//����״̬��� --  ���ݰ�������
u8 USART2_led=0; //�յ�����			

void USART2_IRQHandler(void) //����2�жϷ������  --  ���ݰ�����ʽ����           
{											            	/*				���� Android			*/
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) //�жϲ���   
    {   
        USART_ClearITPendingBit(USART2,USART_IT_RXNE); //����жϱ�־  
        USART2_RX_BUF[USART2_RX_num] = USART_ReceiveData(USART2);   
        USART2_RX_num++;   
        USART2_RX_num &= 0xFF;   
    }  
/*------   ���� ��ʽ  "#0020...%"    -------*/	
    if(USART2_RX_BUF[USART2_RX_num-1] == '#') //����ͷ "#..."
	{
		USART2_RX_head = USART2_RX_num-1;   //ͷ  -1-					      #0001..................%	
	}									
    if( (USART2_RX_BUF[USART2_RX_head] == '#')&&(USART2_RX_BUF[USART2_RX_num-1] == '%') ) //��⵽ͷ������¼�⵽β   
    {   
        USART2_RX_num = 0;    
		USART2_led=1; //�յ�����			
		USART2_RX_len = USART2_RX_num - USART2_RX_head; //����  			//0123456		
    }   
    if(USART_GetFlagStatus(USART2,USART_FLAG_ORE) == SET) //���   
    {   
        USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR   
        USART_ReceiveData(USART2); //��DR  
		memset(USART2_RX_BUF,0,USART2_REC_LEN); //���ڽ��ջ����� �� 0
    } 
/* code ...  */	
}
	#else 
u16 USART2_RX_STA=0;       //����״̬���	  
void USART2_IRQHandler(void)                	//����2�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������		
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
    } 
} 
	#endif	
	
#endif	

/************************************************** ����3 *********************************************************/
#if EN_USART3_RX   ////�������2ʹ���˽���

char USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART2_REC_LEN���ֽ�.
void uart3_init(u32 bound)
{
	//GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;//GPIO�˿�����
    NVIC_InitTypeDef NVIC_InitStructure;//�����ж�  NVIC�˿�����
    USART_InitTypeDef USART_InitStructure;//USART 1-3�˿�����
    /*��������ʱ��*/      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
    USART_DeInit(USART3);  //��λ����3
    //PB10  TXD3  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		// �����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	//PB11 RXD3  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// ��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//Usart NVIC ���� 
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);     //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART2
   //USART ��ʼ������    
    USART_InitStructure.USART_BaudRate = bound;         //һ������Ϊ9600;       
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //һ��ֹͣλ 
    USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //�շ�ģʽ     
    USART_Init(USART3, &USART_InitStructure);//��ʼ������

	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//��������3�����ж�
    //USART_ITConfig(USART3, USART_IT_TXE, ENABLE); //��������3�����ж�  
    USART_Cmd(USART3, ENABLE); 					 //ʹ�ܴ���3   
}
	#if !EN_USART_code_key  /* ���ݰ�����ʽ���� */
u8 USART3_RX_head=0;		//����״̬��� --  ����ͷ
u8 USART3_RX_num=0;		//����״̬��� --  ���ݼ���
u8 USART3_RX_len=0;		//����״̬��� --  ���ݰ�������
void USART3_IRQHandler(void)                	//����3�жϷ������  --  ���ݰ�����ʽ����
{
    if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET) //�жϲ���   
    {   
        USART_ClearITPendingBit(USART3,USART_IT_RXNE); //����жϱ�־  
        USART3_RX_BUF[USART3_RX_num] = USART_ReceiveData(USART3);   
        USART3_RX_num++;   
        USART3_RX_num &= 0xFF;   
    }   
    if(USART3_RX_BUF[USART3_RX_num-1] == 0xff) USART3_RX_head = USART3_RX_num-1;   //ͷ   
    
    if( (USART3_RX_BUF[USART3_RX_head] == 0xff)&&(USART3_RX_BUF[USART3_RX_num-1] == 0xee) ) //��⵽ͷ������¼�⵽β   
    {   
        USART3_RX_len = USART3_RX_num-1- USART3_RX_head; //����   
        USART3_RX_num = 0; //��ʼ��   
        USART3_RX_head = 0;   
    }   
    if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET) //���   
    {   
        USART_ClearFlag(USART3,USART_FLAG_ORE); //��SR   
        USART_ReceiveData(USART3); //��DR   
		memset(USART3_RX_BUF,0,USART3_REC_LEN); //���ڽ��ջ����� �� 0
    } 
/* code ...  */	
}
//	#else 
u16 USART3_RX_STA=0;       //����״̬���	  
void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);	//��ȡ���յ�������		
		if((USART3_RX_STA&0x8000)==0)//����δ���
		{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else USART3_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
    } 
} 
	#endif	
	
#endif	

/***********************************************^^^^^^^^^^^^^^^^^^^^*****************************************/
//void USART_Send_String(USART_TypeDef* USARTx,char *pucString);

/*����һ���ֽ�����*/
unsigned char UARTGetByte(USART_TypeDef* USARTx,unsigned char* Get_Data)
{   	   
    if(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET) return 1;//û���յ����� 
    *Get_Data = USART_ReceiveData(USARTx); 
    return 0;//�յ�����
}

/*  TXE��ʽ ������һ���ֽ�����*/
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

/*  TC��ʽ ������һ���ֽ�����*/
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
void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )//  �ú����Ϳ�����printfʹ�ÿɱ����������ܶ�
{
	const char *s;
	int d;   
	char buf[16];
	va_list ap;
	va_start(ap, Data);
	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
				break;
				case 'n':							          //���з�
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
					case 's':										  //�ַ���
						s = va_arg(ap, const char *);
						for ( ; *s; s++) 
						{
							USART_SendData(USARTx,*s);
							while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
						}
						Data++;
					break;
					case 'd':						
						d = va_arg(ap, int); //ʮ����		
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
