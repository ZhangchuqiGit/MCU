/******************** 鑫盛电子工作室 ********************
 * 文件名  ：adc.c
 * 描述    ：DMA方式读取ADC值应用函数库         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接： PCA0 - ADC1 
 * 库版本  ：ST3.0.0  
 * 淘宝店：http://shop66177872.taobao.com
**********************************************************************************/
#include "adc.h"


#if ADC_load==0   //单路通道

__IO u16 ADC_ConvertedValue;  //单路通道
void ADC1_Mode_init(void)///stm32_adc转换，模拟输入端为PB0
{
    /* ADC1 configuration */
    ADC_InitTypeDef   ADC_InitStructure;  //声明一个结构体
    GPIO_InitTypeDef GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC1时钟
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //PB0设为模拟输入脚
    GPIO_Init(GPIOB,&GPIO_InitStructure);  // 初始化GPIOB

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //配置ADC时钟分频    72M/6=12M,    ADC 最大时间不能超过14M
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//使用独立模式，扫描模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //使能多通道扫描模式//模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换模式//无需外接触发器
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //软件启动转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;  //指定将转换的ADC通道的数量
    ADC_Init(ADC1, &ADC_InitStructure);   //初始化ADC
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5); 
//ADC1选择信道8,转换顺序1，采样时间55.5周期         
    ADC_Cmd(ADC1, ENABLE); //使能ADC1
    ADC_ResetCalibration(ADC1);       //重置ADC1校准寄存器
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待ADC1校准重置完成
    ADC_StartCalibration(ADC1); //开始ADC1校准
    while(ADC_GetCalibrationStatus(ADC1));  //等待ADC1校准完成
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能ADC1软件开始转换

}
/*******************************---------------------*************************************/
//获得ADC值
//ch:通道值
u16 Get_Adc(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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

#else   //多路通道

#define ADC1_DR_Address    ((u32)0x4001244C)
__IO u16 ADC_ConvertedValue[5];  //多路通道  存放结果

/*******************************************初始化ADC1+++++++*******************+++++++++++++*/
//模拟输入端为PA 0 1 2 3/*配置ADC1的工作模式为MDA模式  */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	DMA_Mode_Config();
	ADC_Mode_Config();
}
/*配置采样通道端口 使能GPIO时钟*/
void ADC1_GPIO_Config(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
    GPIO_Init(GPIOC,&GPIO_InitStructure);   
}
/*配置ADC1的工作模式为MDA模式  */
void DMA_Mode_Config(void)
{
    //++++++++++++++++**************------------//////////////
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能MDA1时钟
    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);  //指定DMA通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;////ADC1地址---代表ADC1保存转换值的寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//设置DMA内存地址，ADC转换结果直接放入该地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
    DMA_InitStructure.DMA_BufferSize = 5*2;	//传输总数据---1通道需要传输2个数据
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址固定
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增---总体表示始终从外设ADC1地址处取值---依次保存到连续的两个内存变量中---
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //内存传输数据单元---半字16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式---2个数据依次循环接收从外设ADC1传输过来的ADC值---
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);  //使能DMA通道
}
////////////////////////////////////////////////////////////////////////// 
void ADC_Mode_Config(void)
 {
    ADC_InitTypeDef ADC_InitStructure;	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//使能ADC1时钟

//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12M,ADC最大时间不能超过14M
#if  ADC_Temp_g
	 ADC_TempSensorVrefintCmd(ENABLE);//打开ADC内部温度传感器
#endif
	/* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //使用独立模式，扫描模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //无需外接触发器
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //使用数据右对齐
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel =5;  // ADC转换通道个数
    ADC_Init(ADC1, &ADC_InitStructure);	 
    /* ADC1 regular channel1 configuration */ 
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);//ADC1选择信道0,转换顺序1，采样时间55.5周期         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);//ADC1选择信道1,转换顺序2，采样时间55.5周期         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);//ADC1选择信道2,转换顺序3，采样时间55.5周期         
//    //   .............................
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 12, ADC_SampleTime_55Cycles5);//ADC1选择信道12,转换顺序12，采样时间55.5周期         
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 13, ADC_SampleTime_55Cycles5);//ADC1选择信道13,转换顺序13，采样时间55.5周期           
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_71Cycles5);     
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_71Cycles5);      
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_71Cycles5);   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_71Cycles5);      
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 5, ADC_SampleTime_71Cycles5);       
	/* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	 //使能ADC的DMA
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE); //使能ADC1 //开启AD转换器
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);//重置指定的ADC的校准寄存器
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));//获取ADC重置校准寄存器的状态
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);//开始指定ADC的校准状态
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));//获取指定ADC的校准程序
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //开始转换//开始采集
}

#endif


#if  !ADC_Temp_g  //打开ADC内部温度传感器

/*******************************************************************************
* 函 数 名         : Get_ADC_Temp_Value
* 函数功能		   : 获取通道ch的转换值，取times次,然后平均 	
* 输    入         : ch:通道编号
					 times:获取次数
* 输    出         : 通道ch的times次转换结果平均值
*******************************************************************************/
u16 Get_ADC_Temp_Value(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADC通道,239.5个周期,提高采样时间可以提高精确度
	for(t=0;t<times;t++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能指定的ADC1的软件转换启动功能	
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
		temp_val+=ADC_GetConversionValue(ADC1);
		delay_ms(5);
	}
	return temp_val/times;
} 
/*******************************************************************************
* 函 数 名         : Get_Temperture
* 函数功能		   : 获取温度值 	
* 输    出         : 温度值(扩大了100倍,单位:℃)
*******************************************************************************/
int Get_Temperture(void)
{
	u32 adc_value;
	int temp;
 	double temperture;
	adc_value=Get_ADC_Temp_Value(ADC_Channel_16,10);	//读取通道16内部温度传感器通道,10次取平均
	temperture=(float)adc_value*(3.3/4096);		//电压值
	temperture=(1.43-temperture)/0.0043+25; //转换为温度值 
	temp=temperture*100;					//扩大100倍.
	return temp;
}
////temp=Get_Temperture();
////if(temp<0)
////{
////	temp=-temp;
////	printf("内部温度检测值为：-");
////}
////else
////{
////	printf("内部温度检测值为：+");
////}
////printf("%.2f°C\r\n",(float)temp/100);

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


