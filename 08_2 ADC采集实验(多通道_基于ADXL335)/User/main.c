/********************************************************
ʵ�����ƣ�	ADC�ɼ�ʵ�飨��ͨ��_����ADXL335��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ADXL335��̬��������3V3
            X,Y,Z�ֱ��PA4��PA5��PA6

ʵ������	LCDҺ������ʾADCֵ

����ʱ�䣺  2022-07-21
********************************************************/
#include "led.h"
#include "bsp_ili9341_lcd.h"
#include "adc.h"


int main(void)
{	

	Delay_Init();//��ʱ������ʼ��
	ADC_Hardware_Init();
	ILI9341_Init();//LCD��ʼ��
	while(1)
	{	
		adcx	=ADC_Collect(ADC_Channel_4);//������ֵ�������� adx
        adcy	=ADC_Collect(ADC_Channel_5);//������ֵ�������� ady
        adcz	=ADC_Collect(ADC_Channel_6);//������ֵ�������� adz
		sprintf(show1," X :%4d  VOL:%.3f V",adcx,(float)(adcx)*3.3/4095);//תΪ�ַ���
        sprintf(show2," Y :%4d  VOL:%.3f V",adcy,(float)(adcy)*3.3/4095);//תΪ�ַ���
        sprintf(show3," Z :%4d  VOL:%.3f V",adcz,(float)(adcz)*3.3/4095);//תΪ�ַ���
		ILI9341_DispStringLine_EN(LINE(5),show1);//LCD��ʾ�ַ���
        Delay_ms(10);
        ILI9341_DispStringLine_EN(LINE(6),show2);//LCD��ʾ�ַ���
        Delay_ms(10);
        ILI9341_DispStringLine_EN(LINE(7),show3);//LCD��ʾ�ַ���
		Delay_ms(10);
	}
}


