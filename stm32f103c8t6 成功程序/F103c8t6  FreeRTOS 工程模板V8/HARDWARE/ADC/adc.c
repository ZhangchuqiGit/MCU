#include "adc.h"

/* ����    ��DMA��ʽ��ȡADCֵӦ�ú�����         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ� PCA0 - ADC1 */
#if DMA_ADC_EN

#define ADC1_DR_Address    ((u32)0x4001244C)

__IO u16 ADC_ConvertedValue[4];/*����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ*/

/*******************��ʼ��ADC1++++++++++++++++++++*/
//ģ�������ΪPA 0 1 2 3/*����ADC1�Ĺ���ģʽΪMDAģʽ  */
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
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	  
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;//PA 0 1 2 3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO����Ϊģ������
//    GPIO_Init(GPIOA,&GPIO_InitStructure);      // ��ʼ��GPIOB
    GPIO_Init(GPIOB,&GPIO_InitStructure);      // ��ʼ��GPIOB
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
//    ADC_InitStructure.ADC_NbrOfChannel =4;  // ADCת��ͨ������
    ADC_InitStructure.ADC_NbrOfChannel =2;  // ADCת��ͨ������
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel11 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);//ͨ��1��������55.5��ʱ������ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 4, ADC_SampleTime_239Cycles5);//��һͨ���ɼ�оƬ�ڲ��¶�

//    /* ADC1 regular channel11 configuration */ 
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
//     ////ADC1ѡ���ŵ�0,ת��˳��1������ʱ��55.5����         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); 
//     ////ADC1ѡ���ŵ�1,ת��˳��2������ʱ��55.5����         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
//      ////ADC1ѡ���ŵ�2,ת��˳��3������ʱ��55.5����         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
//     ////ADC1ѡ���ŵ�3,ת��˳��4������ʱ��55.5����         
//*-+
    /* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	 //ʹ��ADC��DMA
//*-+
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
/*����ADC1�Ĺ���ģʽΪMDAģʽ  */
 void DMA_Mode_Config(void)
{
    //++++++++++++++++**************------------//////////////
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��MDA1ʱ��
    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);  //ָ��DMAͨ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;////ADC1��ַ---����ADC1����ת��ֵ�ļĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	/*����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
    DMA_InitStructure.DMA_BufferSize = 4;	//����������---2ͨ����Ҫ����2������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����---�����ʾʼ�մ�����ADC1��ַ��ȡֵ---���α��浽�����������ڴ������---
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�ڴ洫�����ݵ�Ԫ---����16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ---2����������ѭ�����մ�����ADC1���������ADCֵ---
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA channel1 */
    //+++&^%$#**-/-*
    DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��DMAͨ��
//*-+
//*-+   ADC_DMACmd(ADC1, ENABLE);	 //ʹ��ADC��DMA

}

////    AD1_value  = 3300000/4096*ADC_ConvertedValue[0]/1000;//PA 0 
////    AD2_value  = 3300000/4096*ADC_ConvertedValue[1]/1000;//PA 1
////    AD3_value  = 3300000/4096*ADC_ConvertedValue[2]/1000;//PA 2
////    AD4_value  = 3300000/4096*ADC_ConvertedValue[3]/1000;//PA 3
////     printf("AD1 value = %d mV  \r\n", AD1_value);
////     printf("AD2 value = %d mV  \r\n", AD2_value);
////     printf("AD3 value = %d mV  \r\n", AD3_value);
////     printf("AD4 value = %d mV  \r\n", AD4_value);

#endif

void ADC1_Mode_init(void)///stm32_adcת����ģ�������ΪPB0
{
    /* ADC1 configuration */
    ADC_InitTypeDef   ADC_InitStructure;  //����һ���ṹ��
    GPIO_InitTypeDef GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1, ENABLE);	  //ʹ��ADC1ʱ��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //PB0��Ϊģ�������
    GPIO_Init(GPIOB,&GPIO_InitStructure);  // ��ʼ��GPIOB

    RCC_ADCCLKConfig(RCC_PCLK2_Div4);   //����ADCʱ�ӷ�Ƶ
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ʹ�ö���ģʽ��ɨ��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ʹ�ܶ�ͨ��ɨ��ģʽ//ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��ģʽ//������Ӵ�����
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�������ת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;  //ָ����ת����ADCͨ��������
    ADC_Init(ADC1, &ADC_InitStructure);   //��ʼ��ADC
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5); 
//ADC1ѡ���ŵ�8,ת��˳��1������ʱ��55.5����         
    ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1
    ADC_ResetCalibration(ADC1);       //����ADC1У׼�Ĵ���
    while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ�ADC1У׼�������
    ADC_StartCalibration(ADC1); //��ʼADC1У׼
    while(ADC_GetCalibrationStatus(ADC1));  //�ȴ�ADC1У׼���
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ADC1�����ʼת��

}
