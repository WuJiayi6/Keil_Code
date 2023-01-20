/********************************************************
实验名称：	定时器中断实验

硬件模块：	野火指南者核心板（STM32F103VET6）

硬件接线：  HC_SR04超声波模块Trig->PA7   Echo->PA4

实验现象：	LCD显示测量距离

更新时间：  2022-08-4
********************************************************/
#include "bsp_ili9341_lcd.h"
#include "TIM4.h"

int main(void)
{
	char printf_buf[20]={0};
	uint16_t show_time = 0;
	float show_distance = 0;
    
    Delay_Init();//延时函数初始化
    LED_Hardware_Init();
    TIM4_Hardware_Init(10,72);//10us
    Tran_EXTI_Init();//外部中断初始化		
    ILI9341_Init();//LCD初始化
    ILI9341_DispStringLine_EN_CH(LINE(2),"超声波测距实验");//屏幕显示
	while(1)
	{

        Ultrasonic_Wave();//进行一次测距
		Delay_ms(20);						//等待接受获得距离值(单位mm)
		show_time++;
		show_distance+=dis_temp;
		if(show_time==10)					//采集十次获得平均值
		{
            if((show_distance/100)>20)
                show_distance=show_distance+100;
			if((10<show_distance/100)&&(show_distance/100)<15)
				show_distance=show_distance+50;
			sprintf(printf_buf,"测量距离：%5.1f CM  ",show_distance/100);//float转string,(单位cm)
			ILI9341_DispStringLine_EN_CH(LINE(5),printf_buf);//屏幕显示string
			show_time = 0;
			show_distance = 0;
		}
        
	}

}

