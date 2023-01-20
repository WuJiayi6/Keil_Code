#include "bsp_EXTI.h"


/*******************************************************************
��  �ܣ����ⲿ�ж�  
��  ��: ��
����ֵ: ��
********************************************************************/
void EXTI_Hardware_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStruct;//EXTI�ṹ�����
	NVIC_InitTypeDef NVIC_InitStruct;//Ƕ���ж��жϿ������ṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//��ʱ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�����ж���
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;//�ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority  = 0;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);//��ʼ������
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;//�ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority  = 0;//��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);//��ʼ������
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);//�ж��߹���,PA0,�ж���Ϊkey1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);//�ж��߹���,PC13,�ж���Ϊkey2
	//����EXTI
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;//PA0,key1
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger  = EXTI_Trigger_Rising;//����ѡ��������or�½��ش����жϣ�
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ���ж����μĴ���
	EXTI_Init(&EXTI_InitStruct);//��ʼ������
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line13;//PC13,key2
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTI_InitStruct.EXTI_Trigger  = EXTI_Trigger_Rising;//����ѡ��������or�½��ش����жϣ�
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//ʹ���ж����μĴ���
	EXTI_Init(&EXTI_InitStruct);//��ʼ������
}


/*******************************************************************
��  �ܣ��ⲿ�ж�0������  
��  ��: ��
����ֵ: ��
********************************************************************/
void EXTI0_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line0) !=RESET)
	{
		time = 100;
	}
	EXTI_ClearITPendingBit(EXTI_Line0);//����жϱ�־
}


/*******************************************************************
��  �ܣ��ⲿ�ж�13������
��  ��: ��
����ֵ: ��
********************************************************************/
void EXTI15_10_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line13) !=RESET)
	{
		time = 500;
	}
	EXTI_ClearITPendingBit(EXTI_Line13);//����жϱ�־
}


/*******************************************************************
��  �ܣ�LEDƵ����ʾ����
��  ��: ��
����ֵ: ��
********************************************************************/
void LED_Frequency_Display(void)
{
	
	if(time)//���������ⲿ�ж�
	{
		if(time == 100)//����1������
		{
			flag = 1;
		}
		else if(time == 500)//����2������
		{
			flag = 2;
		}
	}
	switch(flag)
	{
		case 1:LED_R(0);Delay_ms(100);LED_R(1);Delay_ms(100);break;
		case 2:LED_G(0);Delay_ms(500);LED_G(1);Delay_ms(500);break;
		default:break;
	}
}

