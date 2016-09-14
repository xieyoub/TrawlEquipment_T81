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
		for(i=0;i<31;i++)
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
									if(Reply_flag == 0)
									{
										replyAsk();
									}
									else
									{
										GPSInfoSend(); //�ϱ�GPS��Ϣ
									}
									Reply_flag = 1-Reply_flag;
				     ReadSog_Cog();
									break;
				
				case 0x58: //T88�ն���������
									ConnectDevices = 1;
									replyAsk();
									break;
				
				case 0x32://��ȡ
									
					    break;
				
				case 0x31: //ע��
					   for(i=0;i<18;i++)
				      Usart1buf[i] = Usart2buf[i];
				     Usart1Send();
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
	
	Usart2_Output;
	//delay_us(800);
	delay_ms(1);
	if(Usart2buf[1]==0x51) //���������31
	{	
	 data = msg_crc(Usart2buf,29);	//CRCУ����������
		Usart2buf[29] = data>>8;
 	Usart2buf[30] = data;
		for(i=0;i<31;i++)	
		{
			USART_SendData(USART2, Usart2buf[i]);
			while (!(USART2->SR & USART_FLAG_TXE));
		}
	}
	else
	{	
	 data = msg_crc(Usart2buf,16);	//CRCУ����������
		Usart2buf[16] = data>>8;
 	Usart2buf[17] = data;
		for(i=0;i<18;i++)	
		{
			USART_SendData(USART2, Usart2buf[i]);
			while (!(USART2->SR & USART_FLAG_TXE));
		}
	}
 //BUF���	
	for(i=0;i<32;i++)
		Usart2buf[i]=0;
	
	//delay_us(800);
	delay_ms(1);
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
	for(i=0;i<31;i++)
		Usart2buf[i] = 0;
	
	Usart2buf[0] = 0x24;
	Usart2buf[1] = 0x51;
 Usart2buf[2] = 1;  //�ϱ����ٺ���MMSI
//����MMSI 3~6
 for(i=0;i<4;i++)
		Usart2buf[i+3] = (u8)(MMSI[0]>>(24-i*8)); 
	
	//��βMMSIs 7~10
	for(i=0;i<4;i++)
		Usart2buf[i+7] = (u8)(MMSI[1]>>(24-i*8));

	//����MMSI 11~14
	for(i=0;i<4;i++)
		Usart2buf[i+11] = (u8)(MMSI[2]>>(24-i*8));
		
		//����T800��״̬ 15
		if(FirstReadFlag==0)
			Usart2buf[15] = (NetState[0]<<6)|(NetState[1]<<4)|(NetState[2]<<2);
		//���ٺ���ȡ��ֵ
		if(NetState[0]!=0|NetState[1]!=0|NetState[1]!=0)
			mid_value(); 
		else
		{
			SOG = 0;
		 COG = 0;
		}
		Usart2buf[16] = SOG>>8;
		Usart2buf[17] = SOG;
		Usart2buf[18] = COG>>8;
		Usart2buf[19] = COG;
	 Com2SendData();
}

//�ϱ�GPS��Ϣ
void GPSInfoSend()
{
	u8 i;
	for(i=0;i<31;i++)
		Usart2buf[i] = 0;
	Usart2buf[0] = 0x24;
	Usart2buf[1] = 0x51;
 Usart2buf[2] = 2;  //�ϱ�GPS��Ϣ
 GPS_invalid();
	
	Usart2buf[3] = EW;
	for(i=0;i<4;i++)
		Usart2buf[i+4] = longitude>>(24 - i*8); //����4~7
	
	Usart2buf[8] = NS;
	for(i=0;i<4;i++)
		Usart2buf[i+9] = latitude>>(24 - i*8); //γ�� 9~12
	
	for(i=0;i<4;i++)
	 Usart2buf[i+13] = UTCTime>>(24 - i*8); //UTC ʱ�� 13~16
	
	for(i=0;i<4;i++)
	 Usart2buf[i+17] = UTCDate>>(24 - i*8); //UTC ����17~20
	
	Com2SendData();
}


//��ȡ���ٺ�����Ϣ�����������û�յ����ڵ�Ӧ�����жϸ���λ�ǹ���
//������λ��ÿ���������һ�ζ�ȡ�жϣ��յ�Ӧ�����жϸ���λ������
void ReadSog_Cog(void)
{
	u8 i;
	Usart1buf[0] = 0x24;
 Usart1buf[1] = 0x06;
	for(i=2;i<18;i++)
	{
		Usart1buf[i]=0;
	}
	
		NetStatejudge();
}

void Read_GPSInfo()
{
		u8 i;
	Usart1buf[0] = 0x24;
 Usart1buf[1] = 0x07;
	for(i=2;i<18;i++)
	{
		Usart1buf[i]=0;
	}
	NetStatejudge();
}

//�ж�T800����״̬Ȼ��ѡ���ȡ
void NetStatejudge()
{
	 if(Left_Net)
	{
	 if(NetState[0]==1 | faultCount[0]==3)
	 {
		 if(faultCount[0]==3)
		 {
			 faultCount[0]=0;
			 NetState[0]=2;
		 }
		 faultCount[0]++;
		 Usart1buf[7]=1;	
		 Usart1Send();
	 }
	 else if(faultCount[0]<3)
	 {
		 faultCount[0]++;
	 }
	}
 else if(Tail_Net)
	{
		if(NetState[1]==1 | faultCount[1]==3)
		{
			if(faultCount[1]==3)
			{
				faultCount[1]=0;
				NetState[1] = 2;
			}
			faultCount[1]++;
			Usart1buf[7]=2;	
			Usart1Send();
		}
		else if(faultCount[1]<3)
		{
			faultCount[1]++;
		}
	}
 else if(Right_Net)
	{
		if(NetState[2]==1 | faultCount[2]==3)
		{
			if(faultCount[2]==3)
			{
			 faultCount[2] = 0;
		   NetState[2] = 2;
			}
			faultCount[2]++;
			Usart1buf[7]=3;	
			Usart1Send();
		}
		else if(faultCount[2]<3)
		{
			faultCount[2]++;
		}
	}
}

void GPS_invalid()
{
		
	if(NetState[0]==1) //��������
	{
		if(GPS.longitude[0]!=0)
		{
			EW = GPS.EW[0];
			longitude = GPS.longitude[0];
			NS = GPS.NS[0];
			latitude = GPS.latitude[0];
			UTCTime = GPS.UTCTime[0];
			UTCDate = GPS.UTCDate[0];
		}
		else if(NetState[1]==1)
		{
				if(GPS.longitude[1]!=0)
				{
					EW = GPS.EW[1];
					longitude = GPS.longitude[1];
					NS = GPS.NS[1];
					latitude = GPS.latitude[1];
					UTCTime = GPS.UTCTime[1];
					UTCDate = GPS.UTCDate[1];
				}
				else if(NetState[2]==1)
				{
					if(GPS.longitude[2]!=0)
					{
						EW = GPS.EW[2];
						longitude = GPS.longitude[2];
						NS = GPS.NS[2];
						latitude = GPS.latitude[2];
						UTCTime = GPS.UTCTime[2];
						UTCDate = GPS.UTCDate[2];
					}
				}
		}
	}
	else if(NetState[1]==1) //��������
	{
			if(GPS.longitude[1]!=0)
			{
				EW = GPS.EW[1];
				longitude = GPS.longitude[1];
				NS = GPS.NS[1];
				latitude = GPS.latitude[1];
				UTCTime = GPS.UTCTime[1];
				UTCDate = GPS.UTCDate[1];
			}
			else if(NetState[2]==1)
			{
				if(GPS.longitude[2]!=0)
				{
					EW = GPS.EW[2];
					longitude = GPS.longitude[2];
					NS = GPS.NS[2];
					latitude = GPS.latitude[2];
					UTCTime = GPS.UTCTime[2];
					UTCDate = GPS.UTCDate[2];
				}
			}
	}
	else if(NetState[2]==1) //��β
	{
		if(GPS.longitude[2]!=0)
		{
			EW = GPS.EW[2];
			longitude = GPS.longitude[2];
			NS = GPS.NS[2];
			latitude = GPS.latitude[2];
			UTCTime = GPS.UTCTime[2];
			UTCDate = GPS.UTCDate[2];
		}
	}
}