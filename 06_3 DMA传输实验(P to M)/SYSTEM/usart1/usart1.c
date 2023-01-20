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
	
    //����1�ж���Ӧ���ȼ�����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ���ж���
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//ѡ���ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//���ô����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//�����ж�ͨ������
    NVIC_Init(&NVIC_InitStructure);//��ʼ�������ж����ȼ�
    
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
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//ʹ�ܿ����ж�
    USART_Cmd(USART1,ENABLE);//��������1
}

/****************************************************************
��  �ܣ�����1�����жϷ�����
��  ������
����ֵ����
*****************************************************************/
void USART1_IRQHandler(void)
{
	uint16_t temp;
	if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)//����ж��Ƿ���
	{	
		DMA_Cmd(DMA1_Channel5,DISABLE); //�ر�DMA����
		temp = DMA_GetCurrDataCounter(DMA1_Channel5);//��ȡʣ�����������
		//����Է������ݣ������������� = USART1_RX_SIZE - ʣ��δ���������������
        USART1_Send_Length_String(usart1_buffer,USART1_RX_SIZE-temp);
        USART1_Send_String("\n");
		DMA_SetCurrDataCounter(DMA1_Channel5,USART1_RX_SIZE);//�������ô������������
		DMA_Cmd(DMA1_Channel5,ENABLE);//����DMA����
		USART_ReceiveData(USART1);                              //��ȡһ�����ݣ���Ȼ��һֱ���ж�
		USART_ClearFlag(USART1,USART_FLAG_IDLE);                //������ڿ����жϱ�־λ
	}
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
