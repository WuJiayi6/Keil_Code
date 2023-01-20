#include "stm32f10x.h"
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_1;//ѡ��˿�
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ������
	
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);//��ʼ������
	
	  
	// ���ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	// �����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*****************ADC����**********************************************/
	// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

	//����ģʽ
	ADC_TypeDefStructure.ADC_Mode = ADC_Mode_Independent;
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_TypeDefStructure.ADC_ScanConvMode = DISABLE;
	//����ת��
	ADC_TypeDefStructure.ADC_ContinuousConvMode = ENABLE;
	//��ֹ�������
	ADC_TypeDefStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//ת������Ҷ���
	ADC_TypeDefStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//1��ת�������й�����
	ADC_TypeDefStructure.ADC_NbrOfChannel = 1;
	//��ʼ������
	ADC_Init(ADC1,&ADC_TypeDefStructure);
	//ADCʹ��
	ADC_Cmd(ADC1,ENABLE);
}


/**********************************************************
��  �ܣ�ADC�ɼ�����
��  ����ch	�ɼ�ͨ��
����ֵ�����һ�ε�ת�����
**********************************************************/
uint16_t ADC_Collect(uint8_t ch)
{
	//ADCX,ADCͨ�����������У� ����ʱ��
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_55Cycles5);
	//�������ADC1����
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	//�ȴ�ת������
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	//�������һ��ת�����
	return ADC_GetConversionValue(ADC1);
}


