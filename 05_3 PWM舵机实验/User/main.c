/********************************************************
ʵ�����ƣ�	PWM���ʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  �����PB8

ʵ������	K1���ƶ��ת���Ƕ�

����ʱ�䣺	2022-07-28
********************************************************/
#include "TIM3.h"
#include "TIM4.h"
#include "bsp_ili9341_lcd.h"
#include <stdio.h>

void All_Peripheral_Init(void);//��ʼ����������

int main(void)
{
    char Show[20] = {0};
    All_Peripheral_Init();//��ʼ����������

	while(1)
	{
        Key_Num = Key_Scan(0);
        if(Key_Num == 1)
        {
            Angle += 30;
            if(Angle >180){Angle = 0;}
            
        }PWM_Servo_Test(Angle);//���ת���Ƕ�
        sprintf(Show,"Angle: %.1f  ",Angle);
        ILI9341_DispStringLine_EN(LINE(1),Show);
	}

}

//��ʼ����������
void All_Peripheral_Init(void)
{
    Delay_Init();//��ʱ������ʼ��
    ILI9341_Init();//LCD��ʼ��
    Key_Hardware_Init();//������ʼ��
    LED_Hardware_Init();//LED��ʼ��
	TIM4_PWM_Hardware_Init(200,7200);//��ʱ��4��ʼ����T = 20ms
}


