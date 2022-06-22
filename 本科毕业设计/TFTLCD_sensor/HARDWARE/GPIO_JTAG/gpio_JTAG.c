#include "GPIO_JTAG.h"

/*���������STM32F103C8T6����һ���¶Ȳ��ģ�飬�õ�PB3��PB4��PA15�����ſ������衣
���ֲ�����ô���ã����������Ŷ��������㡣���������ǰ����������������ڵ�PB3��PB4��PA13��PA14��PA15�������IO�ڣ�
����JTAG/SWD�������ĵ��Խӿڡ�����PA13��PA14�ֱ���ΪSWD���Ե�SWIO��SWCLK��PB3��PB4��PA13��PA14��PA15��ͬ����JTAG
�����IO���ŷǳ����⣬�����������ΪSWJ�������ĵ������ţ����Ҫ��Ϊ��ͨIO��ʹ����Ҫ�ر������
************
��PA13����Ϊ����
�������������ͨIO��PA13��Main function ΪJTMS-SWDIO��������ͨIO�ڵĹ�����Alternate functions�е�remap�
Ҳ����˵PA13Ҫ�뵱����ͨIO��ʹ�ã��ͱ���ʹ�������ù����е���ӳ�䡣��˾���Ҫ����������������
һ.��ʱ�������д򿪸���ʱ�ӣ�RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
��.��PA13���Ž�����ӳ�䣺   GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
**********************************************************************************************
������ӳ��͵���I/O���üĴ���(AFIO_MAPR) �� ��ַƫ�ƣ� 0x04   ��λֵ�� 0x0000 0000
λ31:27 ������
λ26:24 SWJ_CFG[2:0]��������JTAG����
��Щλ���������д����������SWJ�͸��ٸ��ù��ܵ�I/O�ڡ� SWJ(������JTAG)֧��JTAG��
SWD����Cortex�ĵ��Զ˿ڡ�ϵͳ��λ���Ĭ��״̬������SWJ��û�и��ٹ��ܣ�����״̬��
����ͨ��JTMS/JTCK���ϵ��ض��ź�ѡ��JTAG��SW(������)ģʽ��
000����ȫSWJ(JTAG-DP + SW-DP)����λ״̬��
001����ȫSWJ(JTAG-DP + SW-DP)��û��JNTRST��
010���ر�JTAG-DP������SW-DP��
100���ر�JTAG-DP���ر�SW-DP��
������ϣ����á�
λ23:21 ������
λ20 ADC2_ETRGREG_REMAP�� ADC2����ת���ⲿ������ӳ��
��λ��������á�1�����á�0������������ADC2����ת���ⲿ���������Ĵ�������ӳ�񡣵���λ
�á�0��ʱ�� ADC2����ת���ⲿ������EXTI11����������λ�á�1��ʱ�� ADC2����ת���ⲿ������
TIM8_TRGO������
λ19 ADC2_ETRGINJ_REMAP�� ADC2ע��ת���ⲿ������ӳ��
��λ��������á�1�����á�0������������ADC2ע��ת���ⲿ���������Ĵ�������ӳ�񡣵���λ
�á�0��ʱ�� ADC2ע��ת���ⲿ������EXTI15����������λ�á�1��ʱ�� ADC2ע��ת���ⲿ������
TIM8ͨ��4������
λ18 ADC1_ETRGREG_REMAP�� ADC1����ת���ⲿ������ӳ��
��λ��������á�1�����á�0������������ADC2����ת���ⲿ���������Ĵ�������ӳ�񡣵���λ
�á�0��ʱ�� ADC1����ת���ⲿ������EXTI11����������λ�á�1��ʱ�� ADC1����ת���ⲿ������
TIM8_TRGO������
λ17 ADC1_ETRGINJ_REMAP�� ADC1ע��ת���ⲿ������ӳ��
��λ��������á�1�����á�0������������ADC2ע��ת���ⲿ���������Ĵ�������ӳ�񡣵���λ
�á�0��ʱ�� ADC2ע��ת���ⲿ������EXTI15����������λ�á�1��ʱ�� ADC1ע��ת���ⲿ������
TIM8ͨ��4������
λ16 TIM5CH4_IREMAP�� TIM5ͨ��4�ڲ���ӳ��
��λ��������á�1�����á�0����������TIM5ͨ��4�ڲ�ӳ�񡣵���λ�á�0��ʱ�� TIM5_CH4��PA3��
��������λ�á�1��ʱ�� LSI�ڲ�������TIM5_CH4������Ŀ���Ƕ������У׼��
λ15 PD01_REMAP���˿�D0/�˿�D1ӳ��OSC_IN/OSC_OUT
��λ��������á�1�����á�0����������PD0��PD1��GPIO����ӳ�񡣵���ʹ��������HSEʱ(ϵ
ͳ�������ڲ���8MHz��������)PD0��PD1����ӳ��OSC_IN��OSC_OUT���š��˹���ֻ
��������36�� 48��64�ܽŵķ�װ(PD0��PD1������TQFP100�ķ�װ�ϣ�������ӳ��)��
0��������PD0��PD1����ӳ��
1�� PD0ӳ��OSC_IN�� PD1ӳ��OSC_OUT��
λ14:13 CAN_REMAP[1:0]�� CAN���ù�����ӳ��
��Щλ��������á�1�����á�0�������Ƹ��ù���CAN_RX��CAN_TX����ӳ��
00�� CAN_RXӳ��PA11�� CAN_TXӳ��PA12��
01��δ����ϣ�
10�� CAN_RXӳ��PB8�� CAN_TXӳ��PB9(��������36�ŵķ�װ)��
11�� CAN_RXӳ��PD0�� CAN_TXӳ��PD1(ֻ������100�ŵķ�װ)��
λ12 TIM4_REMAP����ʱ��4����ӳ��
��λ��������á�1�����á�0�������ƽ�TIM4��ͨ��1-4ӳ�䵽GPIO�˿��ϡ�
0��û����ӳ��(TIM4_CH1/PB6�� TIM4_CH2/PB7�� TIM4_CH3/PB8�� TIM4_CH4/PB9)��
1����ȫӳ��(TIM4_CH1/PD12�� TIM4_CH2/PD13�� TIM4_CH3/PD14�� TIM4_CH4/PD15)��
ע����ӳ��Ӱ����PE0�ϵ�TIM4_ETR��ͨ�ú͸��ù���I/O STM32F10xxx�ο��ֲ�
λ11:10 TIM3_REMAP[1:0]����ʱ��3����ӳ��
��Щλ��������á�1�����á�0�������ƶ�ʱ��3��ͨ��1��4��GPIO�˿ڵ�ӳ��
00��û����ӳ��(CH1/PA6�� CH2/PA7�� CH3/PB0�� CH4/PB1)��
01��δ����ϣ�
10������ӳ��(CH1/PB4�� CH2/PB5�� CH3/PB0�� CH4/PB1)��
11����ȫӳ��(CH1/PC6�� CH2/PC7�� CH3/PC8�� CH4/PC9)��
ע����ӳ��Ӱ����PD2�ϵ�TIM3_ETR��
λ9:8 TIM2_REMAP[1:0]����ʱ��2����ӳ��
��Щλ��������á�1�����á�0�������ƶ�ʱ��2��ͨ��1��4���ⲿ����(ETR)��GPIO�˿ڵ�ӳ��
00��û����ӳ��(CH1/ETR/PA0�� CH2/PA1�� CH3/PA2�� CH4/PA3)��
01������ӳ��(CH1/ETR/PA15�� CH2/PB3�� CH3/PA2�� CH4/PA3)��
10������ӳ��(CH1/ETR/PA0�� CH2/PA1�� CH3/PB10�� CH4/PB11)��
11����ȫӳ��(CH1/ETR/PA15�� CH2/PB3�� CH3/PB10�� CH4/PB11)��
λ7:6 TIM1_REMAP[1:0]����ʱ��1����ӳ��
��Щλ��������á�1�����á�0�������ƶ�ʱ��1��ͨ��1��4�� 1N��3N���ⲿ����(ETR)�Ͷ�������
(BKIN)��GPIO�˿ڵ�ӳ��
00��û����ӳ��(ETR/PA12�� CH1/PA8�� CH2/PA9�� CH3/PA10�� CH4/PA11�� BKIN/PB12��
CH1N/PB13�� CH2N/PB14�� CH3N/PB15)��
01������ӳ��(ETR/PA12�� CH1/PA8�� CH2/PA9�� CH3/PA10�� CH4/PA11�� BKIN/PA6��
CH1N/PA7�� CH2N/PB0�� CH3N/PB1)��
10: δ����ϣ�
11: ��ȫӳ��(ETR/PE7�� CH1/PE9�� CH2/PE11�� CH3/PE13�� CH4/PE14�� BKIN/PE15��
CH1N/PE8�� CH2N/PE10�� CH3N/PE12)��
λ5:4 USART3_REMAP[1:0]�� USART3����ӳ��
��Щλ��������á�1�����á�0��������USART3��CTS�� RTS�� CK�� TX��RX���ù�����GPIO��
�ڵ�ӳ��
00: û����ӳ��(TX/PB10�� RX/PB11�� CK/PB12�� CTS/PB13�� RTS/PB14)��
01: ����ӳ��(TX/PC10�� RX/PC11�� CK/PC12�� CTS/PB13�� RTS/PB14)��
10: δ����ϣ�
11: ��ȫӳ��(TX/PD8�� RX/PD9�� CK/PD10�� CTS/PD11�� RTS/PD12)��
λ3 USART2_REMAP�� USART2����ӳ��
��Щλ��������á�1�����á�0��������USART2��CTS�� RTS�� CK�� TX��RX���ù�����GPIO��
�ڵ�ӳ��
0: û����ӳ��(CTS/PA0�� RTS/PA1�� TX/PA2�� RX/PA3�� CK/PA4)��
1: ��ӳ��(CTS/PD3�� RTS/PD4�� TX/PD5�� RX/PD6�� CK/PD7)��
λ2 USART1_REMAP�� USART1����ӳ��
��λ��������á�1�����á�0��������USART1��TX��RX���ù�����GPIO�˿ڵ�ӳ��
0: û����ӳ��(TX/PA9�� RX/PA10)��
1: ��ӳ��(TX/PB6�� RX/PB7)��
λ1 I2C1_REMAP�� I2C1����ӳ��
��λ��������á�1�����á�0��������I2C1��SCL��SDA���ù�����GPIO�˿ڵ�ӳ��
0: û����ӳ��(SCL/PB6�� SDA/PB7)��
1: ��ӳ��(SCL/PB8�� SDA/PB9)��
λ0 SPI1_REMAP�� SPI1����ӳ��
��λ��������á�1�����á�0��������SPI1��NSS�� SCK�� MISO��MOSI���ù�����GPIO�˿ڵ�ӳ
��
0: û����ӳ��(NSS/PA4�� SCK/PA5�� MISO/PA6�� MOSI/PA7)��
1: ��ӳ��(NSS/PA15�� SCK/PB3�� MISO/PB4�� MOSI/PB5 
*/

//	JTAG_SWD_GPIO_Config();//��ʹ��JTAG���ԣ���Ӧ��IO����PB3,PB4,PA15������Ϊ��ͨIO��ʹ�ã�������ʹ��SWD�������¼��������ͷų���
void JTAG_SWD_GPIO_Config(void)//stm32��PB3 PB4 PA15���ſ�����ʹ��SWD�������¼��������ͷų�������Ϊ��ͨIO��ʹ��
{
 /*   //����ӳ��ʱ�Ӻ�USART��ӳ����I/O������ʱ��
    //ʹ�ܶ�ӦIO�ڵ�ʱ�ӣ���Ϊ�Ǹ��ù������Ի�Ҫʹ��AFIOʱ��    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);//ʹ��GPIOAʱ�� //��ʱ�������д򿪸���ʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //��PA13���Ž�����ӳ��
    
  //  AFIO->MAPR=0;
    AFIO->MAPR&=0XF8FFFFFF;
    AFIO->MAPR|=0x0C ;*/
    
    /*��γ�ʼ�������������IO��ʼ����󣡣���������
    ����PB3 PB4 PA15(JTAG=SWD+ PB3+PB4+PA15)��ʼ����֮������ٴε���GPIOA GPIOB�ӿڵ�ʱ�Ӵ���Ļ���
	��ʹ��PB3 PA15���ű��JTAG�����ż���ʼ����֮������ִ�����´��룬PB3 PA15���Ż���JTAG�����ţ����޷���Ϊ��ͨIOʹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;  
  
    //����ӳ��ʱ�Ӻ�USART��ӳ����I/O������ʱ��
    //ʹ�ܶ�ӦIO�ڵ�ʱ�ӣ���Ϊ�Ǹ��ù������Ի�Ҫʹ��AFIOʱ��    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//��ӳ�䣬��ʹ��JTAG���ԣ���Ӧ��IO����Ϊ��ͨIO��ʹ��	
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);//��ӳ�䣬��ʹ��JTAG���ԣ���Ӧ��IO����Ϊ��ͨIO��ʹ��	

/********------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);//���� 0  

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


