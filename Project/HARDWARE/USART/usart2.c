#include "usart2.h"

/***********************************************************
 * ������: USART2_DMA_Configuration
 * ����  ������DMA����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART2_DMA_Configuration()
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //����DMAʱ��  

 DMA_DeInit(DMA1_Channel6); 	//USART2 DMA1ͨ��6����   
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);//�����ַ  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart2buf;     //�ڴ��ַ  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //dma���䷽������赽�ڴ� 
	DMA_InitStructure.DMA_BufferSize = UART_LEN;               //����DMA�ڴ���ʱ�������ĳ���  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA�������ַ���� 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      //����DMA���ڴ����ģʽ  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ� 8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //�ڴ������ֳ�8λ   
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //����DMA�Ĵ���ģʽ   
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;      //����DMA�����ȼ��� ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //����DMA��2��memory�еı����������  
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);  

	DMA_Cmd(DMA1_Channel6,ENABLE);	
}


/***********************************************************
 * ������: USART2_NVIC_Configuration
 * ����  �������ж�����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART2_NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	 													
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/***********************************************************
 * ������: USART2_GPIO_Configuration
 * ����  ������GPIO����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART2_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX Control
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //RX Control
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
}


/***********************************************************
 * ������: USART2_MODE_Configuration
 * ����  ������ģʽ����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART2_MODE_Configuration()
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);	
}


/***********************************************************
 * ������: Usart2_Init
 * ����  ������2����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void Usart2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	USART_DeInit(USART2);
	USART2_DMA_Configuration();
	USART2_NVIC_Configuration();
	USART2_GPIO_Configuration();
	USART2_MODE_Configuration();
	
	USART_ITConfig(USART2,USART_IT_TC,DISABLE);    //��ֹ 
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);  //��ֹ 
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);   //����
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);	//����DMA��ʽ����  
 USART_Cmd(USART2, ENABLE);	
}


 /***********************************************************
 * ������: USART2_IRQHandler
 * ����  �����ô���2�жϴ���
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART2_IRQHandler(void)
{
	u32 Length = 0;
	u8 i;
	u16 crcdata,scrData; //У�����ɵ�����, ԴУ�������
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
	{  
		DMA_Cmd(DMA1_Channel6,DISABLE); 
		Length = USART2->SR;  
		Length = USART2->DR; //��USART_IT_IDLE��־  
		
		scrData = (Usart2buf[16]<<8)+Usart2buf[17];
		crcdata = msg_crc(Usart2buf,16); //����У��
		
		if(crcdata == scrData)	
		Com2GetData();
		for(i=0;i<18;i++)
			Usart2buf[i] = 0;	
		
		DMA1_Channel6->CNDTR = UART_LEN;//��װ��,���ý��յ�ַƫַ��0��ʼ
		DMA_Cmd(DMA1_Channel6, ENABLE);//������,�ؿ�DMA   
	}
	__nop();
}



/***********************************************************
 * ������: Com2GetData
 * ����  ��T88������������
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void Com2GetData()
{
	u8 i;
	if(Usart2buf[0]==0x24)
	{
			switch(Usart2buf[1])
		 {
				case 0x5a: //T90�ն���������
									ConnectDevices = 2; 
								 replyAsk();
									break;
				
				case 0x58: //T88�ն���������
									ConnectDevices = 1;
									replyAsk();
									break;
				
				case 0x32://��ȡ
									
					    break;
				
				case 0x31: //ע��
									TailOffset = ((Usart2buf[2])<<8)|Usart2buf[3];
									location = Usart2buf[4];
									LeftRightOffset = ((Usart2buf[5])<<8)|Usart2buf[6];
									Net_Sel = Usart2buf[7];
									OpenSerial();
									break;
				default:
					   break;
		 }
	}
}

/***********************************************************
 * ������: Com2SendData
 * ����  ����T88��������
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void Com2SendData()
{
	u8 i;
	u16 data;
	
	data = msg_crc(Usart2buf,16);	//CRCУ����������
	Usart2buf[16] = data>>8;
	Usart2buf[17] = data;
	
	Usart2_Output;
	delay_ms(5);
	for(i=0;i<18;i++)	
	{
		USART_SendData(USART2, Usart2buf[i]);
		while (!(USART2->SR & USART_FLAG_TXE));
	}
 //BUF���	
	for(i=0;i<18;i++)
		Usart2buf[i]=0;
	
	delay_ms(3);
	Usart2_Receive;
}

/***********************************************************
 * ������: Usart1Send
 * ����  ��ѡ����Ӧ����λ�Ǵ��ڷ�������
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void Usart1Send()
{
	switch (Usart1buf[7])
	{
		case 1:
						Left_Connect;
		    delay_ms(5);
						Com1SendData();	
						break;

		case 2:
						Tail_Connect;
						delay_ms(5);
						Com1SendData();
						break;

		case 3:
						Right_Connect;
						delay_ms(5);
						Com1SendData();	
						break;

			default:
						break;
	}	
}

//�򿪴�������
void OpenSerial()
{
	u8 i;
	for(i=0;i<18;i++)
		Usart1buf[i] = 0;
	
	Usart1buf[0] = 0x24;
	Usart1buf[1] = 0x17;
	Usart1buf[7] = Net_Sel;
	Usart1Send();
}
//ע��
void WriteOffset(void)
{
	u8 i;
	for(i=0;i<18;i++)
		Usart1buf[i] = 0;
	
	Usart1buf[0] = 0x24;
	Usart1buf[1] = 0x31;
	Usart1buf[2] = TailOffset>>8;
	Usart1buf[3] = TailOffset&0xff;
	Usart1buf[4] = location;
	Usart1buf[5] = LeftRightOffset>>8;
	Usart1buf[6] = LeftRightOffset&0xff;
	Usart1buf[7] = Net_Sel;
	Usart1Send();
}
//��ȡ����
void ReadOffSet()
{
	u8 i;
	for(i=0;i<18;i++)
		Usart1buf[i] = 0;
	
	Usart1buf[0] = 0x24;
	Usart1buf[1] = 0x32;
	Usart1buf[7] = Net_Sel;
}
//�رմ�������
void CloseSerial()
{
	u8 i;
	for(i=0;i<18;i++)
		Usart1buf[i] = 0;
	
	Usart1buf[0] = 0x24;
	Usart1buf[1] = 0x18;
	Usart1buf[7] = Net_Sel;
	Usart1Send();
}

//
void replyAsk(void)
{
	u8 i;
	for(i=0;i<18;i++)
		Usart2buf[i] = 0;
	
	Usart2buf[0] = 0x24;
	Usart2buf[1] = 0x51;

//����MMSI
 for(i=0;i<4;i++)
		Usart2buf[i+2] = (u8)(MMSI[0]>>(24-i*8)); 
	
	//��βMMSIs
	for(i=0;i<4;i++)
		Usart2buf[i+6] = (u8)(MMSI[1]>>(24-i*8));

	//����MMSI
	for(i=0;i<4;i++)
		Usart2buf[i+10] = (u8)(MMSI[2]>>(24-i*8));
		
		//����T800��״̬
		if(FirstReadFlag==0)
			Usart2buf[14] = (NetState[0]<<6)|(NetState[1]<<4)|(NetState[2]<<2);
	Com2SendData();
}

