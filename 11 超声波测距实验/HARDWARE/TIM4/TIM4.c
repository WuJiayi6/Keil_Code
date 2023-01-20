#include "TIM4.h"

uint32_t status = 0;						// 计数值
uint32_t real_time = 0;					// 读回值
float dis_temp = 0;						// 距离计算值
/*******************************************************************
功  能：通用定时器4中断初始化，给模块TRIG端口发送大于10us的高电平信号 
参  数: arr：自动重装值
		psc：时钟预分频数
		定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
		Ft=定时器工作频率,单位:72Mhz
返回值: 无
********************************************************************/
void TIM4_Hardware_Init(uint16_t arr,uint16_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);				//使能TIM4时钟
    
	TIM_TimeBaseInitStructure.TIM_Period = arr;						//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;					//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Down;	//向下计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //时钟分频因子
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数设置
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);				//初始化TIM4
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断组
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//选择中断线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;// 中断线使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应优先级  
    NVIC_Init(&NVIC_InitStructure);//中断线优先级初始化
    
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE);	//使能定时器4
}

/*******************************************************************
功  能：定时器4中断服务程序 
参  数: 无
返回值: 无
********************************************************************/	 
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)      //判断定时器4更新中断触发
	{
		status++;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	//清除定时器4更新中断触发
}


/*******************************************************************
功  能：外部中断初始化程序
参  数: 无
返回值: 无
********************************************************************/
void Tran_EXTI_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_TypeStructure;
	EXTI_InitTypeDef EXTI_TypeDefStructure;
	NVIC_InitTypeDef NVIC_TypeDefStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIO_TRAN时钟
	
    //Trig
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_7;//选择端口
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_Out_PP;//通用推挽输出
	GPIO_TypeStructure.GPIO_Speed = GPIO_Speed_50MHz;//选择输出最大速度  50MHz
	GPIO_Init(GPIOA,&GPIO_TypeStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);//默认拉低
    
    //Echo
	GPIO_TypeStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_TypeStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_Init(GPIOA,&GPIO_TypeStructure);//初始化端口
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);//默认拉低
    
	//中断线关联
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);//中断线关联,PA4
	
	//外部中断配置
	EXTI_TypeDefStructure.EXTI_Line = EXTI_Line4;				//中断线
	EXTI_TypeDefStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//请求方式
	EXTI_TypeDefStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
	EXTI_TypeDefStructure.EXTI_LineCmd = ENABLE;				//使能中断线
	EXTI_Init(&EXTI_TypeDefStructure);	
	
	//外部中断向量 EXTI4_IRQn 优先级配置
	NVIC_TypeDefStructure.NVIC_IRQChannel = EXTI4_IRQn;			//中断向量选择
	NVIC_TypeDefStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
	NVIC_TypeDefStructure.NVIC_IRQChannelSubPriority  = 0;		//响应优先级
	NVIC_TypeDefStructure.NVIC_IRQChannelCmd = ENABLE;			//使能中断向量
	NVIC_Init(&NVIC_TypeDefStructure);	
}

//外部中断服务程序
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)		//判断相应中断线触发中断
	{
		TIM_Cmd(TIM4,DISABLE);					//关使能定时器4
		real_time = status ;
		dis_temp=(float)real_time*1.7f-2;      	//计算距离	定时10us，S=Vt/2（减2是误差补尝）单位：mm
	}
	EXTI_ClearITPendingBit(EXTI_Line4);			//清除相应中断线中断标志位
}

/*******************************************************************
功  能：左发生超声波函数
参  数: 无
返回值: 无
********************************************************************/
void Ultrasonic_Wave(void)
{
	uint8_t i = 0;
	
	TIM_Cmd(TIM4,ENABLE);
	status  = 0;								// 定时器清零
	for(i=0;i<4;i++)//4个40KHz
	{
        GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);// 发送信号
		Delay_us(12);  
        GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);// 发送信号        
		Delay_us(13);
	}
}


