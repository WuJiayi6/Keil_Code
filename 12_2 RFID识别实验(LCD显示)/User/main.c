/********************************************************
实验名称：	LCD显示实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  基于自制野火JP4拓展板
            *1--SDA  <----->PA4
            *2--SCK  <----->PA5
            *3--MOSI <----->PA7
            *4--MISO <----->PA6
            *5--悬空
            *6--GND <----->GND
            *7--RST <----->PB0
            *8--VCC <----->VCC

实验现象：	LCD显示具体实验现象

更新时间：  2022-08-17
********************************************************/
#include "RC522.h"
#include "key.h"

extern char LCD_read[50];
extern char LCD_wirte[50];
extern uint8_t LCD_flag;
int main(void)
{	
	Delay_Init();//延时函数初始化
    LED_Hardware_Init();
	ILI9341_Init();//LCD初始化
    ILI9341_DispStringLine_EN_CH(LINE(1),"RFID显示实验");
	ILI9341_DispString_EN_CH(0, 60, "读取:");
	ILI9341_DispString_EN_CH(0, 100, "写入:");
	Delay_ms(50);
    USART1_Hardware_Init(115200);
    RC522_Init();
	
	while(1)
	{	
        RC522_Test();
		if(LCD_flag == 1)
        {
            LED_R(1);
			ILI9341_DispString_EN_CH(40, 60, LCD_read);
			ILI9341_DispString_EN_CH(40, 100, LCD_wirte);
			LCD_flag = 0;
			Delay_ms(1000);Delay_ms(1000);
		}
        LED_R(0);
        ILI9341_DispString_EN_CH(40, 60, "                      ");
        ILI9341_DispString_EN_CH(40, 100, "                      ");
	}
}
