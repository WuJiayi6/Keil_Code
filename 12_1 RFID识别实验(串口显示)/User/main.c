/********************************************************
ʵ�����ƣ�	LCD��ʾʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�   ��������Ұ��JP4��չ��
            *1--SDA  <----->PA4
            *2--SCK  <----->PA5
            *3--MOSI <----->PA7
            *4--MISO <----->PA6
            *5--����
            *6--GND <----->GND
            *7--RST <----->PB0
            *8--VCC <----->VCC

ʵ������	������ʾ���ż�д������

����ʱ�䣺  2022-08-17
********************************************************/
#include "RC522.h"
#include "key.h"
int main(void)
{	
	Delay_Init();//��ʱ������ʼ��
    LED_Hardware_Init();
	ILI9341_Init();//LCD��ʼ��
//    ILI9341_DispStringLine_EN_CH(LINE(1),"RFID��ʾʵ��");
    USART1_Hardware_Init(115200);
    RC522_Init();

	while(1)
	{	
        RC522_Test();
        Delay_ms(100);
	}
}
