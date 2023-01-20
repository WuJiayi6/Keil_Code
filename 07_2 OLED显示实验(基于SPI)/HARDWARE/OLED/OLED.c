#include "OLED.h"

/*******************************************************************
 * @name       :void OLED_WR_Byte(unsigned dat,unsigned cmd)
 * @function   :��һ���ֽڵ�����д��OLED��Ļ
 * @parameters :dat:Ҫд������
                cmd:0-д���� 1-д����
 * @retvalue   :��
********************************************************************/
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		OLED_DC_Set();
	}
	else
	{
		OLED_DC_Clr();
	}
	OLED_CS_Clr();
	SPI_WriteByte(dat);
	OLED_CS_Set();
}


/*******************************************************************
 * @name       :void OLED_Set_Pos(unsigned char x, unsigned char y) 
 * @function   :��OLED��Ļ����������
 * @parameters :x: X����
                y: Y����
 * @retvalue   :��
********************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
 	OLED_WR_Byte(YLevel+y/PAGE_SIZE,OLED_CMD);
	OLED_WR_Byte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte(((x+2)&0x0f),OLED_CMD); 
}  


/*******************************************************************
 * @name       :void OLED_Display_On(void) 
 * @function   :��OLED��ʾ��
 * @parameters :��
 * @retvalue   :��
********************************************************************/ 	  
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}


/*******************************************************************
 * @name       :void OLED_Display_Off(void)
 * @function   :�ر�OLED��ʾ��
 * @parameters :��
 * @retvalue   :��
********************************************************************/    
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}


/*******************************************************************
 * @name       :void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color)
 * @function   :��������ֵΪRAM
 * @parameters :x:���ص�x����
                y:���ص�y����
				color:�����ɫֵ 1-��ɫ 0-��ɫ
 * @retvalue   :��
********************************************************************/ 
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color)
{
	if(color)
	{
		OLED_buffer[(y/PAGE_SIZE)*WIDTH+x]|= (1<<(y%PAGE_SIZE))&0xff;
	}
	else
	{
		OLED_buffer[(y/PAGE_SIZE)*WIDTH+x]&= ~((1<<(y%PAGE_SIZE))&0xff);
	}
}		   			 


/*******************************************************************
 * @name       :void OLED_Display(void)
 * @function   :OLED��Ļ��ʾ
 * @parameters :��
 * @retvalue   :��
********************************************************************/  
void OLED_Display(void)
{
	u8 i,n;		    
	for(i=0;i<PAGE_SIZE;i++)  
	{  
		OLED_WR_Byte (YLevel+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (XLevelL,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (XLevelH,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<WIDTH;n++)
		{
			OLED_WR_Byte(OLED_buffer[i*WIDTH+n],OLED_DATA); 
		}
	}   //������ʾ
}


/*******************************************************************
 * @name       :void OLED_Clear(unsigned dat)  
 * @function   :���OLED��Ļ
 * @parameters :dat:0-��ʾȫ��
                    1-��ʾȫ��
 * @retvalue   :��
********************************************************************/ 
void OLED_Clear(unsigned dat)  
{  
	if(dat)
	{
		memset(OLED_buffer,0xff,sizeof(OLED_buffer));
	}
	else
	{
		memset(OLED_buffer,0,sizeof(OLED_buffer));
	}
	OLED_Display();
}


/*******************************************************************
 * @name       :void OLED_Reset(void) 
 * @function   :����OLED��Ļ
 * @parameters :dat:0-��ʾȫ��
                    1-��ʾȫ��
 * @retvalue   :��
********************************************************************/ 
void OLED_Reset(void)
{
	OLED_RST_Set();
	Delay_ms(100);
	OLED_RST_Clr();
	Delay_ms(100);
	OLED_RST_Set(); 
}	


/*******************************************************************
 * @name       :void OLED_Hardware_Init(void)
 * @function   :��λOLED��
 * @parameters :��
 * @retvalue   :��
********************************************************************/ 
void OLED_Hardware_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);//ʹ��BC�˿�ʱ��
	
	/************    CS   ******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	//GPIOB12 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOB12

	/************    RES   ******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//GPIOC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOC6
	
	/************    DC  ����д�����д����  ******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOC7
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);//����
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);//����
}

/*******************************************************************
 * @name       :void OLED_Init(void)
 * @function   :��ʼ��OLED SSD1306����IC
 * @parameters :��
 * @retvalue   :��
********************************************************************/ 				    
void OLED_Init(void)
{
	SPI2_Hardware_Init();  //��ʼ��SPI2
 	OLED_Hardware_Init(); //��ʼ��GPIO
 	Delay_ms(200);
	OLED_Reset();     //��λOLED
	
/**************��ʼ��SSD1306*****************/	
	OLED_WR_Byte(0xAE,OLED_CMD); /*�ر���ʾ*/
	OLED_WR_Byte(0x00,OLED_CMD); /*���õ��е�ַ*/
	OLED_WR_Byte(0x10,OLED_CMD); /*���ø��е�ַ*/
	OLED_WR_Byte(0x40,OLED_CMD); /*������ʾ��ʼ��*/ 
	OLED_WR_Byte(0xB0,OLED_CMD); /*����ҳ��ַ*/
	OLED_WR_Byte(0x81,OLED_CMD); /*���Ͽ���*/ 
	OLED_WR_Byte(0xFF,OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1,OLED_CMD); /*���ò�������ӳ��*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x3F,OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com ɨ�跽��*/
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0XF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x30,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD); /*set charge pump disable*/ 
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD); /* ����ʾ*/
}  

