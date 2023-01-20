/********************************************************
ʵ�����ƣ�	ADCʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ת�� R42 ��λ���Ըı�ɼ��ĵ�ѹ

ʵ������	LCDҺ������ʾADCֵ

����ʱ�䣺  2022-07-21
********************************************************/
#include "led.h"
#include "bsp_ili9341_lcd.h"
#include "adc1.h"


int main(void)
{	

	Delay_Init();//��ʱ������ʼ��
	ADC1_Hardware_Init();
	ILI9341_Init();//LCD��ʼ��
	while(1)
	{	
		adcx	=ADC1_Collect();//������ֵ�������� adx
		sprintf(show," X :%4d  VOL:%.3f V",adcx,(float)(adcx)*3.3/4095);//תΪ�ַ���
		
		ILI9341_DispStringLine_EN(LINE(5),show);//LCD��ʾ�ַ���
		Delay_ms(500);
	}
}

