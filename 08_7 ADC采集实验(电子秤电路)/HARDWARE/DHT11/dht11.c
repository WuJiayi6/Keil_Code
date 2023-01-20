#include "dht11.h"

//extern const unsigned char Text16[][32];
/**************************************************************
*功  能：复位DHT11
*参  数: 无
*返回值: 无 
**************************************************************/
void DHT11_Rst(void)
{
	DHT11_IO_OUT(); 	//SET OUTPUT
    DHT11_DQ_OUT(0); 	//拉低DQ
    Delay_ms(20);    	//拉低至少18ms
    DHT11_DQ_OUT(1); 	//DQ=1
	Delay_us(40);     	//主机拉高20~40us
}

/**************************************************************
*功  能：等待DHT11的回应
*参  数: 无
*返回值: 返回1:未检测到DHT11的存在； 返回0:存在
**************************************************************/
uint8_t DHT11_Check(void) 	   
{
	uint8_t retry=0;
	DHT11_IO_IN();						//SET INPUT	 
    while (DHT11_DQ_IN()&&retry<100)		//DHT11会拉低40~80us
	{
		retry++;
		Delay_us(1);
	}
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN()&&retry<100)		//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		Delay_us(1);
	}
	if(retry>=100)return 1;	    
	return 0;
}

/**************************************************************
*功  能：从DHT11读取一个位
*参  数: 无
*返回值: 1/0
**************************************************************/
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN()&&retry<100)	//等待变为低电平
	{
		retry++;
		Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN()&&retry<100)	//等待变高电平
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40);					//等待40us
	if(DHT11_DQ_IN())return 1;
	else return 0;
}

/**************************************************************
*功  能：从DHT11读取一个字节
*参  数: 无
*返回值: 读到的数据 
**************************************************************/
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

/**************************************************************
*功  能：从DHT11读取一次数据
*参  数: temp:温度值(范围:0~50°)；humi:湿度值(范围:20%~90%)
*返回值: 0,正常;1,读取失败 
**************************************************************/
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)	//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi = buf[0] * 10 + buf[1];
			*temp = buf[2] * 10 + buf[3];
		}
	}
	else return 1;
	return 0;	    
}
 
/**************************************************************
*功  能：初始化DHT11的IO口 DQ，同时检测DHT11的存在
*参  数: 无
*返回值: 返回1:不存在；返回0:存在
**************************************************************/
uint8_t DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	//使能PE端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//PE6端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE,&GPIO_InitStructure);					//初始化IO口
 	GPIO_SetBits(GPIOE,GPIO_Pin_6);						    //PE6 输出高

	DHT11_Rst();  			//复位DHT11
	return DHT11_Check();	//等待DHT11的回应
} 

/**************************************************************
*功  能：初始化输出端口
*参  数: 无
*返回值: 无 
**************************************************************/
void DQ_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //PE6端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;         //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);				 //初始化IO口
}

/**************************************************************
*功  能：初始化输入端口
*参  数: 无
*返回值: 无 
**************************************************************/
void DQ_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //PE6端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
	
 	GPIO_Init(GPIOE, &GPIO_InitStructure);				 //初始化IO口
	
}

/**************************************************************
*功  能：DHT11传感器检测
*参  数: 无
*返回值: 无 
**************************************************************/
void DHT11_WHILE(void)		
{
	while(DHT11_Init())
	{
		Delay_ms(50);
	}
}
/**************************************************************
*功  能：读取温湿度值
*参  数: 无
*返回值: 无 
**************************************************************/
void DHT11_Test()
{
	static uint16_t temperature;
	static uint16_t humidity;
	char  data[20];
	
    DHT11_Read_Data(&temperature,&humidity);//读取温湿度返回值
    
	sprintf(data,"%.1f  tem",temperature/10.0f);//读温度
    ILI9341_DispStringLine_EN(LINE(1),data);
    
    Delay_ms(2);
    sprintf(data,"%d %% hum",humidity/10);//读湿度
    ILI9341_DispStringLine_EN(LINE(2),data);
	
}


