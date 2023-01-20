#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"
#include  <stdio.h>
#include "led.h"
#include "bsp_ili9341_lcd.h"

#define    CHANEL_NUM			3      // ת��ͨ������

static char show0[40]= {0};		//���ڴ�Ŵ�ӡ���ݵ�����
static char show1[40]= {0};		//���ڴ�Ŵ�ӡ���ݵ�����
static char show2[40]= {0};		//���ڴ�Ŵ�ӡ���ݵ�����


extern volatile uint16_t ADC_Value[CHANEL_NUM];

void ADC1_GPIO_Init(void);                  //ADC1_IO��ʼ��
void ADC1_Hardware_Init(void);				//ADC1��ʼ�����ú���
void ADC1_Init(void);                       //ADC1��ʼ��

void Init_All_Periph(void);                //��ʼ����������
void LCD_Show(void);
#endif

