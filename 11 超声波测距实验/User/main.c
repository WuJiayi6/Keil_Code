/********************************************************
ʵ�����ƣ�	��ʱ���ж�ʵ��

Ӳ��ģ�飺	Ұ��ָ���ߺ��İ壨STM32F103VET6��

Ӳ�����ߣ�  HC_SR04������ģ��Trig->PA7   Echo->PA4

ʵ������	LCD��ʾ��������

����ʱ�䣺  2022-08-4
********************************************************/
#include "bsp_ili9341_lcd.h"
#include "TIM4.h"

int main(void)
{
	char printf_buf[20]={0};
	uint16_t show_time = 0;
	float show_distance = 0;
    
    Delay_Init();//��ʱ������ʼ��
    LED_Hardware_Init();
    TIM4_Hardware_Init(10,72);//10us
    Tran_EXTI_Init();//�ⲿ�жϳ�ʼ��		
    ILI9341_Init();//LCD��ʼ��
    ILI9341_DispStringLine_EN_CH(LINE(2),"���������ʵ��");//��Ļ��ʾ
	while(1)
	{

        Ultrasonic_Wave();//����һ�β��
		Delay_ms(20);						//�ȴ����ܻ�þ���ֵ(��λmm)
		show_time++;
		show_distance+=dis_temp;
		if(show_time==10)					//�ɼ�ʮ�λ��ƽ��ֵ
		{
            if((show_distance/100)>20)
                show_distance=show_distance+100;
			if((10<show_distance/100)&&(show_distance/100)<15)
				show_distance=show_distance+50;
			sprintf(printf_buf,"�������룺%5.1f CM  ",show_distance/100);//floatתstring,(��λcm)
			ILI9341_DispStringLine_EN_CH(LINE(5),printf_buf);//��Ļ��ʾstring
			show_time = 0;
			show_distance = 0;
		}
        
	}

}

