/********************************************************
实验名称：	ADC采集实验(二极管测温)

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  在U16接入DS18B20模块
			ADC采集使用PC5-->ADC12_IN15

实验现象：	LCD液晶屏显示对应的温度值

更新时间：  2022-10-11
********************************************************/
#include "TIM3.h"
#include "adc.h"

int main(void)
{	

	Delay_Init();//延时函数初始化
	ADC_Hardware_Init();//ADC初始化-->PC5
	ILI9341_Init();//LCD初始化
	DS18B20_Init();//DS18B20初始化
	TIM3_Hardware_Init(100,7200);//定时器10ms
	LED_Hardware_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	while(1)
	{	
		Display_result();
		Delay_ms(500);
		Delay_ms(500);
	}
}


