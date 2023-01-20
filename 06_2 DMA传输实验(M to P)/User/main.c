/********************************************************
ʵ�����ƣ�	DMA����ʵ�飨FLASH->Peripheral��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ��

ʵ������	SRAM(M)ͨ�����ڽ�SendBuff[]���鷢����ͬʱ�Ƶ�һֱ��˸��
			����ͨ��DMA(M->P)���䲻ռ��MCU

����ʱ�䣺2022-07-14
********************************************************/
#include "dma.h"
#include "led.h"
#include "usart1.h"

int main(void)
{
	uint16_t i = 0;//����д��SendBuff[]����
	
	LED_Hardware_Init();    //LED �� Beep ��ʼ��
	DMA_USART1_Hardware_Init(115200);//DMA_USART ��ʼ��
	DMA_Hardware_Init();             //DMA��ʼ��
	Delay_Init();//��ʱ������ʼ��

	for(i=0;i<USART1_RX_SIZE;i++)
		usart1_buffer[i] = '1';
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	while(1)
	{	
		
		LED_Y(0);
		Delay_ms(500);
		LED_W(1);//�ر����еĵ�
		Delay_ms(500);

	}

}


