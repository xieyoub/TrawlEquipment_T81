#include "usart1.h"

static u8 Uart1_Rx_Num = 0;
static u8 ReceiveSta = 0;
/***********************************************************
 * 函数名: USART1_DMA_Configuration
 * 描述  ：串口DMA配置
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART1_DMA_Configuration()
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //启动DMA时钟  

 DMA_DeInit(DMA1_Channel5); 	//USART1 DMA1通道5配置   
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);//外设地址  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart1buf;     //内存地址  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //dma传输方向从外设到内存 
	DMA_InitStructure.DMA_BufferSize = UART_LEN;               //设置DMA在传输时缓冲区的长度  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA的外设地址不变 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      //设置DMA的内存递增模式  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据字长 8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //内存数据字长8位   
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //设置DMA的传输模式   
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;      //设置DMA的优先级别 最高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //设置DMA的2个memory中的变量互相访问  
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);  

	DMA_Cmd(DMA1_Channel5,ENABLE);	
}
	

/***********************************************************
 * 函数名: USART1_NVIC_Configuration
 * 描述  ：串口中断配置
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART1_NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure; 
													
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
}


/***********************************************************
 * 函数名: USART1_GPIO_Configuration
 * 描述  ：串口GPIO配置
 * 输入  : 无
 * 输出  : 无
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
 * 函数名: USART1_MODE_Configuration
 * 描述  ：串口模式配置
 * 输入  : 无
 * 输出  : 无
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
 * 函数名: Usart1_Init
 * 描述  ：串口1配置
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void Usart1_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);
	USART_DeInit(USART1);
	USART1_NVIC_Configuration();
	USART1_DMA_Configuration();
	USART1_GPIO_Configuration();
	USART1_MODE_Configuration();
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //禁止 
	USART_ITConfig(USART1,USART_IT_TC,DISABLE);    //禁止 
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);   //开启
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);	//采用DMA方式接收 
 USART_Cmd(USART1, ENABLE);		
}


void USART1_IRQHandler(void)
{   
	u8 i;
	uint32_t Length = 0;//,i=0;
	u16 crcdata,scrData; //校验生成的数据, 源校验的数据
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
	{  
		DMA_Cmd(DMA1_Channel5,DISABLE); 
		Length = USART1->SR;  
		Length = USART1->DR; //清USART_IT_IDLE标志
		
		if(Usart1buf[1]==0x06)//GPS信息
		{
				scrData = (Usart1buf[30]<<8)+Usart1buf[31];
				crcdata = msg_crc(Usart1buf,30); //数据校验
		}
		else
		{
			scrData = (Usart1buf[16]<<8)+Usart1buf[17];
			crcdata = msg_crc(Usart1buf,16); //数据校验
		}
		if(scrData == crcdata)
			Com1GetData();
		
		DMA1_Channel5->CNDTR = UART_LEN;//重装填,并让接收地址偏址从0开始
		for(i=0;i<UART_LEN;i++)
				Usart1buf[i] = 0;
		DMA_Cmd(DMA1_Channel5, ENABLE);//处理完,重开DMA   
		
		//USART_ClearITPendingBit(USART1,USART_IT_IDLE);
	}
	__nop();   	
	
}
// /***********************************************************
// * 函数名: USART1_IRQHandler
// * 描述  ：配置串口1中断处理
// * 输入  : 无
// * 输出  : 无
// ***********************************************************/

//void USART1_IRQHandler(void)
//{
//	u8 Data;
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
//	{
//		Data=USART1->DR; 
//		
//  if(Data==0x24)
//			ReceiveSta = 1;
//		//开始接收
//		if(ReceiveSta)
//		{			
//			Usart1buf[Uart1_Rx_Num] = Data;
//			Uart1_Rx_Num++;
//		}
//		
//		//接收结束
//		if(Uart1_Rx_Num==18)
//		{
//			Uart1_Rx_Num = 0;
//			Com1GetData();
//			ReceiveSta = 0;
//		 USART_ClearITPendingBit(USART1,USART_IT_RXNE); //清楚标志位
//		}
//	}
//}

/***********************************************************
 * 函数名: Com1GetData
 * 描述  ：获取T800发来的数据
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void Com1GetData()
{
	u8 i;
		if(Usart1buf[0] == 0x24)
		{
			switch (Usart1buf[1])
			{
					case 0x06: //航速航向,GPS信息
						    switch(Usart1buf[7])
										{
											case 1:
																NetState[0] = 1;
																faultCount[0] = 0;
																T800Info(0);
										
																Usart1buf[0] = 0x24;
																Usart1buf[1] = 0x06;
											      //收到左舷回复，请求网尾航速航向
																if(Tail_Net)
																{
																	if(NetState[1]==1 | faultCount[1]==3)
																	{
																		if(faultCount[1]==3)
																		{
																			faultCount[1]=0;
																			NetState[1] = 2;
																		}
																		Usart1buf[7] = 2;
																		faultCount[1]++;
																		Usart1Send();
																	}
																	else if(faultCount[1]<3)
																	{
																		faultCount[1]++;
																	}
																}
																else if(Right_Net)//网尾未插入，请求右弦
																{
																	if(NetState[2]==1 | faultCount[2]==3)
																	{
																		if(faultCount[2]==3)
																		{
																			faultCount[2]=0;
																			NetState[2] = 2;
																		}
																		Usart1buf[7] = 3;
																		faultCount[2]++;
																		Usart1Send();
																	}
																	else if(faultCount[2]<3)
																	{
																		faultCount[2]++;
																	}
																}	
												    break;
											
											 case 2:
																	NetState[1] = 1;
																	faultCount[1] = 0;
																	T800Info(1);//将GPS等信息保存
												
																	Usart1buf[0] = 0x24;
																	Usart1buf[1] = 0x06;
																	
															
																	//收到网尾的回复，请求右弦航速航向
																	if(Right_Net)
																	{
																		if(NetState[2]==1 | faultCount[2]==3)
																		{
																			if(faultCount[2]==3)
																			{
																				faultCount[2] = 0;
																				NetState[2] = 2;
																			}
																			Usart1buf[7]=3;	
																			faultCount[2]++;
																			Usart1Send();
																		}
																		else if(faultCount[2]<3)
																		{
																			faultCount[2]++;
																		}
																	}	
											      break;
											
											case 3:
													  	NetState[2] = 1;
												    faultCount[2] = 0;
													  	T800Info(2);
												    break;
										}
						    break;
									
//				case 0x17: //打开串口
//									WriteOffset();
//									break;
				
//				case 0x18: //关闭串口 //收到T800的回复后退出写码状态
//									Usart2buf[0] = 0x24;
//									Usart2buf[1] = 0x31;
//									Usart2buf[7] = Usart1buf[7];
//									Com2SendData();
//									break;
				
				case 0x31: //注入
					    for(i=0;i<18;i++)
				      Usart2buf[i] = Usart1buf[i];
				     Com2SendData();
									//CloseSerial();
									break;
				
				case 0x32: //读取
								{		
									if(FirstReadFlag) //开机第一次读取
									{
										FirstRead();
									}
								}
								break;
			}
		}
}

/***********************************************************
 * 函数名: Com1SendData
 * 描述  ：给T800发送数据
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void Com1SendData()
{
	u8 i;
	u16 data;
	data = msg_crc(Usart1buf,16);	//CRC校验数据生成
	Usart1buf[16] = data>>8;
 Usart1buf[17] = data;
	for(i=0;i<18;i++)	
	{
		USART_SendData(USART1, Usart1buf[i]);
		while (!(USART1->SR & USART_FLAG_TXE));
	}
	for(i=0;i<18;i++)
		Usart1buf[i] = 0;
}

/***********************************************************
 * 函数名: msg_crc
 * 描述  ：CRC校验
 * 输入  : 无
 * 输出  : 无
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
 * 函数名: ReadInit
 * 描述  ：开机读取
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void ReadInit()
{
	FirstReadFlag=1;
	if(Left_Net)//左舷
	{	
		Net_Sel = 1;
		ReadOffSet();
		Usart1Send();
		
		tim2Enable();
	}
	else if(Tail_Net)//船尾
	{
		NetState[0] = 0;
		Net_Sel = 2;
		ReadOffSet();
		Usart1Send();			
		
  tim2Enable();
	}
	else if(Right_Net)//右舷
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

//开机第一次读取已插入的网位仪 、按顺序读取，左舷、船尾、右舷
void FirstRead()
{
		if(Usart1buf[7] == 1)//左舷
		{
			TIM_Cmd(TIM2,DISABLE);
			NetState[0] = 1;
			MMSI[0] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
			if(Tail_Net)//继续读取下一个 (船尾)
			{
				Net_Sel = 2;
				ReadOffSet();
				Usart1Send();
				tim2Enable();
			}
			else if(Right_Net) //船尾没插入，读取右舷
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
		else if(Usart1buf[7] == 2)//船尾
		{
			TIM_Cmd(TIM2,DISABLE);
			NetState[1] = 1;
			MMSI[1] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
			if(Right_Net) //读取右舷
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
		else if(Usart1buf[7] == 3)//右舷
		{			
			TIM_Cmd(TIM2,DISABLE);
			NetState[2] = 1;
			MMSI[2] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11];
			FirstReadFlag = 0;
		}
}

//T800上报的信息(经纬度，时间，航速航向)
void T800Info(int flag)
{
	SogData[flag] = Usart1buf[2]<<8|Usart1buf[3]; //航速
	CogData[flag] = Usart1buf[4]<<8|Usart1buf[5];	//航向
	MMSI[flag] = Usart1buf[8]<<24 | Usart1buf[9]<<16 | Usart1buf[10]<<8 | Usart1buf[11]; //MMSI
	GPS.EW[flag] = Usart1buf[12];
	GPS.longitude[flag] = Usart1buf[13]<<24 | Usart1buf[14]<<16 | Usart1buf[15]<<8 |Usart1buf[16];	//经度
	GPS.NS[flag] = Usart1buf[17];
	GPS.latitude[flag] = Usart1buf[18]<<24 | Usart1buf[19]<<16 | Usart1buf[20]<<8 | Usart1buf[21];	//纬度
	GPS.UTCTime[flag] = Usart1buf[22]<<24 | Usart1buf[23]<<16 | Usart1buf[24]<<8 | Usart1buf[25];		//UTC时间
	GPS.UTCDate[flag] = Usart1buf[26]<<24 | Usart1buf[27]<<16 | Usart1buf[28]<<8 | Usart1buf[29];		//UTC日期
}