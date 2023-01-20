#include "DS18B20.h"


/*******************************************************************
功  能：DS18B20 初始化函数
函数名：DS18B20_Init
参  数: 无
返回值: 无
********************************************************************/
uint8_t DS18B20_Init(void)
{
	DS18B20_Hardware_Init();
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	DS18B20_Reset();
	
	return DS18B20_Presence();
}


/*******************************************************************
功  能：配置DS18B20用到的I/O口  PE6
函数名：DS18B20_Hardware_Init
参  数: 无
返回值: 无
********************************************************************/
void DS18B20_Hardware_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;/*定义一个GPIO_InitTypeDef类型的结构体*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 														   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*设置引脚模式为通用推挽输出*/   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;/*设置引脚速率为50MHz */ 
	GPIO_Init (GPIOE,&GPIO_InitStructure );
}


/*******************************************************************
功  能：使DS18B20-DATA引脚变为输入模式
函数名：DS18B20_IN_IPU
参  数: 无
返回值: 无
********************************************************************/
void DS18B20_IN_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;/*设置引脚模式为浮空输入模式*/	
	GPIO_Init(GPIOE,&GPIO_InitStructure );/*调用库函数，初始化*/
}


/*******************************************************************
功  能：使DS18B20-DATA引脚变为输出模式
函数名：DS18B20_OUT_PP
参  数: 无
返回值: 无
********************************************************************/
void DS18B20_OUT_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;														   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*设置引脚模式为通用推挽输出*/   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;/*设置引脚速率为50MHz */
	GPIO_Init(GPIOE,&GPIO_InitStructure );/*调用库函数，初始化*/
}


/*******************************************************************
功  能：主机给从机发送复位脉冲
函数名：DS18B20_Reset
参  数: 无
返回值: 无
********************************************************************/
void DS18B20_Reset(void)
{
	DS18B20_OUT_PP();/* 主机设置为推挽输出 */
	GPIO_ResetBits(GPIOE,GPIO_Pin_6 );/* 主机至少产生480us的低电平复位信号 */
	Delay_us(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_6 );/* 主机在产生复位信号后，需将总线拉高 */
	Delay_us(30);/*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
}


/*******************************************************************
功  能：检测从机给主机返回的存在脉冲
函数名：DS18B20_Presence
参  数: 无
返回值: 0   成功
		1   失败
说  明:
	若GPIO引脚配置为上拉输入模式，(GPIO引脚无输入)，读取 得的GPIO引脚数据位1
	若GPIO引脚配置为下拉输入模式，(GPIO引脚无输入)，读取 得的GPIO引脚数据位0
********************************************************************/
uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	DS18B20_IN_IPU();/* 主机设置为上拉输入 */

/* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，
 * 会在15~60us后给主机发一个存在脉冲 */
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) && pulse_time<100 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	
	if( pulse_time >=100 )/* 经过100us后，存在脉冲都还没有到来*/
		return 1;
	else
		pulse_time = 0;
	
	/* 存在脉冲到来，且存在的时间不能超过240us */
	while( !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) && pulse_time<240 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}


/*******************************************************************
功  能：从DS18B20读取一个bit
函数名：DS18B20_Read_Bit
参  数: 无
返回值: Data   0 或 1
********************************************************************/
uint8_t DS18B20_Read_Bit(void)
{
	uint8_t Data;
	
	/* 读0和读1的时间至少要大于60us */	
	DS18B20_OUT_PP();
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	Delay_us(10);
	
	DS18B20_IN_IPU();/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	Delay_us(2);
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == SET ){Data = 1;}
	else {Data = 0;}
	
	Delay_us(45);/* 这个延时参数请参考时序图 */
	
	return Data;
}


/*******************************************************************
功  能：从DS18B20读一个字节，低位先行
函数名：DS18B20_Read_Byte
参  数: 无
返回值: Data
********************************************************************/
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j,Data = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();	//在读一个位中循环8次	
		Data = (Data) | (j<<i);   //低位在前
	}
	return Data;
}


/*******************************************************************
功  能：写一个字节到DS18B20，低位先行
函数名：DS18B20_Write_Byte
参  数: Data
返回值: 无
********************************************************************/
void DS18B20_Write_Byte(uint8_t Data)
{
	uint8_t i, testb;
	DS18B20_OUT_PP();
	
	for(i=0;i<8;i++)
	{
		testb = Data&0x01;
		Data = Data>>1;		
		/* 写0和写1的时间至少要大于60us */
		if (testb)//写1
		{			
			GPIO_ResetBits(GPIOE,GPIO_Pin_6);//拉低
			Delay_us(10);/* 1us < 这个延时 < 15us */
			GPIO_SetBits(GPIOE,GPIO_Pin_6);//拉高
			Delay_us(58);
		}		
		else //写0
		{			
			GPIO_ResetBits(GPIOE,GPIO_Pin_6);//拉低
			Delay_us(70);/* 60us < Tx 0 < 120us */
			GPIO_SetBits(GPIOE,GPIO_Pin_6);//拉高			
			Delay_us(5);/* 1us < Trec(恢复时间) < 无穷大*/
		}
	}
}


/*******************************************************************
功  能：跳过匹配 DS18B20 ROM
函数名：DS18B20_Skip_ROM
参  数: 无
返回值: 无
********************************************************************/
void DS18B20_Skip_ROM(void)
{
	DS18B20_Reset();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* 跳过 ROM */
}


/*******************************************************************
功  能：执行匹配 DS18B20 ROM
函数名：DS18B20_Skip_ROM
参  数: 无
返回值: 无
********************************************************************/
void DS18B20_Match_ROM(void)
{
	DS18B20_Reset();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0X55);		/* 匹配 ROM */
}


/*
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */

/*******************************************************************
功  能：在跳过匹配 ROM 情况下获取 DS18B20 温度值
函数名：DS18B20_GetTemp_SkipROM
参  数: 无
返回值: Tem 温度值
********************************************************************/
float DS18B20_GetTemp_SkipROM(void)
{
	uint8_t TMSB, TLSB;
	short s_tem;
	float Tem;

	DS18B20_Skip_ROM();
	DS18B20_Write_Byte(0X44);/* 开始转换 */				

	DS18B20_Skip_ROM();
	DS18B20_Write_Byte(0XBE);/* 读温度值 */				
	
	TLSB = DS18B20_Read_Byte();//Byte 0		 
	TMSB = DS18B20_Read_Byte();//Byte 1
	
	s_tem = (TMSB<<8)|TLSB;
	
	/* 负温度 */
	if( s_tem < 0 ){Tem = (~s_tem+1) * 0.0625;}		
	else {Tem = s_tem * 0.0625;}

	return Tem; 	
}


/*******************************************************************
功  能：在匹配 ROM 情况下获取 DS18B20 温度值 
函数名：DS18B20_ReadId
参  数: DS18B20_ID：用于存放 DS18B20 序列号的数组的首地址
返回值: 无
********************************************************************/
void DS18B20_ReadId(uint8_t * DS18B20_ID)
{
	uint8_t uc;
	DS18B20_Write_Byte(0x33);       //读取序列号
	for (uc = 0;uc<8; uc ++ ){DS18B20_ID[uc] = DS18B20_Read_Byte();}
}


/*******************************************************************
功  能：在匹配 ROM 情况下获取 DS18B20 温度值
函数名：DS18B20_GetTemp_MatchROM
参  数: DS18B20_ID: 存放 DS18B20 序列号的数组的首地址
返回值: Tem 温度值
********************************************************************/
float DS18B20_GetTemp_MatchROM ( uint8_t * DS18B20_ID)
{
	uint8_t TMSB,TLSB, i;
	short s_tem;
	float Tem;
	
	DS18B20_Match_ROM();            //匹配ROM
	
	for(i=0;i<8;i++){DS18B20_Write_Byte(DS18B20_ID[i]);}
		
	DS18B20_Write_Byte(0X44);		/* 开始转换 */
	DS18B20_Match_ROM();            //匹配ROM
	
	for(i=0;i<8;i++){DS18B20_Write_Byte ( DS18B20_ID[i]);}
	
	DS18B20_Write_Byte(0XBE);	   /* 读温度值 */
	
	TLSB = DS18B20_Read_Byte();		 
	TMSB = DS18B20_Read_Byte(); 
	
	s_tem = (TMSB<<8)|TLSB;
	/* 负温度 */
	if(s_tem<0){Tem = (~s_tem+1) * 0.0625;}		
	else {Tem =s_tem * 0.0625;}
		
	return Tem; 	
}

