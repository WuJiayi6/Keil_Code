#include "DS18B20.h"


/*******************************************************************
��  �ܣ�DS18B20 ��ʼ������
��������DS18B20_Init
��  ��: ��
����ֵ: ��
********************************************************************/
uint8_t DS18B20_Init(void)
{
	DS18B20_Hardware_Init();
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	DS18B20_Reset();
	
	return DS18B20_Presence();
}


/*******************************************************************
��  �ܣ�����DS18B20�õ���I/O��  PE6
��������DS18B20_Hardware_Init
��  ��: ��
����ֵ: ��
********************************************************************/
void DS18B20_Hardware_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 														   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*��������ģʽΪͨ���������*/   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;/*������������Ϊ50MHz */ 
	GPIO_Init (GPIOE,&GPIO_InitStructure );
}


/*******************************************************************
��  �ܣ�ʹDS18B20-DATA���ű�Ϊ����ģʽ
��������DS18B20_IN_IPU
��  ��: ��
����ֵ: ��
********************************************************************/
void DS18B20_IN_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;/*��������ģʽΪ��������ģʽ*/	
	GPIO_Init(GPIOE,&GPIO_InitStructure );/*���ÿ⺯������ʼ��*/
}


/*******************************************************************
��  �ܣ�ʹDS18B20-DATA���ű�Ϊ���ģʽ
��������DS18B20_OUT_PP
��  ��: ��
����ֵ: ��
********************************************************************/
void DS18B20_OUT_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;														   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;/*��������ģʽΪͨ���������*/   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;/*������������Ϊ50MHz */
	GPIO_Init(GPIOE,&GPIO_InitStructure );/*���ÿ⺯������ʼ��*/
}


/*******************************************************************
��  �ܣ��������ӻ����͸�λ����
��������DS18B20_Reset
��  ��: ��
����ֵ: ��
********************************************************************/
void DS18B20_Reset(void)
{
	DS18B20_OUT_PP();/* ��������Ϊ������� */
	GPIO_ResetBits(GPIOE,GPIO_Pin_6 );/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	Delay_us(500);
	GPIO_SetBits(GPIOE,GPIO_Pin_6 );/* �����ڲ�����λ�źź��轫�������� */
	Delay_us(30);/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
}


/*******************************************************************
��  �ܣ����ӻ����������صĴ�������
��������DS18B20_Presence
��  ��: ��
����ֵ: 0   �ɹ�
		1   ʧ��
˵  ��:
	��GPIO��������Ϊ��������ģʽ��(GPIO����������)����ȡ �õ�GPIO��������λ1
	��GPIO��������Ϊ��������ģʽ��(GPIO����������)����ȡ �õ�GPIO��������λ0
********************************************************************/
uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	DS18B20_IN_IPU();/* ��������Ϊ�������� */

/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź�
 * ����15~60us���������һ���������� */
	while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) && pulse_time<100 )
	{
		pulse_time++;
		Delay_us(1);
	}	
	
	if( pulse_time >=100 )/* ����100us�󣬴������嶼��û�е���*/
		return 1;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
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
��  �ܣ���DS18B20��ȡһ��bit
��������DS18B20_Read_Bit
��  ��: ��
����ֵ: Data   0 �� 1
********************************************************************/
uint8_t DS18B20_Read_Bit(void)
{
	uint8_t Data;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_OUT_PP();
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	Delay_us(10);
	
	DS18B20_IN_IPU();/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	Delay_us(2);
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == SET ){Data = 1;}
	else {Data = 0;}
	
	Delay_us(45);/* �����ʱ������ο�ʱ��ͼ */
	
	return Data;
}


/*******************************************************************
��  �ܣ���DS18B20��һ���ֽڣ���λ����
��������DS18B20_Read_Byte
��  ��: ��
����ֵ: Data
********************************************************************/
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j,Data = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();	//�ڶ�һ��λ��ѭ��8��	
		Data = (Data) | (j<<i);   //��λ��ǰ
	}
	return Data;
}


/*******************************************************************
��  �ܣ�дһ���ֽڵ�DS18B20����λ����
��������DS18B20_Write_Byte
��  ��: Data
����ֵ: ��
********************************************************************/
void DS18B20_Write_Byte(uint8_t Data)
{
	uint8_t i, testb;
	DS18B20_OUT_PP();
	
	for(i=0;i<8;i++)
	{
		testb = Data&0x01;
		Data = Data>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)//д1
		{			
			GPIO_ResetBits(GPIOE,GPIO_Pin_6);//����
			Delay_us(10);/* 1us < �����ʱ < 15us */
			GPIO_SetBits(GPIOE,GPIO_Pin_6);//����
			Delay_us(58);
		}		
		else //д0
		{			
			GPIO_ResetBits(GPIOE,GPIO_Pin_6);//����
			Delay_us(70);/* 60us < Tx 0 < 120us */
			GPIO_SetBits(GPIOE,GPIO_Pin_6);//����			
			Delay_us(5);/* 1us < Trec(�ָ�ʱ��) < �����*/
		}
	}
}


/*******************************************************************
��  �ܣ�����ƥ�� DS18B20 ROM
��������DS18B20_Skip_ROM
��  ��: ��
����ֵ: ��
********************************************************************/
void DS18B20_Skip_ROM(void)
{
	DS18B20_Reset();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* ���� ROM */
}


/*******************************************************************
��  �ܣ�ִ��ƥ�� DS18B20 ROM
��������DS18B20_Skip_ROM
��  ��: ��
����ֵ: ��
********************************************************************/
void DS18B20_Match_ROM(void)
{
	DS18B20_Reset();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0X55);		/* ƥ�� ROM */
}


/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */

/*******************************************************************
��  �ܣ�������ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ
��������DS18B20_GetTemp_SkipROM
��  ��: ��
����ֵ: Tem �¶�ֵ
********************************************************************/
float DS18B20_GetTemp_SkipROM(void)
{
	uint8_t TMSB, TLSB;
	short s_tem;
	float Tem;

	DS18B20_Skip_ROM();
	DS18B20_Write_Byte(0X44);/* ��ʼת�� */				

	DS18B20_Skip_ROM();
	DS18B20_Write_Byte(0XBE);/* ���¶�ֵ */				
	
	TLSB = DS18B20_Read_Byte();//Byte 0		 
	TMSB = DS18B20_Read_Byte();//Byte 1
	
	s_tem = (TMSB<<8)|TLSB;
	
	/* ���¶� */
	if( s_tem < 0 ){Tem = (~s_tem+1) * 0.0625;}		
	else {Tem = s_tem * 0.0625;}

	return Tem; 	
}


/*******************************************************************
��  �ܣ���ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ 
��������DS18B20_ReadId
��  ��: DS18B20_ID�����ڴ�� DS18B20 ���кŵ�������׵�ַ
����ֵ: ��
********************************************************************/
void DS18B20_ReadId(uint8_t * DS18B20_ID)
{
	uint8_t uc;
	DS18B20_Write_Byte(0x33);       //��ȡ���к�
	for (uc = 0;uc<8; uc ++ ){DS18B20_ID[uc] = DS18B20_Read_Byte();}
}


/*******************************************************************
��  �ܣ���ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ
��������DS18B20_GetTemp_MatchROM
��  ��: DS18B20_ID: ��� DS18B20 ���кŵ�������׵�ַ
����ֵ: Tem �¶�ֵ
********************************************************************/
float DS18B20_GetTemp_MatchROM ( uint8_t * DS18B20_ID)
{
	uint8_t TMSB,TLSB, i;
	short s_tem;
	float Tem;
	
	DS18B20_Match_ROM();            //ƥ��ROM
	
	for(i=0;i<8;i++){DS18B20_Write_Byte(DS18B20_ID[i]);}
		
	DS18B20_Write_Byte(0X44);		/* ��ʼת�� */
	DS18B20_Match_ROM();            //ƥ��ROM
	
	for(i=0;i<8;i++){DS18B20_Write_Byte ( DS18B20_ID[i]);}
	
	DS18B20_Write_Byte(0XBE);	   /* ���¶�ֵ */
	
	TLSB = DS18B20_Read_Byte();		 
	TMSB = DS18B20_Read_Byte(); 
	
	s_tem = (TMSB<<8)|TLSB;
	/* ���¶� */
	if(s_tem<0){Tem = (~s_tem+1) * 0.0625;}		
	else {Tem =s_tem * 0.0625;}
		
	return Tem; 	
}

