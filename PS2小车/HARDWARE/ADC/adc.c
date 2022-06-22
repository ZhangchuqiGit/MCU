/******************** 鑫盛电子工作室 ********************
 * 文件名  ：adc.c
 * 描述    ：DMA方式读取ADC值应用函数库         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接： PCA0 - ADC1 
 * 库版本  ：ST3.0.0  
 * 淘宝店：http://shop66177872.taobao.com
**********************************************************************************/

#include "adc.h"

#define ADC1_DR_Address    ((u32)0x4001244C)

__IO u16 ADC_ConvertedValue;  

/*
ADC_value=ADC_GetConversionValue(ADC1);//uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
voltage= 3300000/4096*ADC_value/1000;
*/

void ADC1_Mode_init(void)///stm32_adc转换，模拟输入端为PB0
{
    /* ADC1 configuration */
    ADC_InitTypeDef   ADC_InitStructure;  //声明一个结构体
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC1时钟
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;  //PB0设为模拟输入脚
 	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;  //PB0设为模拟输入脚 
    GPIO_Init(GPIOA ,&GPIO_InitStructure);  // 初始化GPIOA

    RCC_ADCCLKConfig(RCC_PCLK2_Div4);   //配置ADC时钟分频
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//使用独立模式，扫描模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //使能多通道扫描模式//模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换模式//无需外接触发器
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //软件启动转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;  //指定将转换的ADC通道的数量
    ADC_Init(ADC1, &ADC_InitStructure);   //初始化ADC
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5); 
	/*  ADC1选择信道8,转换顺序1，采样时间55.5周期    */     
	
    ADC_Cmd(ADC1, ENABLE); //使能ADC1
    ADC_ResetCalibration(ADC1);       //重置ADC1校准寄存器
    while(ADC_GetResetCalibrationStatus(ADC1));  //等待ADC1校准重置完成
    ADC_StartCalibration(ADC1); //开始ADC1校准
    while(ADC_GetCalibrationStatus(ADC1));  //等待ADC1校准完成
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能ADC1软件开始转换
}
//u16 ADCvalue_Function(void)
//{
//	u16 ADC_value=ADC_GetConversionValue(ADC1);//uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
//	return 3300000/4096*ADC_value/1000;/*voltage= 3300000/4096*ADC_value/1000;*/
//}

/*******************初始化ADC1++++++++++++++++++++*/
//模拟输入端为PA 0 1 2 3/*配置ADC1的工作模式为DMA模式  */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	DMA_Mode_Config();
	ADC_Mode_Config();
}
/*配置采样通道端口 使能GPIO时钟	  设置ADC采样PA0端口信号*/
 void ADC1_GPIO_Config(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;//PA 0 1 2 3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
    GPIO_Init(GPIOA,&GPIO_InitStructure);      // 初始化GPIOA

}
 /*配置ADC1的工作模式为MDA模式  */
 void DMA_Mode_Config(void)
{
    /* DMA2 支持全部三种传输模式，而 DMA1 只有外设到存储器和存储器到外设两种模式 */
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA1时钟
    /* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1); //指定DMA通道
	/*上面这句是给DMA配置通道，根据ST提供的资料，STM3210Fx中DMA1包含7个通道（CH1~CH7），DMA2包含5个通道（CH1~CH5），可以为 外设 或 memory 提供7座“桥梁” */
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;////ADC1地址---代表ADC1保存转换值的寄存器
 	/*                DMA_PeripheralBaseAddr 给DMA一个起始地址，好比是一个        buffer起始地址，
	数据流程：外设寄存器 DMA_PeripheralBaseAddr Memory内存 中变量空间（或flash中数据空间等），ADC1_DR_Address是我定义的一个地址变量；*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//设置DMA内存地址，ADC转换结果直接放入该地址
	/*                DMA_MemoryBaseAddr 是DMA要连接在 Memory内存 中变量的地址，ADC_ConvertedValue 是我自己在 memory 中定义的一个变量；*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为设置为数据传输的来源
	/*                DMA_DIR 是设置DMA的传输方向，这里设置的是 单向传输，
	                  DMA可以双向传输，如果需要双向传输，把 DMA_DIR_PeripheralSRC 改成 DMA_DIR_PeripheralDST 即可。*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	/*                DMA_PeripheralInc  是设置DMA的 外设 递增模式，如果DMA选用的通道（CHx）有多个外设连接，需要使用外设递增模式 DMA_PeripheralInc_Enable
	我的例子里 DMA 只与 ADC1 建立了联系，所以选用 DMA_PeripheralInc_Disable */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增---总体表示始终从外设ADC1地址处取值---依次保存到连续的两个内存变量中---
	/*                DMA_MemoryInc      是设置DMA的 内存 递增模式，DMA访问多个内存参数时，需要使用DMA_MemoryInc_Enable，
	当DMA只访问一个内存参数时，可设置成 DMA_MemoryInc_Disable */
    
	DMA_InitStructure.DMA_BufferSize = 4;	//传输总数据---2通道需要传输2个数据
	/*                DMA_BufferSize 是设置DMA在传输时 缓冲区的长度，前面有定义   buffer起始地址：ADC1_DR_Address ，
	为了安全性和可靠性，一般需要给buffer定义一个储存片区，这个参数的单位有          三种类型：Byte、HalfWord、word，
	我设置的2个 half-word(见下面的设置)；32位的MCU中1个half-word占16 bits。*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据宽度为16位
	/*                DMA_PeripheralDataSize 是设置DMA在访问时 每次操作的数据长度。 三种数据长度类型，前面已经讲过了，这里不在叙述。*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//内存数据宽度为16位
	/*                DMA_MemoryDataSize 是设置DMA在访问时 每次操作的数据长度。     三种数据长度类型，前面已经讲过了，这里不在叙述。*/

/*buffer_size设置时需要注意的事项， 
1. 外设数据宽度、内存数据宽度 
2. 我们设置了外部存储区的大小
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   #外设数据宽度为16
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  #内存数据宽度为16位
上述此情况下，假设传输 2个字节，则buffer_size = 16/16= 1 
假设传输 4个*2个字节，则buffer_size = 4*1 =4*/

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式---2个数据依次循环接收从外设ADC1传输过来的ADC值---
	/*                DMA_Mode 是设置DMA的 传输模式：循环模式，
	若只想访问一次后就不要访问了（或 按指令操作来反问，也就是想要它访问的时候就访问，不要它访问的时候就停止），可以设置成通用模式 DMA_Mode_Normal  */
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/*                DMA_Priority 是设置DMA的 优先级别：可以分为4级：VeryHigh,High,Medium,Low */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/*                DMA_M2M 是设置DMA的 2个 memory内存 中的 变量 互相访问 */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);// 初始化

	DMA_Cmd(DMA1_Channel1, ENABLE);  //使能DMA通道
}

void ADC_Mode_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//使能ADC1时钟

    /* ADC1 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //使用独立模式，扫描模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //无需外接触发器
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //使用数据右对齐
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel =4;  // ADC转换通道个数
    ADC_Init(ADC1, &ADC_InitStructure);

    /* ADC1 regular channel11 configuration */ 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
     ////ADC1选择信道0,转换顺序1，采样时间55.5周期         
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5); 
     ////ADC1选择信道1,转换顺序2，采样时间55.5周期         
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
      ////ADC1选择信道2,转换顺序3，采样时间55.5周期         
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
     ////ADC1选择信道3,转换顺序4，采样时间55.5周期         

/* Enable ADC1 DMA */
    ADC_DMACmd(ADC1, ENABLE);	 //使能ADC的DMA

/* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE); //使能ADC1
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));

    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //开始转换

 }

////    AD1_value  = 3300000/4096*ADC_ConvertedValue[0]/1000;//PA 0 
////    AD2_value  = 3300000/4096*ADC_ConvertedValue[1]/1000;//PA 1
////    AD3_value  = 3300000/4096*ADC_ConvertedValue[2]/1000;//PA 2
////    AD4_value  = 3300000/4096*ADC_ConvertedValue[3]/1000;//PA 3
////    printf("AD1 value = %d mV  \r\n", AD1_value);
////     printf("AD2 value = %d mV  \r\n", AD2_value);
////     printf("AD3 value = %d mV  \r\n", AD3_value);
////     printf("AD4 value = %d mV  \r\n", AD4_value);


