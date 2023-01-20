#include "adc.h"


/**********************************************************
功  能：ADC1初始化配置函数
参  数：无
返回值：无
**********************************************************/
void ADC_Hardware_Init(void)
{
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	ADC_InitTypeDef ADC_TypeDefStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_5;//选择端口
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AIN; //模拟输入
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);//初始化配置
	
	// 配置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*****************  ADC设置  ******************/
	// 配置ADC时钟为PCLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

	
	ADC_TypeDefStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_TypeDefStructure.ADC_ScanConvMode = DISABLE;// 禁止扫描模式，多通道才要，单通道不需要
	ADC_TypeDefStructure.ADC_ContinuousConvMode = ENABLE;//连续转换
	ADC_TypeDefStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//禁止触发检测
	ADC_TypeDefStructure.ADC_DataAlign = ADC_DataAlign_Right;//转换结果右对齐
	ADC_TypeDefStructure.ADC_NbrOfChannel = 1;//1个转换在序列规则中
	ADC_Init(ADC1,&ADC_TypeDefStructure);//初始化配置
	
	ADC_Cmd(ADC1,ENABLE);//ADC使能
}


/**********************************************************
功  能：ADC采集函数
参  数：无
返回值：最近一次的转换结果
**********************************************************/
uint16_t ADC_Collect(void)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,1,ADC_SampleTime_55Cycles5);//ADCX,ADC通道，规则序列， 采样时间
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//开启软件ADC1功能
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//等待转换结束
	return ADC_GetConversionValue(ADC1);//返回最近一次转换结果
}


/**********************************************************
功  能：通过LCD显示转换结果
参  数：无
返回值：无
**********************************************************/
void Display_result(void)
{
	float temp,vol;
	adcx = ADC_Collect();
	temp = adcx/50.0;
	vol = adcx*3.3/4096;
	sprintf(show,"Test T:%4d %.1f",adcx,temp);
	ILI9341_DispStringLine_EN(LINE(4),show);
	sprintf(show,"Test VOL:%.3f   V",vol);
	ILI9341_DispStringLine_EN(LINE(5),show);
	Delay_ms(500);
	Delay_ms(500);


}



