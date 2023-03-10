#include "TIM3.h"
 
/*******************************************************************
功  能：通用定时器3中断初始化  
参  数: arr：自动重装值
		psc：时钟预分频数
		定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
		Ft=定时器工作频率,单位:72Mhz
返回值: 无
********************************************************************/

void TIM3_Hardware_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;            //PWM配置
    GPIO_InitTypeDef  GPIO_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);				//使能TIM3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;               //配置RGB引脚	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //复用输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
    TIM_TimeBaseInitStructure.TIM_Period = arr -1;						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc -1;					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);				//初始化TIM3

	TIM_OCStructInit(&TIM_OCInitStructure);//默认配置TIM_OCInitStructure结构体
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//输出比较模式2
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//极性选择
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                // CCR 参考上面介绍公式
	
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);  //初始化TIM3_OC3通道
											/*	TIM3通道3默认复用PB0 
													通道4默认复用PB1  
													通道2重定义PB5 */
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE);	//使能定时器3

}

#if 0
/*******************************************************************
功  能：定时器中断函数 
参  数: 无
返回值: 无
********************************************************************/
void TIM3_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 	//溢出中断
	{

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  	//清除中断标志位
}
#endif


/*******************************************************************
功  能：PWM呼吸灯函数 
参  数: 无
返回值: 无
********************************************************************/
void PWM_LED_Breath_Test(void)
{
	uint8_t i;
	for(i=0;i<=100;i++)
	{
		TIM_SetCompare4(TIM3,i);//更改TIM3通道2的值（设置CCR寄存器的值）
		Delay_ms(15);
	}
	
	for(i=0;i<=100;i++)
	{
		TIM_SetCompare4(TIM3,100-i);
		Delay_ms(15);
	}

}

