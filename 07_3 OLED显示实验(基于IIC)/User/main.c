/********************************************************
实验名称：	LCD显示实验(基于SPI)

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  接自制野火OLED拓展板

实验现象：	LCD屏显示 "Happy Mid-Autumn Festival"
			OLED屏显示 "中秋快乐"

更新时间：  2022-07-19
********************************************************/
#include "TIM3.h"
#include "OLED_I2C.h"


extern const unsigned char BMP1[];

int main(void)
{	

	Delay_Init();//延时函数初始化
	TIM3_Hardware_Init(10000,7200);//1s
	LED_Hardware_Init();//LED初始化
	I2C_Hardware_Init();//IIC初始化
	OLED_Init();//OLED初始化
	while(1)
	{	
		OLED_Fill(0XFF);//全屏点亮

		Delay_s(2);
		OLED_Clear();//清屏
		OLED_ShowStr(0,3,"Wildfire Tech",1);//测试6*8字符
		OLED_ShowStr(0,5,"Hello wildfire",2);//测试8*16字符
		Delay_s(4);
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
		Delay_s(10);

	}

}


