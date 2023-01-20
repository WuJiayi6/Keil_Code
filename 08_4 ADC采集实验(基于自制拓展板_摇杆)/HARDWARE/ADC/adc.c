#include "adc.h"

volatile uint16_t ADC_Value[CHANEL_NUM]={0,0,0};

/**********************************************************
��  �ܣ�ADC GPIO ��ʼ��
��  ������
����ֵ����
**********************************************************/
void ADC1_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE );// �� ADC IO�˿�ʱ��
    // ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);// ��ʼ�� ADC IO	
    
}

/**********************************************************
��  �ܣ�����ADC����ģʽ
��  ������
����ֵ����
**********************************************************/
void ADC1_Hardware_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);// ��DMAʱ��
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_ADC1,ENABLE);// ��ADCʱ��
	
	DMA_DeInit(DMA1_Channel1);// ��λDMA������
	
	/*********���� DMA ��ʼ���ṹ��**********/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;// �洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// ����Դ��������
	DMA_InitStructure.DMA_BufferSize = CHANEL_NUM;// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // �洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	// ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);// ��ʼ��DMA
	DMA_Cmd(DMA1_Channel1,ENABLE);// ʹ�� DMA ͨ��
	
	/*********ADC ģʽ����**********/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;// ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;// ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// ת������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = CHANEL_NUM;	// ת��ͨ������
	
	ADC_Init(ADC1, &ADC_InitStructure);// ��ʼ��ADC
    
	// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ����ADC ͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_55Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);// ʹ��ADC DMA ����
	ADC_Cmd(ADC1, ENABLE);// ����ADC ������ʼת��

    /*********��ʼ��ADC У׼�Ĵ���**********/    
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));// �ȴ�У׼�Ĵ�����ʼ�����
	ADC_StartCalibration(ADC1);// ADC��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC1));// �ȴ�У׼���
	 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);// ����û�в����ⲿ����������ʹ���������ADCת��
}


/**********************************************************
��  �ܣ�ADC��ʼ��
��  ������
����ֵ����
**********************************************************/
void ADC1_Init(void)
{
	ADC1_GPIO_Init();
	ADC1_Hardware_Init();
}

/**********************************************************
��  �ܣ���ʼ����������
��  ������
����ֵ����
**********************************************************/
void Init_All_Periph(void)
{
    Delay_Init();//��ʱ������ʼ��
    LED_Hardware_Init();//led��ʼ��
	ILI9341_Init();//LCD��ʼ��
    ADC1_Init();//ADC1��ʼ��
    LCD_SetFont(&Font16x24);
    ILI9341_DispStringLine_EN_CH(LINE(1),"---����������չ��---");//LCD��ʾ�ַ���
    LCD_SetBackColor(BLACK);
    LCD_SetTextColor(YELLOW);
    LCD_SetFont(&Font8x16);

}

/**********************************************************
��  �ܣ�LCD��ʾ
��  ������
����ֵ����
**********************************************************/
void LCD_Show(void)
{
    sprintf(show0," PA4_X:%5d VOL:%.2f V",ADC_Value[0],(float)(ADC_Value[0])*3.3/4095);
    sprintf(show1," PA5_Y:%5d VOL:%.2f V",ADC_Value[1],(float)(ADC_Value[1])*3.3/4095);
    sprintf(show2," PA6_W:%5d VOL:%.2f V",ADC_Value[2],(float)(ADC_Value[2])*3.3/4095);

    ILI9341_DispStringLine_EN(LINE(3),show0);//LCD��ʾ�ַ���
    ILI9341_DispStringLine_EN(LINE(4),show1);//LCD��ʾ�ַ���
    ILI9341_DispStringLine_EN(LINE(5),show2);//LCD��ʾ�ַ���

    Delay_ms(50);
}


