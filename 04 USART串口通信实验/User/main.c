/********************************************************
ʵ�����ƣ�	����ͨ��ʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ��USB-B�߽�USBת����

ʵ������	ͨ�����ڷ���ָ�����LED��
			����"left"-->RGB����500msѭ��
			����"spark"-->R����500ms��˸

����ʱ�䣺2022-07-12
********************************************************/

#include "stm32f10x.h"  
#include "usart1.h"


int main(void)
{
	LED_Hardware_Init();//LED �� Beep ��ʼ��
	USART1_Hardware_Init(115200);//���ڳ�ʼ��
	Delay_Init();       //��ʱ��ʼ��
	USART1_Send_String("please send \"left\" or \"spark\"\n ");
	while(1)
	{
		USART_Send_Command();
	}
}




