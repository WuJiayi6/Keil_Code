#include "TIM4.h"
/*******************************************************************
��  �ܣ�ͨ�ö�ʱ��4�жϳ�ʼ��  

��  ��: arr���Զ���װֵ
		psc��ʱ��Ԥ��Ƶ��
		��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft.
		Ft=��ʱ������Ƶ��,��λ:Mhz (Ĭ��72MHz)
		
���㹫ʽ��  PWMƵ�ʣ�Freq = CK_PSC / ((PSC+1)*(ARR+1))
			PWMռ�ձȣ�Duty = CCR / (ARR+1)
			PWM�ֱ��ʣ� Reso = 1 / (ARR+1)
			
����ֵ: ��
********************************************************************/
void TIM4_PWM_Hardware_Init(uint16_t arr ,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//��ʱ������
	TIM_OCInitTypeDef TIM_OCInitStructure;            //PWM����
	GPIO_InitTypeDef  GPIO_InitStructure;             //��������
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);		//ʹ��TIM3ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;               //����PB8	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��GPIO

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = arr - 1;	//�Զ���װ��ֵ   ARR �ο�������ܹ�ʽ
	TIM_TimeBaseInitStructure.TIM_Prescaler= psc - 1;//��ʱ����Ƶ     PSC �ο�������ܹ�ʽ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);				//��ʼ��TIM4

	TIM_OCStructInit(&TIM_OCInitStructure);//Ĭ������TIM_OCInitStructure�ṹ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//����Ƚ�ģʽ2 (�������Ϊ������ת)
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//����ѡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                // CCR �ο�������ܹ�ʽ
	
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);  //��ʼ��TIM4_OC3ͨ��
	
    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_Cmd(TIM4,ENABLE);	//ʹ�ܶ�ʱ��3
}

#if 0
/*******************************************************************
��  �ܣ���ʱ��4�жϺ��� 
��  ��: ��
����ֵ: ��
********************************************************************/
void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) 	//����ж�
	{


	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  	//����жϱ�־λ
}
#endif
/*******************************************************************
��  �ܣ�	����TIM4ͨ��3��CCR
��  ����    
��  ����	��
����ֵ��	��
********************************************************************/
void TIM4_PWM_SetCompare3(float Compare)
{
	TIM_SetCompare3(TIM4,Compare);
}


/*******************************************************************
��  �ܣ�PWM������� 
��  ��: ��
����ֵ: ��
********************************************************************/
float Angle = 0;
void PWM_Servo_Test(float Angle)
{
    Angle = ((Angle /180)*20 + 5);
    TIM4_PWM_SetCompare3(Angle);

}


