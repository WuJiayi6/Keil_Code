#include "usart1.h"

uint8_t usart1_buffer[USART1_RX_SIZE] = { 0 };//���洮��һ�������ݵ�����
uint16_t usart1_length = 0;//���洮��һ������������ĳ���

/****************************************************************
��  �ܣ�����1�ĳ�ʼ������
��  ����baudrate		����1�Ĳ�����
����ֵ����
*****************************************************************/
void USART1_Hardware_Init(uint32_t baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // �򿪴���GPIO��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);// �򿪴��������ʱ��
    
//	//���Ÿ���ӳ��PA9��PA10��������
//    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
//	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	
    //GPIOA9 - Tx ��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//ѡ��˿�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//�����������ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�
    GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ������
    
    //GPIOA10 - Rx ��ʼ��	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//ѡ��˿�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//���ø�������ģʽ
    GPIO_Init(GPIOA,&GPIO_InitStructure);//��ʼ������
    
    //����1��������
    USART_InitStructure.USART_BaudRate = baudrate;//���ò�����
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���ô���ģʽ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������Ӳ��������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8bit����λ
    USART_InitStructure.USART_Parity = USART_Parity_No;//��У��λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//1ֹͣλ
    USART_Init(USART1,&USART_InitStructure);//��ʼ�����ô���1
    
    //����1�ж���Ӧ���ȼ�����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//ѡ���ж���
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//ѡ���ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//���ô����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�����ж�ͨ������
    NVIC_Init(&NVIC_InitStructure);//��ʼ�������ж����ȼ�
    
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//��������1�����ж�ʹ��
    USART_Cmd(USART1,ENABLE);//��������1
}

/****************************************************************
��  �ܣ�����1�Ľ����жϷ�����
��  ������
����ֵ����
*****************************************************************/
void USART1_IRQHandler(void)
{
	//�жϴ��ڽ����жϱ�־λ
    if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
    {
		//��ȡ��������
		usart1_buffer[usart1_length] = USART_ReceiveData(USART1);
		//ָ������
		usart1_length++;
		//������泬������
		if(usart1_length == USART1_RX_SIZE)
		{
			//���´�����0��ʼ�����ֽ�
			usart1_length = 0 ;
		}
    }
	//��������жϽ��ձ�־λ
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

/****************************************************************
��  �ܣ�����1���͵��ֽ�����
��  ����src	���͵ĵ��ֽ�����
����ֵ����
*****************************************************************/
void USART1_Send_Byte(uint8_t src)
{
	//��������
	USART_SendData(USART1,src);
	//�ȴ����ڷ������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

/****************************************************************
��  �ܣ�����1����һ�����ȵ��ֽ�����
��  ����src	���͵��ַ�������,length	���͵����ݳ���
����ֵ����
*****************************************************************/
void USART1_Send_Length_String(uint8_t *src,uint16_t length)
{
	uint16_t Tx_cut = 0;
	uint16_t Tx_length = 0;
	Tx_length = length;
	//ѭ������ָ�����ȵ�����
	while(Tx_cut != Tx_length)
	{
		//����һ���ֽ���Ϣ
		USART1_Send_Byte(*(src+Tx_cut));
		//ָ������
		Tx_cut++;
	}
}

/****************************************************************
��  �ܣ�����1�����ַ�������
��  ����src	���͵��ַ�������,��'\0'��0x00��ʶ������������
����ֵ����
*****************************************************************/
void USART1_Send_String(char *src)
{
    uint16_t Tx_cut = 0;
    //��⵽��һ�������ֽ�Ϊ0x00��' 'ʱֹͣ����
    while(*(src+Tx_cut) != '\0')
    {
		//����һ���ֽ���Ϣ
        USART1_Send_Byte(*(src+Tx_cut));
		//ָ������
        Tx_cut++;
    }
	
}

/****************************************************************
��  �ܣ�����1����������������
��  ������
����ֵ����
*****************************************************************/
void USART1_Clear_Buffer(void)
{
	while(usart1_length)      //��ս��յ�������
	{
		//ָ���Լ�
		usart1_length--;
		//��������
		usart1_buffer[usart1_length] = 0;
	}
}

//��װָ���
void USART_Send_Command(void)
{
	static uint8_t ret_left=0;//��ˮ�Ʊ�־λ
	static uint8_t ret_spark=0;//��˸��־λ
	static uint8_t Selet_LED=0;
	if(usart1_length != 0 )//�յ�����ָ��
	{
		USART1_Send_String("\nYou said :");	//��ӡ��ͷ�ַ���
		USART1_Send_Length_String(usart1_buffer,usart1_length);
		if(strcmp(usart1_buffer,"left") == 0)//�ж�ָ���Ƿ�Ϊ left
		{
			ret_left=1;
			ret_spark=0;
			Selet_LED=0;
		}
		else if(strcmp(usart1_buffer,"spark") == 0)//�ж�ָ���Ƿ�Ϊ spark
		{
			ret_left=0;
			ret_spark=1;
			Selet_LED=0;
			LED_R(1);LED_G(1);LED_B(1);//Ϩ��
		}
	}USART1_Clear_Buffer();//���ͽ��յ������ݲ����
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

