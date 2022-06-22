/******************** ��ʢ���ӹ����� ********************
 * �ļ���  ��adc.c
 * ����    ��DMA��ʽ��ȡADCֵӦ�ú�����         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ� PCA0 - ADC1 
 * ��汾  ��ST3.0.0  
 * �Ա��꣺http://shop66177872.taobao.com
**********************************************************************************/

#include "adc.h"

#define ADC1_DR_Address    ((u32)0x4001244C)

__IO u16 ADC_ConvertedValue;  

/*
ADC_value=ADC_GetConversionValue(ADC1);//uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
voltage= 3300000/4096*ADC_value/1000;
*/

void ADC1_Mode_init(void)///stm32_adcת����ģ�������ΪPB0
{
    /* ADC1 configuration */
    ADC_InitTypeDef   ADC_InitStructure;  //����һ���ṹ��
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE);	  //ʹ��ADC1ʱ��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //PB0��Ϊģ�������
 	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  //PB0��Ϊģ������� 
    GPIO_Init(GPIOA ,&GPIO_InitStructure);  // ��ʼ��GPIOA

    RCC_ADCCLKConfig(RCC_PCLK2_Div4);   //����ADCʱ�ӷ�Ƶ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ʹ�ö���ģʽ��ɨ��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ʹ�ܶ�ͨ��ɨ��ģʽ//ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��ģʽ//������Ӵ�����
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�������ת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;  //ָ����ת����ADCͨ��������
    ADC_Init(ADC1, &ADC_InitStructure);   //��ʼ��ADC
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5); 
	/*  ADC1ѡ���ŵ�8,ת��˳��1������ʱ��55.5����    */     
	
    ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
    ADC_ResetCalibration(ADC1);       //����ADC1У׼�Ĵ���
    while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ�ADC1У׼�������
    ADC_StartCalibration(ADC1); //��ʼADC1У׼
    while(ADC_GetCalibrationStatus(ADC1));  //�ȴ�ADC1У׼���
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ADC1�����ʼת��
}
//u16 ADCvalue_Function(void)
//{
//	u16 ADC_value=ADC_GetConversionValue(ADC1);//uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
//	return 3300000/4096*ADC_value/1000;/*voltage= 3300000/4096*ADC_value/1000;*/
//}

/*******************��ʼ��ADC1++++++++++++++++++++*/
//ģ�������ΪPA 0 1 2 3/*����ADC1�Ĺ���ģʽΪDMAģʽ  */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	DMA_Mode_Config();
	ADC_Mode_Config();
}
/*���ò���ͨ���˿� ʹ��GPIOʱ��	  ����ADC����PA0�˿��ź�*/
 void ADC1_GPIO_Config(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;//PA 0 1 2 3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
    GPIO_Init(GPIOA,&GPIO_InitStructure);      // ��ʼ��GPIOA

}
 /*����ADC1�Ĺ���ģʽΪMDAģʽ  */
 void DMA_Mode_Config(void)
{
    /* DMA2 ֧��ȫ�����ִ���ģʽ���� DMA1 ֻ�����赽�洢���ʹ洢������������ģʽ */
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMA1ʱ��
    /* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1); //ָ��DMAͨ��
	/*��������Ǹ�DMA����ͨ��������ST�ṩ�����ϣ�STM3210Fx��DMA1����7��ͨ����CH1~CH7����DMA2����5��ͨ����CH1~CH5��������Ϊ ���� �� memory �ṩ7���������� */
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;////ADC1��ַ---����ADC1����ת��ֵ�ļĴ���
 	/*                DMA_PeripheralBaseAddr ��DMAһ����ʼ��ַ���ñ���һ��        buffer��ʼ��ַ��
	�������̣�����Ĵ��� DMA_PeripheralBaseAddr Memory�ڴ� �б����ռ䣨��flash�����ݿռ�ȣ���ADC1_DR_Address���Ҷ����һ����ַ������*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
	/*                DMA_MemoryBaseAddr ��DMAҪ������ Memory�ڴ� �б����ĵ�ַ��ADC_ConvertedValue �����Լ��� memory �ж����һ��������*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
	/*                DMA_DIR ������DMA�Ĵ��䷽���������õ��� �����䣬
	                  DMA����˫���䣬�����Ҫ˫���䣬�� DMA_DIR_PeripheralSRC �ĳ� DMA_DIR_PeripheralDST ���ɡ�*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	/*                DMA_PeripheralInc  ������DMA�� ���� ����ģʽ�����DMAѡ�õ�ͨ����CHx���ж���������ӣ���Ҫʹ���������ģʽ DMA_PeripheralInc_Enable
	�ҵ������� DMA ֻ�� ADC1 ��������ϵ������ѡ�� DMA_PeripheralInc_Disable */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����---�����ʾʼ�մ�����ADC1��ַ��ȡֵ---���α��浽�����������ڴ������---
	/*                DMA_MemoryInc      ������DMA�� �ڴ� ����ģʽ��DMA���ʶ���ڴ����ʱ����Ҫʹ��DMA_MemoryInc_Enable��
	��DMAֻ����һ���ڴ����ʱ�������ó� DMA_MemoryInc_Disable */
    
	DMA_InitStructure.DMA_BufferSize = 4;	//����������---2ͨ����Ҫ����2������
	/*                DMA_BufferSize ������DMA�ڴ���ʱ �������ĳ��ȣ�ǰ���ж���   buffer��ʼ��ַ��ADC1_DR_Address ��
	Ϊ�˰�ȫ�ԺͿɿ��ԣ�һ����Ҫ��buffer����һ������Ƭ������������ĵ�λ��          �������ͣ�Byte��HalfWord��word��
	�����õ�2�� half-word(�����������)��32λ��MCU��1��half-wordռ16 bits��*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݿ��Ϊ16λ
	/*                DMA_PeripheralDataSize ������DMA�ڷ���ʱ ÿ�β��������ݳ��ȡ� �������ݳ������ͣ�ǰ���Ѿ������ˣ����ﲻ��������*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�ڴ����ݿ��Ϊ16λ
	/*                DMA_MemoryDataSize ������DMA�ڷ���ʱ ÿ�β��������ݳ��ȡ�     �������ݳ������ͣ�ǰ���Ѿ������ˣ����ﲻ��������*/

/*buffer_size����ʱ��Ҫע������ 
1. �������ݿ�ȡ��ڴ����ݿ�� 
2. �����������ⲿ�洢���Ĵ�С
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   #�������ݿ��Ϊ16
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  #�ڴ����ݿ��Ϊ16λ
����������£����贫�� 2���ֽڣ���buffer_size = 16/16= 1 
���贫�� 4��*2���ֽڣ���buffer_size = 4*1 =4*/

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ---2����������ѭ�����մ�����ADC1���������ADCֵ---
	/*                DMA_Mode ������DMA�� ����ģʽ��ѭ��ģʽ��
	��ֻ�����һ�κ�Ͳ�Ҫ�����ˣ��� ��ָ����������ʣ�Ҳ������Ҫ�����ʵ�ʱ��ͷ��ʣ���Ҫ�����ʵ�ʱ���ֹͣ�����������ó�ͨ��ģʽ DMA_Mode_Normal  */
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/*                DMA_Priority ������DMA�� ���ȼ��𣺿��Է�Ϊ4����VeryHigh,High,Medium,Low */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/*                DMA_M2M ������DMA�� 2�� memory�ڴ� �е� ���� ������� */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);// ��ʼ��

	DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��DMAͨ��
}

void ADC_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//ʹ��ADC1ʱ��

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ʹ�ö���ģʽ��ɨ��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //������Ӵ�����
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ʹ�������Ҷ���
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel =4;  // ADCת��ͨ������
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel11 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
     ////ADC1ѡ���ŵ�0,ת��˳��1������ʱ��55.5����         
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); 
     ////ADC1ѡ���ŵ�1,ת��˳��2������ʱ��55.5����         
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
      ////ADC1ѡ���ŵ�2,ת��˳��3������ʱ��55.5����         
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
     ////ADC1ѡ���ŵ�3,ת��˳��4������ʱ��55.5����         

/* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	 //ʹ��ADC��DMA

/* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));

    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //��ʼת��

 }

////    AD1_value  = 3300000/4096*ADC_ConvertedValue[0]/1000;//PA 0 
////    AD2_value  = 3300000/4096*ADC_ConvertedValue[1]/1000;//PA 1
////    AD3_value  = 3300000/4096*ADC_ConvertedValue[2]/1000;//PA 2
////    AD4_value  = 3300000/4096*ADC_ConvertedValue[3]/1000;//PA 3
////    printf("AD1 value = %d mV  \r\n", AD1_value);
////     printf("AD2 value = %d mV  \r\n", AD2_value);
////     printf("AD3 value = %d mV  \r\n", AD3_value);
////     printf("AD4 value = %d mV  \r\n", AD4_value);


