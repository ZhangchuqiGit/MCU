/******************** ��ʢ���ӹ����� ********************
 * �ļ���  ��adc.c
 * ����    ��DMA��ʽ��ȡADCֵӦ�ú�����         
 * ʵ��ƽ̨��MINI STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ� PCA0 - ADC1 
 * ��汾  ��ST3.0.0  
 * �Ա��꣺http://shop66177872.taobao.com
**********************************************************************************/
#include "adc.h"


#if ADC_load==0   //��·ͨ��

__IO u16 ADC_ConvertedValue;  //��·ͨ��
void ADC1_Mode_init(void)///stm32_adcת����ģ�������ΪPB0
{
    /* ADC1 configuration */
    ADC_InitTypeDef   ADC_InitStructure;  //����һ���ṹ��
    GPIO_InitTypeDef GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1, ENABLE);	  //ʹ��ADC1ʱ��
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //PB0��Ϊģ�������
    GPIO_Init(GPIOB,&GPIO_InitStructure);  // ��ʼ��GPIOB

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADCʱ�ӷ�Ƶ    72M/6=12M,    ADC ���ʱ�䲻�ܳ���14M
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
/*******************************---------------------*************************************/
//���ADCֵ
//ch:ͨ��ֵ
u16 Get_Adc(u8 ch)   
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t,i;
//	if(times==0)i=1;
//	else i=times;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
//		delay_ms(5);
	}
	return temp_val/times;
} 	 
/*******************************---------------------*************************************/

#else   //��·ͨ��

#define ADC1_DR_Address    ((u32)0x4001244C)
__IO u16 ADC_ConvertedValue[5];  //��·ͨ��  ��Ž��

/*******************************************��ʼ��ADC1+++++++*******************+++++++++++++*/
//ģ�������ΪPA 0 1 2 3/*����ADC1�Ĺ���ģʽΪMDAģʽ  */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	DMA_Mode_Config();
	ADC_Mode_Config();
}
/*���ò���ͨ���˿� ʹ��GPIOʱ��*/
void ADC1_GPIO_Config(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
    GPIO_Init(GPIOC,&GPIO_InitStructure);   
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
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //����Ϊ����Ϊ���ݴ������Դ
    DMA_InitStructure.DMA_BufferSize = 5*2;	//����������---1ͨ����Ҫ����2������
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����---�����ʾʼ�մ�����ADC1��ַ��ȡֵ---���α��浽�����������ڴ������---
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�ڴ洫�����ݵ�Ԫ---����16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ---2����������ѭ�����մ�����ADC1���������ADCֵ---
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��DMAͨ��
}
////////////////////////////////////////////////////////////////////////// 
void ADC_Mode_Config(void)
 {
    ADC_InitTypeDef ADC_InitStructure;	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//ʹ��ADC1ʱ��

//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12M,ADC���ʱ�䲻�ܳ���14M
#if  ADC_Temp_g
	 ADC_TempSensorVrefintCmd(ENABLE);//��ADC�ڲ��¶ȴ�����
#endif
	/* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ʹ�ö���ģʽ��ɨ��ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //������Ӵ�����
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //ʹ�������Ҷ���
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel =5;  // ADCת��ͨ������
    ADC_Init(ADC1, &ADC_InitStructure);	 
    /* ADC1 regular channel1 configuration */ 
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//ADC1ѡ���ŵ�0,ת��˳��1������ʱ��55.5����         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);//ADC1ѡ���ŵ�1,ת��˳��2������ʱ��55.5����         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);//ADC1ѡ���ŵ�2,ת��˳��3������ʱ��55.5����         
//    //   .............................
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 12, ADC_SampleTime_55Cycles5);//ADC1ѡ���ŵ�12,ת��˳��12������ʱ��55.5����         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 13, ADC_SampleTime_55Cycles5);//ADC1ѡ���ŵ�13,ת��˳��13������ʱ��55.5����           
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_71Cycles5);     
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_71Cycles5);      
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_71Cycles5);   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_71Cycles5);      
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 5, ADC_SampleTime_71Cycles5);       
	/* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	 //ʹ��ADC��DMA
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE); //ʹ��ADC1 //����ADת����
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);//����ָ����ADC��У׼�Ĵ���
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));//��ȡADC����У׼�Ĵ�����״̬
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);//��ʼָ��ADC��У׼״̬
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));//��ȡָ��ADC��У׼����
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //��ʼת��//��ʼ�ɼ�
}

#endif


#if  !ADC_Temp_g  //��ADC�ڲ��¶ȴ�����

/*******************************************************************************
* �� �� ��         : Get_ADC_Temp_Value
* ��������		   : ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 	
* ��    ��         : ch:ͨ�����
					 times:��ȡ����
* ��    ��         : ͨ��ch��times��ת�����ƽ��ֵ
*******************************************************************************/
u16 Get_ADC_Temp_Value(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADCͨ��,239.5������,��߲���ʱ�������߾�ȷ��
	for(t=0;t<times;t++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ָ����ADC1�����ת����������	
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
		temp_val+=ADC_GetConversionValue(ADC1);
		delay_ms(5);
	}
	return temp_val/times;
} 
/*******************************************************************************
* �� �� ��         : Get_Temperture
* ��������		   : ��ȡ�¶�ֵ 	
* ��    ��         : �¶�ֵ(������100��,��λ:��)
*******************************************************************************/
int Get_Temperture(void)
{
	u32 adc_value;
	int temp;
 	double temperture;
	adc_value=Get_ADC_Temp_Value(ADC_Channel_16,10);	//��ȡͨ��16�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
	temperture=(float)adc_value*(3.3/4096);		//��ѹֵ
	temperture=(1.43-temperture)/0.0043+25; //ת��Ϊ�¶�ֵ 
	temp=temperture*100;					//����100��.
	return temp;
}
////temp=Get_Temperture();
////if(temp<0)
////{
////	temp=-temp;
////	printf("�ڲ��¶ȼ��ֵΪ��-");
////}
////else
////{
////	printf("�ڲ��¶ȼ��ֵΪ��+");
////}
////printf("%.2f��C\r\n",(float)temp/100);

#endif


////    ADC_value=ADC_GetConversionValue(ADC1);//uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
////    voltage= 3300000/4096*ADC_value/1000; 
////    ADC_value  = 3.3*ADC_ConvertedValue[0]/4096;
////    ADC_value  = 3.3*ADC_ConvertedValue[1]/4096;
////    ADC_value  = 3.3*ADC_ConvertedValue[2]/4096;
////    ADC_value  = 3.3*ADC_ConvertedValue[3]/4096;
////    ADC_value  = 3.3*ADC_ConvertedValue[4]/4096;
////    ADC_value  = 3300000/4096*ADC_ConvertedValue[0]/1000;
////    ADC_value  = 3300000/4096*ADC_ConvertedValue[1]/1000;
////    ADC_value  = 3300000/4096*ADC_ConvertedValue[2]/1000;
////    ADC_value  = 3300000/4096*ADC_ConvertedValue[3]/1000;
////    ADC_value  = 3300000/4096*ADC_ConvertedValue[4]/1000;
////    printf("AD1 value = %d mV  \r\n", AD1_value);
////     printf("AD2 value = %d mV  \r\n", AD2_value);
////     printf("AD3 value = %d mV  \r\n", AD3_value);
////     printf("AD4 value = %d mV  \r\n", AD4_value);


