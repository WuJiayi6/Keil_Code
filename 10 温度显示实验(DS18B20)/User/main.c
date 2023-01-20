/********************************************************
ʵ�����ƣ�	��ʪ��ʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ��U16����DS18B20ģ��

ʵ������	LCDҺ������ʾ��Ӧ���¶�ֵ

����ʱ�䣺  2022-07-27
********************************************************/
#include "led.h"
#include "bsp_ili9341_lcd.h"
#include "DS18B20.h"


int main(void)
{	
	float Temperature;
	char buff[100];
	Delay_Init();//��ʱ������ʼ��
	ILI9341_Init();//LCD��ʼ��
	DS18B20_Init();//DS18B20��ʼ��
	while(1)
	{	
		Temperature = DS18B20_GetTemp_SkipROM();
		sprintf((char *)buff,"Temperature :   %.3f",Temperature);
		ILI9341_DispStringLine_EN(LINE(1),buff);

		Delay_ms(500);
	}
}


