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
	ADC_InitTypeDef ADC_TypeDefStructure;//����ʱ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//ѡ��˿�
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ������
	GPIO_Init(GPIOA,&GPIO_TypeDefStructure);//��ʼ������
    GPIO_TypeDefStructure.GPIO_Pin = GPIO_Pin_1;//ѡ��˿�
	GPIO_TypeDefStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ������
	GPIO_Init(GPIOC,&GPIO_TypeDefStructure);//��ʼ������

	/*****************ADC����**********************************************/
	// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
    //ע��ADCCLK�����14MHz������ֻ����6��Ƶ����8��Ƶ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	ADC_TypeDefStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_TypeDefStructure.ADC_ScanConvMode = DISABLE;
	ADC_TypeDefStructure.ADC_ContinuousConvMode = DISABLE;//����ת��
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
    

}

/**********************************************************
��  �ܣ�ADC�ɼ�����
��  ����ADC_Channel	�ɼ�ͨ��
����ֵ�����һ�ε�ת�����
**********************************************************/
uint16_t ADC_Collect(uint8_t ADC_Channel)
{
    //ADCX,ADCͨ�����������У� ����ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_55Cycles5);
    //�������ADC1����
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	//�������һ��ת�����
	return ADC_GetConversionValue(ADC1);
}


