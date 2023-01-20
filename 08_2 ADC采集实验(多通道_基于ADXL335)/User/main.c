/********************************************************
实验名称：	ADC采集实验（多通道_基于ADXL335）

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  ADXL335姿态传感器接3V3
            X,Y,Z分别接PA4，PA5，PA6

实验现象：	LCD液晶屏显示ADC值

更新时间：  2022-07-21
********************************************************/
#include "led.h"
#include "bsp_ili9341_lcd.h"
#include "adc.h"


int main(void)
{	

	Delay_Init();//延时函数初始化
	ADC_Hardware_Init();
	ILI9341_Init();//LCD初始化
	while(1)
	{	
		adcx	=ADC_Collect(ADC_Channel_4);//将采样值传给变量 adx
        adcy	=ADC_Collect(ADC_Channel_5);//将采样值传给变量 ady
        adcz	=ADC_Collect(ADC_Channel_6);//将采样值传给变量 adz
		sprintf(show1," X :%4d  VOL:%.3f V",adcx,(float)(adcx)*3.3/4095);//转为字符串
        sprintf(show2," Y :%4d  VOL:%.3f V",adcy,(float)(adcy)*3.3/4095);//转为字符串
        sprintf(show3," Z :%4d  VOL:%.3f V",adcz,(float)(adcz)*3.3/4095);//转为字符串
		ILI9341_DispStringLine_EN(LINE(5),show1);//LCD显示字符串
        Delay_ms(10);
        ILI9341_DispStringLine_EN(LINE(6),show2);//LCD显示字符串
        Delay_ms(10);
        ILI9341_DispStringLine_EN(LINE(7),show3);//LCD显示字符串
		Delay_ms(10);
	}
}


