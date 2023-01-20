#include "TIM3.h"
 
/*******************************************************************
��  �ܣ�ͨ�ö�ʱ��3�жϳ�ʼ��  
��  ��: arr���Զ���װֵ
		psc��ʱ��Ԥ��Ƶ��
		��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
		Ft=��ʱ������Ƶ��,��λ:Mhz
����ֵ: ��
********************************************************************/

void TIM3_Hardware_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);				//ʹ��TIM3ʱ��

	TIM_TimeBaseInitStructure.TIM_Period = arr;						//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;					//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//��ʼ��TIM3

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);						//����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE);	//ʹ�ܶ�ʱ��3

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;					//��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;		//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;				//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************
��  �ܣ���ʱ���жϺ��� 
��  ��: ��
����ֵ: ��
********************************************************************/

void TIM3_IRQHandler(void)
{
    static uint16_t h=23,m=59,s=55;
    char time[30];
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 	//����ж�
	{
		LEDR_TOGGLE();
        s++;//�����жϣ����һ
    //24Сʱʱ��    
     if(s==60)
    {
        m+=1;
        s=0;    
    }
    if(m==60)
    {
        h+=1;
        m=0;     
    }
    if(h==24)
    {
        h=0;
    }
        sprintf(time,"time: %.2d:%.2d:%.2d \n",h,m,s);	//ѭ�����ڴ�ӡ
        USART1_Send_String(time);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  	//����жϱ�־λ
}

