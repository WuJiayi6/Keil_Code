/********************************************************
实验名称：	LCD显示实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：   基于自制野火JP4拓展板
            *1--SDA  <----->PA4
            *2--SCK  <----->PA5
            *3--MOSI <----->PA7
            *4--MISO <----->PA6
            *5--悬空
            *6--GND <----->GND
            *7--RST <----->PB0
            *8--VCC <----->VCC

实验现象：	串口显示卡号及写卡内容

更新时间：  2022-08-17
********************************************************/
#include "RC522.h"
#include "key.h"
int main(void)
{	
	Delay_Init();//延时函数初始化
    LED_Hardware_Init();
	ILI9341_Init();//LCD初始化
//    ILI9341_DispStringLine_EN_CH(LINE(1),"RFID显示实验");
    USART1_Hardware_Init(115200);
    RC522_Init();

	while(1)
	{	
        RC522_Test();
        Delay_ms(100);
	}
}
