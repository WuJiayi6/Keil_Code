/********************************************************
实验名称：	PWM舵机实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  舵机接PB8

实验现象：	K1控制舵机转动角度

更新时间：	2022-07-28
********************************************************/
#include "TIM3.h"
#include "TIM4.h"
#include "bsp_ili9341_lcd.h"
#include <stdio.h>

void All_Peripheral_Init(void);//初始化所有外设

int main(void)
{
    char Show[20] = {0};
    All_Peripheral_Init();//初始化所有外设

	while(1)
	{
        Key_Num = Key_Scan(0);
        if(Key_Num == 1)
        {
            Angle += 30;
            if(Angle >180){Angle = 0;}
            
        }PWM_Servo_Test(Angle);//舵机转动角度
        sprintf(Show,"Angle: %.1f  ",Angle);
        ILI9341_DispStringLine_EN(LINE(1),Show);
	}

}

//初始化所有外设
void All_Peripheral_Init(void)
{
    Delay_Init();//延时函数初始化
    ILI9341_Init();//LCD初始化
    Key_Hardware_Init();//按键初始化
    LED_Hardware_Init();//LED初始化
	TIM4_PWM_Hardware_Init(200,7200);//定时器4初始化，T = 20ms
}


