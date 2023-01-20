/********************************************************
ʵ�����ƣ�	LCD��ʾʵ��(����SPI)

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  ������Ұ��OLED��չ��

ʵ������	LCD����ʾ "Happy Mid-Autumn Festival"
			OLED����ʾ "�������"

����ʱ�䣺  2022-07-19
********************************************************/
#include "TIM3.h"
#include "gui.h"
#include "bmp.h"

int main(void)
{	

	Delay_Init();//��ʱ������ʼ��
	ILI9341_Init();
	TIM3_Hardware_Init(10000,7200);//1s
	LED_Hardware_Init();//LED��ʼ��
    OLED_Init();		//��ʼ��OLED  
    OLED_Clear(0);      //������ȫ�ڣ�
	/***********************************************************************************/
	/**///�ٸ��ϵ磬��ƷоƬ�ڲ�������Ҫʱ�䣬��ʱ100~500ms,����500ms
	/***********************************************************************************/
	Delay_ms(500);	
	GUI_ShowCHinese(0,10,24,"�������",1);
    OLED_WR_Byte(0x2E,OLED_CMD); //�رչ���
    OLED_WR_Byte(0x27,OLED_CMD); //ˮƽ��������ҹ��� 26/27
    OLED_WR_Byte(0x00,OLED_CMD); //�����ֽ�
    OLED_WR_Byte(0x00,OLED_CMD); //��ʼҳ 0
    OLED_WR_Byte(0x07,OLED_CMD); //����ʱ����
    OLED_WR_Byte(0x07,OLED_CMD); //��ֹҳ 2
    OLED_WR_Byte(0x00,OLED_CMD); //�����ֽ�
    OLED_WR_Byte(0xFF,OLED_CMD); //�����ֽ�
	OLED_WR_Byte(0x2F,OLED_CMD); //��������
	LCD_SetFont(&Font8x16);
	ILI9341_DispStringLine_EN(LINE(5),"Happy Mid-Autumn Festival");//LCD��ʾ
	while(1)
	{	
		LCD_SetFont(&Font16x24);
		TIM3_Counter();
	}

}


