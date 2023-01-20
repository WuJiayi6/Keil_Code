#include "stm32f10x.h"
#include "adc1.h"

/**********************************************************
��  �ܣ�ADC1��ʼ�����ú���
��  ������
����ֵ����
**********************************************************/
void ADC1_Hardware_Init(void)
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// �����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*****************ADC����**********************************************/
	// ����ADCʱ��ΪPCLK2��6��Ƶ����12MHz
    //ע��ADCCLK�����14MHz������ֻ����6��Ƶ����8��Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 

	//ADCX,ADCͨ�����������У� ����ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_55Cycles5);
	
	ADC_TypeDefStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_TypeDefStructure.ADC_ScanConvMode = DISABLE;
	ADC_TypeDefStructure.ADC_ContinuousConvMode = ENABLE;//��ͨ������ת����ɨ��ģʽ
	ADC_TypeDefStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ֹ�������
	ADC_TypeDefStructure.ADC_DataAlign = ADC_DataAlign_Right;//ת������Ҷ���
	ADC_TypeDefStructure.ADC_NbrOfChannel = 1;//1��ת�������й�����
	ADC_Init(ADC1,&ADC_TypeDefStructure);//��ʼ������
	//ADCʹ��
	ADC_Cmd(ADC1,ENABLE);  
    
    //У׼
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1) ==SET );
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1) ==SET);
    
	//�������ADC1����
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}


/**********************************************************
��  �ܣ�ADC�ɼ�����
��  ����ch	�ɼ�ͨ��
����ֵ�����һ�ε�ת�����
**********************************************************/
uint16_t ADC1_Collect(void)
{

	//�������һ��ת�����
	return ADC_GetConversionValue(ADC1);
}


