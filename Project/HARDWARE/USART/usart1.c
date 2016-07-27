#include "usart1.h"

static u8 Uart1_Rx_Num = 0;
static u8 ReceiveSta = 0;
/***********************************************************
 * ������: USART1_DMA_Configuration
 * ����  ������DMA����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_DMA_Configuration()
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //����DMAʱ��  

 DMA_DeInit(DMA1_Channel5); 	//USART1 DMA1ͨ��5����   
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);//�����ַ  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart1buf;     //�ڴ��ַ  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //dma���䷽������赽�ڴ� 
	DMA_InitStructure.DMA_BufferSize = UART_LEN;               //����DMA�ڴ���ʱ�������ĳ���  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����DMA�������ַ���� 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      //����DMA���ڴ����ģʽ  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//���������ֳ� 8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //�ڴ������ֳ�8λ   
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //����DMA�Ĵ���ģʽ   
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;      //����DMA�����ȼ��� ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //����DMA��2��memory�еı����������  
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);  

	DMA_Cmd(DMA1_Channel5,ENABLE);	
}
	

/***********************************************************
 * ������: USART1_NVIC_Configuration
 * ����  �������ж�����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure; 
													
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
}


/***********************************************************
 * ������: USART1_GPIO_Configuration
 * ����  ������GPIO����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);		
}


/***********************************************************
 * ������: USART1_MODE_Configuration
 * ����  ������ģʽ����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void USART1_MODE_Configuration()
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);	
}	
	

/***********************************************************
 * ������: Usart1_Init
 * ����  ������1����
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void Usart1_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	USART_DeInit(USART1);
	USART1_NVIC_Configuration();
	USART1_GPIO_Configuration();
	USART1_MODE_Configuration();
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //��ֹ 
 USART_Cmd(USART1, ENABLE);		
}


 /***********************************************************
 * ������: USART1_IRQHandler
 * ����  �����ô���1�жϴ���
 * ����  : ��
 * ���  : ��
 ***********************************************************/

void USART1_IRQHandler(void)
{
	u8 Data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		Data=USART1->DR; 
		
  if(Data==0x24)
			ReceiveSta = 1;
		//��ʼ����
		if(ReceiveSta)
		{			
			Usart1buf[Uart1_Rx_Num] = Data;
			Uart1_Rx_Num++;
		}
		
		//���ս���
		if(Uart1_Rx_Num==18)
		{
			Uart1_Rx_Num = 0;
			Com1GetData();
			ReceiveSta = 0;
		 USART_ClearITPendingBit(USART1,USART_IT_RXNE); //�����־λ
		}
	}
}

/***********************************************************
 * ������: Com1GetData
 * ����  ����ȡT800����������
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void Com1GetData()
{
		if(Usart1buf[0] == 0x24)
		{
			switch (Usart1buf[1])
			{
				case 0x17: //�򿪴���
									WriteOffset();
									break;
				
				case 0x18: //�رմ��� //�յ�T800�Ļظ����˳�д��״̬
									Usart2buf[0] = 0x24;
									Usart2buf[1] = 0x31;
									Usart2buf[7] = Usart1buf[7];
									Com2SendData();
									break;
				
				case 0x31: //ע��
									CloseSerial();
									break;
				
				case 0x32: //��ȡ
								{		
									if(FirstReadFlag) //������һ�ζ�ȡ
									{
										FirstRead();
									}
									else              
									{
										//�ж��յ����������ĸ���λ�ǵ�
										TIM_Cmd(TIM2,DISABLE);
										switch (Usart1buf[7])
										{
											case 1:
																NetState[0] = 1;
																MMSI[0] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
																break;
											case 2:
																NetState[1] = 1;
																MMSI[1] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
																break;
											case 3:
																NetState[2] = 1;
																MMSI[2] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
																break;
										}
									}
								}
								break;
			}
		}
}

/***********************************************************
 * ������: Com1SendData
 * ����  ����T800��������
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void Com1SendData()
{
	u8 i;
	for(i=0;i<18;i++)	
	{
		USART_SendData(USART1, Usart1buf[i]);
		while (!(USART1->SR & USART_FLAG_TXE));
	}
	for(i=0;i<18;i++)
		Usart1buf[i] = 0;
}

/***********************************************************
 * ������: msg_crc
 * ����  ��CRCУ��
 * ����  : ��
 * ���  : ��
 ***********************************************************/
u16 msg_crc(u8 *ptr,u8 num)
{
	u16 crc=0xffff;
	u8 i;
	u16 gx=0x1021;
	while(num--)
	{
		for(i=0x01;i!=0;i<<=1)
		{
			if((crc&0x8000)!=0)
			{
				crc<<=1;
				crc^=gx;
			}
			else
			{
				crc<<=1;
			}
			if(((*ptr)&i)!=0)
			{
				crc^=gx;
			}
		}
		ptr++;
	}
	return ~crc;
}


/***********************************************************
 * ������: ReadInit
 * ����  ��������ȡ
 * ����  : ��
 * ���  : ��
 ***********************************************************/
void ReadInit()
{
	FirstReadFlag=1;
	if(Left_Net)//����
	{	
		Net_Sel = 1;
		ReadOffSet();
		Usart1Send();
		
		tim2Enable();
	}
	else if(Tail_Net)//��β
	{
		NetState[0] = 0;
		Net_Sel = 2;
		ReadOffSet();
		Usart1Send();			
		
  tim2Enable();
	}
	else if(Right_Net)//����
	{
		NetState[0] = 0;
		NetState[1] = 0;
		Net_Sel = 3;
		ReadOffSet();
		Usart1Send();		
		
  tim2Enable();
	}
	else
	{
		NetState[0] = 0;
		NetState[1] = 0;
		NetState[2] = 0;
		FirstReadFlag = 0;
	}
}

//������һ�ζ�ȡ�Ѳ������λ�� ����˳���ȡ�����ϡ���β������
void FirstRead()
{
		if(Usart1buf[7] == 1)//����
		{
			TIM_Cmd(TIM2,DISABLE);
			NetState[0] = 1;
			MMSI[0] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
			if(Tail_Net)//������ȡ��һ�� (��β)
			{
				Net_Sel = 2;
				ReadOffSet();
				Usart1Send();
				tim2Enable();
			}
			else if(Right_Net) //��βû���룬��ȡ����
			{
				NetState[1] = 0;
				Net_Sel = 3;
				ReadOffSet();
				Usart1Send();			
				tim2Enable();											
			}
			else
			{
				NetState[1] = 0;
				NetState[2] = 0;
				FirstReadFlag = 0;
			}
		}
		else if(Usart1buf[7] == 2)//��β
		{
			TIM_Cmd(TIM2,DISABLE);
			NetState[1] = 1;
			MMSI[1] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
			if(Right_Net) //��ȡ����
			{
				Net_Sel = 3;
				ReadOffSet();
				Usart1Send();														
				tim2Enable();		
			}
			else
			{
				NetState[2] = 0;
				FirstReadFlag = 0;
			}
		}
		else if(Usart1buf[7] == 3)//����
		{			
			TIM_Cmd(TIM2,DISABLE);
			NetState[2] = 1;
			MMSI[2] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
			FirstReadFlag = 0;
		}
}

