#include "led.h" 
/*******************************************************************
��  �ܣ�LED ��Beep IO��ʼ��  
��  ��: ��
����ֵ: ��
********************************************************************/
void LED_Hardware_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5;//����RGB����	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //����Beep����	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��GPIO
	
	LED_W(1);//�ر����еĵ�
}

/**
  * @brief  �л�ָ����GPIO����
  * @param  GPIOx: where x can be (A..G)
  * @param  GPIO_Pin:ָ��Ҫ�л�������
  * @retval ��
  */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->ODR ^= GPIO_Pin;
}

