#include "adc.h"

volatile uint16_t ADC_Value[CHANEL_NUM]={0,0,0};

/**********************************************************
功  能：ADC GPIO 初始化
参  数：无
返回值：无
**********************************************************/
void ADC1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE );// 打开 ADC IO端口时钟
    // 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);// 初始化 ADC IO	
    
}

/**********************************************************
功  能：配置ADC工作模式
参  数：无
返回值：无
**********************************************************/
void ADC1_Hardware_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);// 打开DMA时钟
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1,ENABLE);// 打开ADC时钟
	
	DMA_DeInit(DMA1_Channel1);// 复位DMA控制器
	
	/*********配置 DMA 初始化结构体**********/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;// 存储器地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// 数据源来自外设
	DMA_InitStructure.DMA_BufferSize = CHANEL_NUM;// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 存储器地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	// 循环传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// 禁止存储器到存储器模式，因为是从外设到存储器
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);// 初始化DMA
	DMA_Cmd(DMA1_Channel1,ENABLE);// 使能 DMA 通道
	
	/*********ADC 模式配置**********/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;// 只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // 扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;// 连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// 转换结果右对齐
	ADC_InitStructure.ADC_NbrOfChannel = CHANEL_NUM;	// 转换通道个数
	
	ADC_Init(ADC1, &ADC_InitStructure);// 初始化ADC
    
	// 配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置ADC 通道的转换顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);// 使能ADC DMA 请求
	ADC_Cmd(ADC1, ENABLE);// 开启ADC ，并开始转换

    /*********初始化ADC 校准寄存器**********/    
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));// 等待校准寄存器初始化完成
	ADC_StartCalibration(ADC1);// ADC开始校准
	while(ADC_GetCalibrationStatus(ADC1));// 等待校准完成
	 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);// 由于没有采用外部触发，所以使用软件触发ADC转换
}


/**********************************************************
功  能：ADC初始化
参  数：无
返回值：无
**********************************************************/
void ADC1_Init(void)
{
	ADC1_GPIO_Init();
	ADC1_Hardware_Init();
}

/**********************************************************
功  能：初始化所有外设
参  数：无
返回值：无
**********************************************************/
void Init_All_Periph(void)
{
    Delay_Init();//延时函数初始化
    LED_Hardware_Init();//led初始化
	ILI9341_Init();//LCD初始化
    ADC1_Init();//ADC1初始化
    LCD_SetFont(&Font16x24);
    ILI9341_DispStringLine_EN_CH(LINE(1),"---基于自制拓展板---");//LCD显示字符串
    LCD_SetBackColor(BLACK);
    LCD_SetTextColor(YELLOW);
    LCD_SetFont(&Font8x16);

}

/**********************************************************
功  能：LCD显示
参  数：无
返回值：无
**********************************************************/
void LCD_Show(void)
{
    sprintf(show0," PA4_X:%5d VOL:%.2f V",ADC_Value[0],(float)(ADC_Value[0])*3.3/4095);
    sprintf(show1," PA5_Y:%5d VOL:%.2f V",ADC_Value[1],(float)(ADC_Value[1])*3.3/4095);
    sprintf(show2," PA6_W:%5d VOL:%.2f V",ADC_Value[2],(float)(ADC_Value[2])*3.3/4095);

    ILI9341_DispStringLine_EN(LINE(3),show0);//LCD显示字符串
    ILI9341_DispStringLine_EN(LINE(4),show1);//LCD显示字符串
    ILI9341_DispStringLine_EN(LINE(5),show2);//LCD显示字符串

    Delay_ms(50);
}


