#include "TIM4.h"

uint32_t status = 0;						// ����ֵ
uint32_t real_time = 0;					// ����ֵ
float dis_temp = 0;						// �������ֵ
/*******************************************************************
��  �ܣ�ͨ�ö�ʱ��4�жϳ�ʼ������ģ��TRIG�˿ڷ��ʹ���10us�ĸߵ�ƽ�ź� 
��  ��: arr���Զ���װֵ
		psc��ʱ��Ԥ��Ƶ��
		��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
		Ft=��ʱ������Ƶ��,��λ:72Mhz
����ֵ: ��
********************************************************************/
void TIM4_Hardware_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);				//ʹ��TIM4ʱ��
    
	TIM_TimeBaseInitStructure.TIM_Period = arr;						//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;					//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Down;	//���¼���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӷ�Ƶ����
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//�ظ���������
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);				//��ʼ��TIM4
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж���
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//ѡ���ж���
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// �ж���ʹ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //��Ӧ���ȼ�  
    NVIC_Init(&NVIC_InitStructure);//�ж������ȼ���ʼ��
    
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//����ʱ��4�����ж�
	TIM_Cmd(TIM4,ENABLE);	//ʹ�ܶ�ʱ��4
}

/*******************************************************************
��  �ܣ���ʱ��4�жϷ������ 
��  ��: ��
����ֵ: ��
********************************************************************/	 
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)      //�ж϶�ʱ��4�����жϴ���
	{
		status++;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	//�����ʱ��4�����жϴ���
}


/*******************************************************************
��  �ܣ��ⲿ�жϳ�ʼ������
��  ��: ��
����ֵ: ��
********************************************************************/
void Tran_EXTI_Init(void)
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_TypeStructure;
	EXTI_InitTypeDef EXTI_TypeDefStructure;
	NVIC_InitTypeDef NVIC_TypeDefStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����GPIO_TRANʱ��
	
    //Trig
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_7;//ѡ��˿�
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;//ͨ���������
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;//ѡ���������ٶ�  50MHz
	GPIO_Init(GPIOA,&GPIO_TypeStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);//Ĭ������
    
    //Echo
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(GPIOA,&GPIO_TypeStructure);//��ʼ���˿�
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//Ĭ������
    
	//�ж��߹���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);//�ж��߹���,PA4
	
	//�ⲿ�ж�����
	EXTI_TypeDefStructure.EXTI_Line = EXTI_Line4;				//�ж���
	EXTI_TypeDefStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//����ʽ
	EXTI_TypeDefStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI_TypeDefStructure.EXTI_LineCmd = ENABLE;				//ʹ���ж���
	EXTI_Init(&EXTI_TypeDefStructure);	
	
	//�ⲿ�ж����� EXTI4_IRQn ���ȼ�����
	NVIC_TypeDefStructure.NVIC_IRQChannel = EXTI4_IRQn;			//�ж�����ѡ��
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority  = 0;		//��Ӧ���ȼ�
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;			//ʹ���ж�����
	NVIC_Init(&NVIC_TypeDefStructure);	
}

//�ⲿ�жϷ������
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)		//�ж���Ӧ�ж��ߴ����ж�
	{
		TIM_Cmd(TIM4,DISABLE);					//��ʹ�ܶ�ʱ��4
		real_time = status ;
		dis_temp=(float)real_time*1.7f-2;      	//�������	��ʱ10us��S=Vt/2����2����������λ��mm
	}
	EXTI_ClearITPendingBit(EXTI_Line4);			//�����Ӧ�ж����жϱ�־λ
}

/*******************************************************************
��  �ܣ���������������
��  ��: ��
����ֵ: ��
********************************************************************/
void Ultrasonic_Wave(void)
{
	uint8_t i = 0;
	
	TIM_Cmd(TIM4,ENABLE);
	status  = 0;								// ��ʱ������
	for(i=0;i<4;i++)//4��40KHz
	{
        GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);// �����ź�
		Delay_us(12);  
        GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);// �����ź�        
		Delay_us(13);
	}
}


