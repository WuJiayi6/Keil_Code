#include "dht11.h"

//extern const unsigned char Text16[][32];
/**************************************************************
*��  �ܣ���λDHT11
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DHT11_Rst(void)
{
	DHT11_IO_OUT(); 	//SET OUTPUT
    DHT11_DQ_OUT(0); 	//����DQ
    Delay_ms(20);    	//��������18ms
    DHT11_DQ_OUT(1); 	//DQ=1
	Delay_us(40);     	//��������20~40us
}

/**************************************************************
*��  �ܣ��ȴ�DHT11�Ļ�Ӧ
*��  ��: ��
*����ֵ: ����1:δ��⵽DHT11�Ĵ��ڣ� ����0:����
**************************************************************/
uint8_t DHT11_Check(void) 	   
{
	uint8_t retry=0;
	DHT11_IO_IN();						//SET INPUT	 
    while (DHT11_DQ_IN()&&retry<100)		//DHT11������40~80us
	{
		retry++;
		Delay_us(1);
	}
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN()&&retry<100)		//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		Delay_us(1);
	}
	if(retry>=100)return 1;	    
	return 0;
}

/**************************************************************
*��  �ܣ���DHT11��ȡһ��λ
*��  ��: ��
*����ֵ: 1/0
**************************************************************/
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN()&&retry<100)	//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN()&&retry<100)	//�ȴ���ߵ�ƽ
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40);					//�ȴ�40us
	if(DHT11_DQ_IN())return 1;
	else return 0;
}

/**************************************************************
*��  �ܣ���DHT11��ȡһ���ֽ�
*��  ��: ��
*����ֵ: ���������� 
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
*��  �ܣ���DHT11��ȡһ������
*��  ��: temp:�¶�ֵ(��Χ:0~50��)��humi:ʪ��ֵ(��Χ:20%~90%)
*����ֵ: 0,����;1,��ȡʧ�� 
**************************************************************/
uint8_t DHT11_Read_Data(uint16_t *temp,uint16_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)	//��ȡ40λ����
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
*��  �ܣ���ʼ��DHT11��IO�� DQ��ͬʱ���DHT11�Ĵ���
*��  ��: ��
*����ֵ: ����1:�����ڣ�����0:����
**************************************************************/
uint8_t DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	//ʹ��PE�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				//PE6�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE,&GPIO_InitStructure);					//��ʼ��IO��
 	GPIO_SetBits(GPIOE,GPIO_Pin_6);						    //PE6 �����

	DHT11_Rst();  			//��λDHT11
	return DHT11_Check();	//�ȴ�DHT11�Ļ�Ӧ
} 

/**************************************************************
*��  �ܣ���ʼ������˿�
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DQ_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //PE6�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;         //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);				 //��ʼ��IO��
}

/**************************************************************
*��  �ܣ���ʼ������˿�
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DQ_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //PE6�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	
 	GPIO_Init(GPIOE, &GPIO_InitStructure);				 //��ʼ��IO��
	
}

/**************************************************************
*��  �ܣ�DHT11���������
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DHT11_WHILE(void)		
{
	while(DHT11_Init())
	{
		Delay_ms(50);
	}
}
/**************************************************************
*��  �ܣ���ȡ��ʪ��ֵ
*��  ��: ��
*����ֵ: �� 
**************************************************************/
void DHT11_Test()
{
	static uint16_t temperature;
	static uint16_t humidity;
	char  data[20];
	
    DHT11_Read_Data(&temperature,&humidity);//��ȡ��ʪ�ȷ���ֵ
    
	sprintf(data,"%.1f  tem",temperature/10.0f);//���¶�
    ILI9341_DispStringLine_EN(LINE(1),data);
    
    Delay_ms(2);
    sprintf(data,"%d %% hum",humidity/10);//��ʪ��
    ILI9341_DispStringLine_EN(LINE(2),data);
	
}


