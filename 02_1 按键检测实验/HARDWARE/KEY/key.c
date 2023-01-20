#include "stm32f10x.h"
#include "key.h"

/**************************************************************
功  能：按键初始化函数
参  数: 无
返回值: 无
**************************************************************/
void Key_Hardware_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIO时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		        //KEY1引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);			        //初始化GPIOA
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 		        //KEY2 对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);			        //初始化GPIOC

} 

/**************************************************************
功  能：按键处理函数
参  数: mode:0,不支持连续按;1,支持连续按;
返回值: 0，没有任何按键按下
		1，KEY1按下
		2，KEY2按下
		注意此函数有响应优先级,KEY1>KEY2>WK_UP!!
**************************************************************/

//uint8_t key_time=0;                 //按键长按时间
uint8_t Key_Scan(uint8_t mode)
{	 
	static uint8_t key_up=1;		//按键按松开标志
	if(mode == 1)key_up=1;			//支持连按,key_up=1按键挂起，key_up=0按键按下
//	if(mode == 2 && key_up == 0)	//支持长按
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

		Delay_ms(10);		//去抖动 
		key_up=0;
		if(KEY1()==1)     return 1;
		else if(KEY2()==1)return 2;
	}
   
    else if(KEY1()==0&&KEY2()==0)//没有按键按下，关闭蜂鸣器
    {
        key_up=1;
    }       
 	return 0;//无按键按下
}

