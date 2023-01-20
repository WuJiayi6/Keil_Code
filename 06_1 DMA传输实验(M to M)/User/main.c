/********************************************************
ʵ�����ƣ�	DMA����ʵ�飨FLASH->SRAM��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ��

ʵ������	��λ���Ƶƣ�DMA����������̵�

����ʱ�䣺2022-07-13
********************************************************/
#include "dma.h"
#include "led.h"

int main(void)
{
	
	LED_Hardware_Init();    //LED �� Beep ��ʼ��
	DMA_Hardware_Init();//DMA��ʼ��
	Delay_Init();//��ʱ������ʼ��
	LED_Y(0);
	Delay_ms(1000);
	LED_W(1);//�ر����еĵ�
	while(1)
	{	
		TransferStatus = Buffercmp(aSRC_Const_Buffer,aDST_Buffer,BUFFER_SIZE);
		/* �ȴ�DMA������� */
		while(DMA_GetFlagStatus(DMA1_FLAG_TC6)==RESET) {}
		if(TransferStatus)
			LED_G(0);
		else
			LED_R(0);
	}

}


