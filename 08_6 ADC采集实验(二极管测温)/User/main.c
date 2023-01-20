/********************************************************
ʵ�����ƣ�	ADC�ɼ�ʵ��(�����ܲ���)

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ��U16����DS18B20ģ��
			ADC�ɼ�ʹ��PC5-->ADC12_IN15

ʵ������	LCDҺ������ʾ��Ӧ���¶�ֵ

����ʱ�䣺  2022-10-11
********************************************************/
#include "TIM3.h"
#include "adc.h"

int main(void)
{	

	Delay_Init();//��ʱ������ʼ��
	ADC_Hardware_Init();//ADC��ʼ��-->PC5
	ILI9341_Init();//LCD��ʼ��
	DS18B20_Init();//DS18B20��ʼ��
	TIM3_Hardware_Init(100,7200);//��ʱ��10ms
	LED_Hardware_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	while(1)
	{	
		Display_result();
		Delay_ms(500);
		Delay_ms(500);
	}
}


