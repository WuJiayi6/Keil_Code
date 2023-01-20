#include "stm32f10x.h"
#include "adc1.h"

/**********************************************************
功  能：ADC1初始化配置函数
参  数：无
返回值：无
**********************************************************/
void ADC1_Hardware_Init(void)
{
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	ADC_InitTypeDef ADC_TypeDefStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_1;//选择端口
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入
	
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);//初始化配置
	
	  
	// 优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// 配置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*****************ADC设置**********************************************/
	// 配置ADC时钟为PCLK2的6分频，即12MHz
    //注意ADCCLK最大是14MHz，所以只能用6分频或者8分频
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 

	//ADCX,ADC通道，规则序列， 采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_55Cycles5);
	
	ADC_TypeDefStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_TypeDefStructure.ADC_ScanConvMode = DISABLE;
	ADC_TypeDefStructure.ADC_ContinuousConvMode = ENABLE;//单通道连续转换非扫描模式
	ADC_TypeDefStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//禁止触发检测
	ADC_TypeDefStructure.ADC_DataAlign = ADC_DataAlign_Right;//转换结果右对齐
	ADC_TypeDefStructure.ADC_NbrOfChannel = 1;//1个转换在序列规则中
	ADC_Init(ADC1,&ADC_TypeDefStructure);//初始化配置
	//ADC使能
	ADC_Cmd(ADC1,ENABLE);  
    
    //校准
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1) ==SET );
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1) ==SET);
    
	//开启软件ADC1功能
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}


/**********************************************************
功  能：ADC采集函数
参  数：ch	采集通道
返回值：最近一次的转换结果
**********************************************************/
uint16_t ADC1_Collect(void)
{

	//返回最近一次转换结果
	return ADC_GetConversionValue(ADC1);
}


