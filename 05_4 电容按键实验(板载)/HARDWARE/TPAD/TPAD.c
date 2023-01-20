#include "TPAD.h" 

/*******************************************************************
��  �ܣ��ͷŵ��ݵ�����ʹ��ѹΪ��
��  ��: ��
����ֵ: ��
********************************************************************/
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
 	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;//PA1 �˿�����
 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//�������
 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStruct);
 	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//PA1����͵�ƽ�õ��ݷŵ�
	
	Delay_ms(5);//�ȴ��ŵ����
	
	TIM_ClearFlag(TIM5,TIM_FLAG_CC2);//��������־λ
	TIM_SetCounter(TIM5,0);//���¿�ʼ��ʱ
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//����������
}


/*******************************************************************
��  �ܣ���ȡһ������ʱ��
��  ��: ��
����ֵ: ����CCR�е�ֵ(����ʱ��)
********************************************************************/
uint16_t TPAD_Get_VAL(void)
{
	TPAD_Reset();//���ݷŵ磬ʹ��ѹΪ��
	
	//�ȴ��������
	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_CC2)==RESET)
	{
		//����ֵ̫��ֱ�ӷ��ؼ�����CNT��ֵ
		if(TIM_GetCounter(TIM5)>(MAX_ARR_VAL-500))
			return TIM_GetCounter(TIM5);
	}
	return TIM_GetCapture2(TIM5);//����CCR�е�ֵ
}


/*******************************************************************
��  �ܣ�TPAD��ʼ��
��  ��: ��
����ֵ: ����CCR�е�ֵ(����ʱ��)
********************************************************************/
void TPAD_Init(void)
{
	uint8_t i = 0,j=0;
	uint16_t init_val[10] = {0};//���10�β���ֵ
	uint16_t temp = 0;//��ʱ����
	
	for(i=0;i<10;i++)//������ȡ10��
	{				 
		init_val[i]=TPAD_Get_VAL();
		Delay_ms(7);	    
	}				    
	for(i=0;i<9;i++)//ð������
	{
		for(j=i+1;j<10;j++)
		{
			if(init_val[i]>init_val[j])//��������
			{
				temp=init_val[i];
				init_val[i]=init_val[j];
				init_val[j]=temp;
			}
		}
	}
	temp = 0;
	for(i=2;i<8;i++)//�ֱ�ȥ���������ֵ�����ֵ
	{
		temp+=init_val[i];
	}
	DefaultVal = temp/6;//��ƽ��ֵ
}


/*******************************************************************
��  �ܣ���ȡ������ֵ
��  ��: sample:��������
����ֵ: ��n�����������з������ֵ(ʱ��)
********************************************************************/
uint16_t TPAD_Get_MaxVal(uint8_t sample)
{
	uint16_t temp=0;
	uint16_t res=0;
	while(sample--)
	{
		temp=TPAD_Get_VAL();//�õ�һ��ֵ
		if(temp>res)
			res=temp;
	}
	return res;
}

/*******************************************************************
��  �ܣ�ɨ�败������
��  ��: mode:1������  0���ΰ���
����ֵ: res:����״̬ 1���� 0δ��
˵  ��: ��̬����keyֵ������ģʽΪ0(���ΰ���)����Ч
********************************************************************/
uint8_t TPAD_Scan(uint8_t mode)
{
	static uint8_t key = 0;
	uint16_t rval = 0;//���շ���ֵ
	uint8_t res = 0,sample = 3;//Ĭ�ϲ�������������
	
	if(mode)
	{
		sample = 6;//������ʱ����6��
		key =0;
	}
	rval = TPAD_Get_MaxVal(sample);
	//������������ȥ��
	if(rval >DefaultVal+GATE_VAL){
		if(key==0){
			res=1;
		}
		key=2;
	}
	
	if(key!=0){
		key--;
	}
	return res;
}


