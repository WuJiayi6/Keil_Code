/********************************************************
实验名称：	步进电机控制实验

硬件模块：	野火指南者核心板（STM32F103VET6）
			自制ULN2003电机驱动板

硬件接线：  PA4567以及5V供电

实验现象：	按下KEY1电机正转，按下KEY2电机反转

更新时间：	2022-11-17
********************************************************/ 
#include "Motor.h"

int main(void)
{
	Delay_Init();       //延时初始化
	Key_Hardware_Init();//按键初始化
	Motor_Hardware_Init();//LED 和 Beep 初始化
	EXTI_Hardware_Init();//外部中断初始化
	LED_Hardware_Init();//LED初始化
	
	while(1)
	{
		if(Motor_flag){
			Motor_Foreward();
		}
		else{
			Motor_Reversal();
		}
		
	}
}


