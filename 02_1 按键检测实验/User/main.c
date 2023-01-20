/********************************************************
实验名称：	按键检测实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  无

实验现象：	按键控制LED

更新时间：2022-07-10
********************************************************/


#include "stm32f10x.h"  
#include "led.h"
#include "key.h"

int main(void)
{
	LED_Hardware_Init();//LED 和 Beep 初始化
	Key_Hardware_Init();//按键初始化
	Delay_Init();       //延时初始化

	while(1)
	{
        switch(Key_Scan(0))
        {
            case 1:LEDR_TOGGLE();break;
            case 2:LEDG_TOGGLE();break;
			default:break;
        }
		
	}
}


