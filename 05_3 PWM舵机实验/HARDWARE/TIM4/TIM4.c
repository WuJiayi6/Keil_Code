#include "TIM4.h"
/*******************************************************************
功  能：通用定时器4中断初始化  

参  数: arr：自动重装值
		psc：时钟预分频数
		定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft.
		Ft=定时器工作频率,单位:Mhz (默认72MHz)
		
计算公式：  PWM频率：Freq = CK_PSC / ((PSC+1)*(ARR+1))
			PWM占空比：Duty = CCR / (ARR+1)
			PWM分辨率： Reso = 1 / (ARR+1)
			
返回值: 无
********************************************************************/
void TIM4_PWM_Hardware_Init(uint16_t arr ,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//定时器配置
	TIM_OCInitTypeDef TIM_OCInitStructure;            //PWM配置
	GPIO_InitTypeDef  GPIO_InitStructure;             //引脚配置
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);		//使能TIM3时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;               //配置PB8	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //复用输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化GPIO

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = arr - 1;	//自动重装载值   ARR 参考上面介绍公式
	TIM_TimeBaseInitStructure.TIM_Prescaler= psc - 1;//定时器分频     PSC 参考上面介绍公式
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);				//初始化TIM4

	TIM_OCStructInit(&TIM_OCInitStructure);//默认配置TIM_OCInitStructure结构体
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//输出比较模式2 (可以理解为调正反转)
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//极性选择
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                // CCR 参考上面介绍公式
	
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);  //初始化TIM4_OC3通道
	
    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_Cmd(TIM4,ENABLE);	//使能定时器3
}

#if 0
/*******************************************************************
功  能：定时器4中断函数 
参  数: 无
返回值: 无
********************************************************************/
void TIM4_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) 	//溢出中断
	{


	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  	//清除中断标志位
}
#endif
/*******************************************************************
功  能：	更改TIM4通道3的CCR
函  数：    
参  数：	无
返回值：	无
********************************************************************/
void TIM4_PWM_SetCompare3(float Compare)
{
	TIM_SetCompare3(TIM4,Compare);
}


/*******************************************************************
功  能：PWM舵机函数 
参  数: 无
返回值: 无
********************************************************************/
float Angle = 0;
void PWM_Servo_Test(float Angle)
{
    Angle = ((Angle /180)*20 + 5);
    TIM4_PWM_SetCompare3(Angle);

}


