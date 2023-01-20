#include "adc.h"


/**********************************************************
��  �ܣ�ADC1��ʼ�����ú���
��  ������
����ֵ����
**********************************************************/
void ADC_Hardware_Init(void)
{
	GPIO_InitTypeDef GPIO_TypeDefStructure;
	ADC_InitTypeDef ADC_TypeDefStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_5;//ѡ��˿�
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ������
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);//��ʼ������
	
	// �����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*****************  ADC����  ******************/
	// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

	
	ADC_TypeDefStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_TypeDefStructure.ADC_ScanConvMode = DISABLE;// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_TypeDefStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
	ADC_TypeDefStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ֹ�������
	ADC_TypeDefStructure.ADC_DataAlign = ADC_DataAlign_Right;//ת������Ҷ���
	ADC_TypeDefStructure.ADC_NbrOfChannel = 1;//1��ת�������й�����
	ADC_Init(ADC1,&ADC_TypeDefStructure);//��ʼ������
	
	ADC_Cmd(ADC1,ENABLE);//ADCʹ��
}


/**********************************************************
��  �ܣ�ADC�ɼ�����
��  ������
����ֵ�����һ�ε�ת�����
**********************************************************/
uint16_t ADC_Collect(void)
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,1,ADC_SampleTime_55Cycles5);//ADCX,ADCͨ�����������У� ����ʱ��
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�������ADC1����
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);//�������һ��ת�����
}


/**********************************************************
��  �ܣ�ͨ��LCD��ʾת�����
��  ������
����ֵ����
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



