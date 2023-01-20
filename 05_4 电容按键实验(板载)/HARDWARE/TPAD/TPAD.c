#include "TPAD.h" 

/*******************************************************************
功  能：释放电容电量，使电压为零
参  数: 无
返回值: 无
********************************************************************/
void TPAD_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
 	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;//PA1 端口配置
 	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStruct);
 	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//PA1输出低电平让电容放电
	
	Delay_ms(5);//等待放电完成
	
	TIM_ClearFlag(TIM5,TIM_FLAG_CC2);//清除捕获标志位
	TIM_SetCounter(TIM5,0);//重新开始计时
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//将引脚拉低
}


/*******************************************************************
功  能：获取一次上升时间
参  数: 无
返回值: 返回CCR中的值(上升时间)
********************************************************************/
uint16_t TPAD_Get_VAL(void)
{
	TPAD_Reset();//电容放电，使电压为零
	
	//等待捕获完成
	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_CC2)==RESET)
	{
		//计数值太大，直接返回计数器CNT的值
		if(TIM_GetCounter(TIM5)>(MAX_ARR_VAL-500))
			return TIM_GetCounter(TIM5);
	}
	return TIM_GetCapture2(TIM5);//返回CCR中的值
}


/*******************************************************************
功  能：TPAD初始化
参  数: 无
返回值: 返回CCR中的值(上升时间)
********************************************************************/
void TPAD_Init(void)
{
	uint8_t i = 0,j=0;
	uint16_t init_val[10] = {0};//存放10次采样值
	uint16_t temp = 0;//临时变量
	
	for(i=0;i<10;i++)//连续读取10次
	{				 
		init_val[i]=TPAD_Get_VAL();
		Delay_ms(7);	    
	}				    
	for(i=0;i<9;i++)//冒泡排序
	{
		for(j=i+1;j<10;j++)
		{
			if(init_val[i]>init_val[j])//升序排列
			{
				temp=init_val[i];
				init_val[i]=init_val[j];
				init_val[j]=temp;
			}
		}
	}
	temp = 0;
	for(i=2;i<8;i++)//分别去掉两个最高值和最低值
	{
		temp+=init_val[i];
	}
	DefaultVal = temp/6;//求平均值
}


/*******************************************************************
功  能：获取最大采样值
参  数: sample:采样次数
返回值: 从n个采样次数中返回最大值(时间)
********************************************************************/
uint16_t TPAD_Get_MaxVal(uint8_t sample)
{
	uint16_t temp=0;
	uint16_t res=0;
	while(sample--)
	{
		temp=TPAD_Get_VAL();//得到一次值
		if(temp>res)
			res=temp;
	}
	return res;
}

/*******************************************************************
功  能：扫描触摸按键
参  数: mode:1连续按  0单次按键
返回值: res:按键状态 1按下 0未按
说  明: 静态变量key值在输入模式为0(单次按键)才有效
********************************************************************/
uint8_t TPAD_Scan(uint8_t mode)
{
	static uint8_t key = 0;
	uint16_t rval = 0;//接收返回值
	uint8_t res = 0,sample = 3;//默认采样次数是三次
	
	if(mode)
	{
		sample = 6;//连续按时采样6次
		key =0;
	}
	rval = TPAD_Get_MaxVal(sample);
	//触摸按键按下去后
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


