/********************************************************
ʵ�����ƣ�	�������ʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ��

ʵ������	��������LED

����ʱ�䣺2022-07-10
********************************************************/


#include "stm32f10x.h"  
#include "led.h"
#include "key.h"

int main(void)
{
	LED_Hardware_Init();//LED �� Beep ��ʼ��
	Key_Hardware_Init();//������ʼ��
	Delay_Init();       //��ʱ��ʼ��

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


