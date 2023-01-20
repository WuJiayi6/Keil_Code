/********************************************************
ʵ�����ƣ�	LCD��ʾʵ��(����SPI)

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ������Ұ��OLED��չ��

ʵ������	LCD����ʾ "Happy Mid-Autumn Festival"
			OLED����ʾ "�������"

����ʱ�䣺  2022-07-19
********************************************************/
#include "TIM3.h"
#include "OLED_I2C.h"


extern const unsigned char BMP1[];

int main(void)
{	

	Delay_Init();//��ʱ������ʼ��
	TIM3_Hardware_Init(10000,7200);//1s
	LED_Hardware_Init();//LED��ʼ��
	I2C_Hardware_Init();//IIC��ʼ��
	OLED_Init();//OLED��ʼ��
	while(1)
	{	
		OLED_Fill(0XFF);//ȫ������

		Delay_s(2);
		OLED_Clear();//����
		OLED_ShowStr(0,3,"Wildfire Tech",1);//����6*8�ַ�
		OLED_ShowStr(0,5,"Hello wildfire",2);//����8*16�ַ�
		Delay_s(4);
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
		Delay_s(10);

	}

}


