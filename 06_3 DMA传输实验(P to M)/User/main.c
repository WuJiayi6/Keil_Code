/********************************************************
ʵ�����ƣ�	DMA����ʵ�飨Peripheral->SRAM��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ��

ʵ������	�����򴮿ڷ������ݣ����ݻ᷵�ص�����

����ʱ�䣺  2022-08-7
********************************************************/
#include "dma.h"
#include "led.h"
#include "usart1.h"

int main(void)
{
	Delay_Init();//��ʱ������ʼ��
	LED_Hardware_Init();    //LED �� Beep ��ʼ��
	USART1_Hardware_Init(115200);//DMA_USART ��ʼ��
	DMA_Hardware_Init();             //DMA��ʼ��
    USART1_Send_String("DMA(P->M)ģʽ�������򴮿ڷ������ݣ����ݻ᷵�ص�����\r\n");

	while(1)
	{	
		LED_R(0);
	}

}

