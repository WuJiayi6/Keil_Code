/********************************************************
ʵ�����ƣ�	�ⲿ�ж�ʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ��

ʵ������	�ⲿ�жϣ���������LED

����ʱ�䣺2022-07-12
********************************************************/

#include "stm32f10x.h"  
#include "led.h"
#include "key.h"
#include "bsp_EXTI.h"

int main(void)
{
	LED_Hardware_Init();//LED �� Beep ��ʼ��
	Key_Hardware_Init();//������ʼ��
	EXTI_Hardware_Init();
	Delay_Init();       //��ʱ��ʼ��

	while(1)
	{

	}
}


