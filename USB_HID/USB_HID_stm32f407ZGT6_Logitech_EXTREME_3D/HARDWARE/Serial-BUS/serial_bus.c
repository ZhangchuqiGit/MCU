#include "serial_bus.h"

/* SBUSȫ��serial-bus����һ�ִ���ͨ��Э�飬�㷺Ӧ���ں�ģң���������ջ����С�
ֻ��һ���ź��߾��ܴ�����16ͨ�������ݣ��ȶ�·PWM�����Ч��ʡ��Դ��

�������ã�
	100kbps(100000 bps�������� 115200 bps) �����ʣ�
	8λ����λ����stm32��Ҫѡ��9λ)��
	2λֹͣλ��żУ�飨EVEN)���޿�����25�ֽڡ�

Э���ʽ��  [startbyte] [data1][data2]��[data22] [flags] [endbyte]   : 25�ֽ�
	startbyte 	= 0x0f;
	endbyte 	= 0x00; 
	data1��data22: 	LSB(��λ��ǰ)����Ӧ16��ͨ��(ch1-ch16)��ÿ��ͨ�� 11 bit(22*8 = 16*11)��
	
	flag: 	ʧ�ر�����־
	�ֵ�AT9S��ң����ͨ�ϵ�ʱ����0x00���Ͽ���ʱ����0xC0������ͨ����ѯflagλ����ȡʧ�ر�����
	flags��־λ����������������px4�Ƿ�Ͽ��ı�־λ��
	flags=1�����������������������
	flags=0����������������Ͽ���ʧ�أ���px4����Ƶ��ͣת��

���ݷ�Χ
	��ģң���������PWMֵ��1000~2000����ֵΪ1500��
	sbus ����Ļ᲻һ������ң���������йأ������ֵ�AT9S�ķ�ΧΪ300 ~ 1700����ֵ1000��

sbus �ĸ��߼�
	����ط�һ��Ҫ���ע�⣬�źű���Ӳ��ȡ�������ȡ������Ч�ģ�
	��ΪSBUS���ź��ǲ��õĸ��߼���Ҳ���� ��ƽ�෴����Ҫ��ͼ���������ȡ����
	��Ϊ�������ֻ�ܲ�������λ���ǵô����������������λ8ô����
	���ǲ�������ֹͣλ��У��λɶ�ģ���������Լ�������Ҳ�ܼ򵥡�
	
���ģʽ
	����ģʽ��ÿ��7msһ֡���ݣ���Ϊ��֡�ļ��ֻ�г���3ms���Żᱻ���ܣ�
		�����ݲ����ʼ���һ�£�����25�ֽ���Ҫ��ʱ�� + 3~4ms = 7ms��
	��ͨģʽ��ÿ��14msһ֡���� = ����25�ֽ���Ҫ��ʱ�� + 3~4ms��  

���ݷ�Χ��0-2047֮�䣬��������1102~1927����ֵΪ1500��

ch1��11λ=data2�ĵ�3λ+data1��8λ��
���磺data1=00110110��data2=11001111��
��ʱch1=111 00110110=1846��ͨ��1 ��ֵ��Ϊ1846��
ch2=data3�ĵ�6λ+data2�ĸ�5λ��
ch3=data5�ĵ�1λ+data4��8λ+data3�ĸ�2λ��
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
	
void USART1_SBUS_Init(void) // ֻ����
{
#if EN_USART1_RX	
	NVIC_InitTypeDef NVIC_InitStructure ;//�����жϽṹ��
 #endif
	GPIO_InitTypeDef GPIO_InitStructure;//����IO��ʼ���ṹ��
	USART_InitTypeDef USART_InitStructure;//���崮�ڽṹ��
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); /*GPIOA9����ΪUSART1 TX */
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1 RX
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; /* | GPIO_Pin_10 ; */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/*�ٶ�*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = 100000;/*���������� 100kbps*/
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;/*SBUS��stm32��Ҫѡ��9λ���ݸ�ʽ*/
	USART_InitStructure.USART_StopBits = USART_StopBits_2;/*2��ֹͣλ*/
	USART_InitStructure.USART_Parity = USART_Parity_Even;/*żУ��*/
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx;	/*ֻ��ģʽ*/
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

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

//�ϳɺ�����
//void sbus_out��uint8_t num_values��;//����Ϊͨ������
//values[16]���Ƕ�Ӧ��16��ͨ����

uint16_t values[16]={0}; // ͨ��

void sbus_send(uint8_t num_values/*ͨ����*/)
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
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	// �ȴ����ͽ���
		USART_SendData(USART1, (uint16_t)oframe[i]); // ÿ��1�ֽ����ݷ��͵�����
	}
#if 0 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	// �ȴ����ͽ���
	USART_SendData(USART1, (uint16_t)'\n'); // ÿ��1�ֽ����ݷ��͵�����
#endif
}

/* Э���ʽ��[startbyte]  [data1] [data2] �� [data22]      [flags] [endbyte]  : 25�ֽ� 
   ����       0x0f        X Y RZ Slider Hat_Switch button   0x00    0x00      : 25�ֽ� */

void sbus_out(HID_Logitech_Data_Analyze *Logitech_data) // ����
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


