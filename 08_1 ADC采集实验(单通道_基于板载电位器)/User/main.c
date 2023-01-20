/********************************************************
实验名称：	ADC实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  转动 R42 电位器以改变采集的电压

实验现象：	LCD液晶屏显示ADC值

更新时间：  2022-07-21
********************************************************/
#include "led.h"
#include "bsp_ili9341_lcd.h"
#include "adc1.h"


int main(void)
{	

	Delay_Init();//延时函数初始化
	ADC1_Hardware_Init();
	ILI9341_Init();//LCD初始化
	while(1)
	{	
		adcx	=ADC1_Collect();//将采样值传给变量 adx
		sprintf(show," X :%4d  VOL:%.3f V",adcx,(float)(adcx)*3.3/4095);//转为字符串
		
		ILI9341_DispStringLine_EN(LINE(5),show);//LCD显示字符串
		Delay_ms(500);
	}
}

