/********************************************************
ʵ�����ƣ�	�����������ʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��
			����ULN2003���������

Ӳ�����ߣ�  PA4567�Լ�5V����

ʵ������	����KEY1�����ת������KEY2�����ת

����ʱ�䣺	2022-11-17
********************************************************/ 
#include "Motor.h"

int main(void)
{
	Delay_Init();       //��ʱ��ʼ��
	Key_Hardware_Init();//������ʼ��
	Motor_Hardware_Init();//LED �� Beep ��ʼ��
	EXTI_Hardware_Init();//�ⲿ�жϳ�ʼ��
	LED_Hardware_Init();//LED��ʼ��
	
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


