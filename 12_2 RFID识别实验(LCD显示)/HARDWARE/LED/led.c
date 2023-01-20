#include "led.h" 
/*******************************************************************
功  能：LED 和Beep IO初始化  
参  数: 无
返回值: 无
********************************************************************/
void LED_Hardware_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	//使能时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5;//配置RGB引脚	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           //配置Beep引脚	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化GPIO
	
	LED_W(1);//关闭所有的灯
}

/**
  * @brief  切换指定的GPIO引脚
  * @param  GPIOx: where x can be (A..G)
  * @param  GPIO_Pin:指定要切换的引脚
  * @retval 无
  */
void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->ODR ^= GPIO_Pin;
}

