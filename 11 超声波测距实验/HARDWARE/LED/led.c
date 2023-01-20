#include "stm32f10x.h"
#include "led.h" 
/*******************************************************************
��  �ܣ�LED ��Beep IO��ʼ��  
��  ��: ��
����ֵ: ��
********************************************************************/
void LED_Hardware_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5;//����RGB����	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��GPIO
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //����Beep����	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��GPIO
	
	LED_R(1);LED_B(1);LED_G(1);               //�ر�����LED
}

/**
  * @brief  Toggles the specified GPIO pins..
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F405xx/407xx and STM32F415xx/417xx devices
  *                      x can be (A..I) to select the GPIO peripheral for STM32F42xxx/43xxx devices.
  *                      x can be (A, B, C, D and H) to select the GPIO peripheral for STM32F401xx devices. 
  * @param  GPIO_Pin: Specifies the pins to be toggled.
  * @retval None
  */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->ODR ^= GPIO_Pin;
}

