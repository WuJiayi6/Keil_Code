#include "TIM3.h"
/*******************************************************************
��  �ܣ�ͨ�ö�ʱ��3�жϳ�ʼ��  

��  ��: arr���Զ���װֵ
		psc��ʱ��Ԥ��Ƶ��
		��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft.
		Ft=��ʱ������Ƶ��,��λ:Mhz (Ĭ��72MHz)
		
���㹫ʽ��  PWMƵ�ʣ�Freq = CK_PSC / ((PSC+1)*(ARR+1))
			PWMռ�ձȣ�Duty = CCR / (ARR+1)
			PWM�ֱ��ʣ� Reso = 1 / (ARR+1)
			
����ֵ: ��
********************************************************************/
void TIM3_PWM_Hardware_Init(uint16_t arr ,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//��ʱ������
	TIM_OCInitTypeDef TIM_OCInitStructure;            //PWM����
	GPIO_InitTypeDef  GPIO_InitStructure;             //��������
	NVIC_InitTypeDef NVIC_InitStruct;//Ƕ���ж��жϿ������ṹ�����
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);		//ʹ��TIM3ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);     //ʹ�ܸ���IOʱ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж���
    
    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_Init(&NVIC_InitStruct);
    
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);     //������ӳ��TIM3 ->PB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               //����RGB����	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��GPIO

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = arr - 1;	//�Զ���װ��ֵ   ARR �ο�������ܹ�ʽ
	TIM_TimeBaseInitStructure.TIM_Prescaler= psc - 1;//��ʱ����Ƶ     PSC �ο�������ܹ�ʽ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//��ʼ��TIM3

	TIM_OCStructInit(&TIM_OCInitStructure);//Ĭ������TIM_OCInitStructure�ṹ��
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//����Ƚ�ģʽ2
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����ѡ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                // CCR �ο�������ܹ�ʽ
	
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);  //��ʼ��TIM3_OC3ͨ��
											/*	TIM3ͨ��3Ĭ�ϸ���PB0 
													ͨ��4Ĭ�ϸ���PB1  
													ͨ��2�ض���PB5 */
	TIM_Cmd(TIM3,ENABLE);	//ʹ�ܶ�ʱ��3

}


/*******************************************************************
��  �ܣ���ʱ���жϺ��� 
��  ��: ��
����ֵ: ��
********************************************************************/
void TIM3_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 	//����ж�
	{
        LEDR_TOGGLE();

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  	//����жϱ�־λ
}


/*******************************************************************
��  �ܣ�PWM�����ƺ��� 
��  ��: PWM_LED_Breath_Test
��  ��: ��
����ֵ: ��
********************************************************************/
void PWM_LED_Breath_Test(void)
{
    uint8_t i;
	for(i=0;i<=100;i++)
	{
		TIM_SetCompare2(TIM3,i);//����TIM3ͨ��2��ֵ������CCR�Ĵ�����ֵ��
		Delay_ms(10);
	}
	
	for(i=0;i<=100;i++)
	{
		TIM_SetCompare2(TIM3,100-i);
		Delay_ms(10);
	}

}


