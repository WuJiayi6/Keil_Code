/********************************************************
ʵ�����ƣ�	LCD��ʾʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  LCD ����оƬ LCDID_ST7789V   0x8552

ʵ������	LCDҺ������ʾ "Hello World" �� 24Сʱʱ��

����ʱ�䣺  2022-07-19
********************************************************/
#include "delay.h"
#include "TIM3.h"
#include "bsp_ili9341_lcd.h"

int main(void)
{	

	Delay_Init();//��ʱ������ʼ��
	TIM3_Hardware_Init(10000-1,7200-1);//��ʱ��1s
	ILI9341_Init();//LCD��ʼ��
    LCD_SetFont(&Font8x16);//��ʾ�����С
	ILI9341_DispStringLine_EN_CH(LINE(1),"���");
    LCD_SetFont(&Font16x24);//��ʾ�����С
    ILI9341_DispStringLine_EN(LINE(2),"Hello World!");
	while(1)
	{	
		TIM3_24_Clock();
	}

}


