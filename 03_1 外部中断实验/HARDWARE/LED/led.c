#include "stm32f10x.h"
#include "led.h" 
/*******************************************************************
功  能：LED 和Beep IO初始化  
参  数: 无
返回值: 无
********************************************************************/
void LED_Hardware_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5;//配置RGB引脚	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化GPIO
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //配置Beep引脚	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIO
	
	LED_R(1);LED_B(1);LED_G(1);               //关闭所有LED
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

