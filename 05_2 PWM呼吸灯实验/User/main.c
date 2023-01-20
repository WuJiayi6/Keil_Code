/********************************************************
实验名称：	PWM呼吸灯实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  无

实验现象：	使用TIM3发出PWM波，使R灯(PB5)实现呼吸效果

更新时间：  2022-07-26
********************************************************/
#include "led.h" 
#include "TIM3.h"
#include "delay.h"

int main(void)
{

	Delay_Init();//延时函数初始化
	TIM3_PWM_Hardware_Init(100,720);//定时器3初始化，1ms,1KHz
    
	while(1)
	{
		PWM_LED_Breath_Test();
	}

}


