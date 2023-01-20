#include "stm32f10x.h"
#include "key.h"

/**************************************************************
��  �ܣ�������ʼ������
��  ��: ��
����ֵ: ��
**************************************************************/
void Key_Hardware_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��GPIOʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		        //KEY1����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);			        //��ʼ��GPIOA
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 		        //KEY2 ��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);			        //��ʼ��GPIOC

} 

/**************************************************************
��  �ܣ�����������
��  ��: mode:0,��֧��������;1,֧��������;
����ֵ: 0��û���κΰ�������
		1��KEY1����
		2��KEY2����
		ע��˺�������Ӧ���ȼ�,KEY1>KEY2>WK_UP!!
**************************************************************/

//uint8_t key_time=0;                 //��������ʱ��
uint8_t Key_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;		//�������ɿ���־
	if(mode == 1)key_up=1;			//֧������,key_up=1��������key_up=0��������
//	if(mode == 2 && key_up == 0)	//֧�ֳ���
//	{
//		Delay_ms(1);
//		key_time++;
//	}
//	else
//	{
//		key_time = 0;
//		key_up=1;
//	}
	if(key_up&&(KEY1()==1||KEY2()==1))
	{

		Delay_ms(10);		//ȥ���� 
		key_up=0;
		if(KEY1()==1)     return 1;
		else if(KEY2()==1)return 2;
	}
   
    else if(KEY1()==0&&KEY2()==0)//û�а������£��رշ�����
    {
        key_up=1;
    }       
 	return 0;//�ް�������
}

