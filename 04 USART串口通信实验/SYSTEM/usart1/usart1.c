#include "usart1.h"

uint8_t usart1_buffer[USART1_RX_SIZE] = { 0 };//储存串口一接收数据的数组
uint16_t usart1_length = 0;//储存串口一接收数据数组的长度

/****************************************************************
功  能：串口1的初始化函数
参  数：baudrate		串口1的波特率
返回值：无
*****************************************************************/
void USART1_Hardware_Init(uint32_t baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 打开串口GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);// 打开串口外设的时钟
    
//	//引脚复用映射PA9和PA10到串口上
//    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	
    //GPIOA9 - Tx 初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//选择端口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输出速度
    GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化配置
    
    //GPIOA10 - Rx 初始化	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//选择端口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//复用浮空输入模式
    GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化配置
    
    //串口1参数设置
    USART_InitStructure.USART_BaudRate = baudrate;//设置波特率
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//设置串口模式
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//设置无硬件流控制
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit数据位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无校验位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//1停止位
    USART_Init(USART1,&USART_InitStructure);//初始化配置串口1
    
    //串口1中断响应优先级设置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//选择中断组
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//选择中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//设置先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//设置从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//设置中断通道开启
    NVIC_Init(&NVIC_InitStructure);//初始化配置中断优先级
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启串口1接受中断使能
    USART_Cmd(USART1,ENABLE);//开启串口1
}

/****************************************************************
功  能：串口1的接收中断服务函数
参  数：无
返回值：无
*****************************************************************/
void USART1_IRQHandler(void)
{
	//判断串口接收中断标志位
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
    {
		//读取串口数据
		usart1_buffer[usart1_length] = USART_ReceiveData(USART1);
		//指针自增
		usart1_length++;
		//如果储存超出长度
		if(usart1_length == USART1_RX_SIZE)
		{
			//重新从数组0开始储存字节
			usart1_length = 0 ;
		}
    }
	//清除串口中断接收标志位
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

/****************************************************************
功  能：串口1发送单字节数据
参  数：src	发送的单字节数据
返回值：无
*****************************************************************/
void USART1_Send_Byte(uint8_t src)
{
	//发送数据
	USART_SendData(USART1,src);
	//等待串口发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

/****************************************************************
功  能：串口1发送一定长度的字节数据
参  数：src	发送的字符串数据,length	发送的数据长度
返回值：无
*****************************************************************/
void USART1_Send_Length_String(uint8_t *src,uint16_t length)
{
	uint16_t Tx_cut = 0;
	uint16_t Tx_length = 0;
	Tx_length = length;
	//循环发送指定长度的数据
	while(Tx_cut != Tx_length)
	{
		//发送一个字节信息
		USART1_Send_Byte(*(src+Tx_cut));
		//指针自增
		Tx_cut++;
	}
}

/****************************************************************
功  能：串口1发送字符串数据
参  数：src	发送的字符串数据,以'\0'即0x00标识结束发送数据
返回值：无
*****************************************************************/
void USART1_Send_String(char *src)
{
    uint16_t Tx_cut = 0;
    //检测到下一个发送字节为0x00即' '时停止发送
    while(*(src+Tx_cut) != '\0')
    {
		//发送一个字节信息
        USART1_Send_Byte(*(src+Tx_cut));
		//指针自增
        Tx_cut++;
    }
	
}

/****************************************************************
功  能：串口1清除储存数组的数据
参  数：无
返回值：无
*****************************************************************/
void USART1_Clear_Buffer(void)
{
	while(usart1_length)      //清空接收到的数据
	{
		//指针自减
		usart1_length--;
		//数据清零
		usart1_buffer[usart1_length] = 0;
	}
}

//封装指令函数
void USART_Send_Command(void)
{
	static uint8_t ret_left=0;//流水灯标志位
	static uint8_t ret_spark=0;//闪烁标志位
	static uint8_t Selet_LED=0;
	if(usart1_length != 0 )//收到串口指令
	{
		USART1_Send_String("\nYou said :");	//打印开头字符串
		USART1_Send_Length_String(usart1_buffer,usart1_length);
		if(strcmp(usart1_buffer,"left") == 0)//判断指令是否为 left
		{
			ret_left=1;
			ret_spark=0;
			Selet_LED=0;
		}
		else if(strcmp(usart1_buffer,"spark") == 0)//判断指令是否为 spark
		{
			ret_left=0;
			ret_spark=1;
			Selet_LED=0;
			LED_R(1);LED_G(1);LED_B(1);//熄灯
		}
	}USART1_Clear_Buffer();//发送接收到的数据并清空
	if(ret_left)
	{
		LED_R(1);LED_G(1);LED_B(1);
		switch(Selet_LED++)
		{
			case 1:LED_R(0);break;
			case 2:LED_G(0);break;
			case 3:LED_B(0);Selet_LED = 0;break;
			default:break;
		}
	}
	else if(ret_spark)
	{
		LEDR_TOGGLE();
	}
	Delay_ms(500);
}

