#include "TIM5.h"
 
/*******************************************************************
��  �ܣ�ͨ�ö�ʱ��5�жϳ�ʼ��  
��  ��: arr���Զ���װֵ
		psc��ʱ��Ԥ��Ƶ��
		��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
		Ft=��ʱ������Ƶ��,��λ:Mhz
����ֵ: ��
********************************************************************/

void TIM5_Hardware_Init(uint16_t arr,uint16_t psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	//ʹ��GPIOA��TIM5��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//����������
	
	//ʱ����ʼ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_Period=arr-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	
	//���벶���ʼ��
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;//PA1-->TIM5_CH2
	TIM_ICInitStruct.TIM_ICFilter=0x3;//2^3=8���˲�
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;//�����ز���
	TIM_ICInitStruct.TIM_ICPrescaler=TIM_ICPSC_DIV1;//����Ƶ����
	TIM_ICInitStruct.TIM_ICSelection=TIM_ICSelection_DirectTI;//ֱ��ӳ�䵽TI2
	TIM_ICInit(TIM5,&TIM_ICInitStruct);//TIM5ͨ��2��ʼ��
	
	TIM_Cmd(TIM5,ENABLE);//ʹ�ܶ�ʱ��5
}
