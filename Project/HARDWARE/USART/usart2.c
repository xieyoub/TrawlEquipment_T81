#include "usart2.h"

/***********************************************************
 * 函数名: USART2_DMA_Configuration
 * 描述  ：串口DMA配置
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART2_DMA_Configuration()
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //启动DMA时钟  

 DMA_DeInit(DMA1_Channel6); 	//USART2 DMA1通道6配置   
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);//外设地址  
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart2buf;     //内存地址  
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //dma传输方向从外设到内存 
	DMA_InitStructure.DMA_BufferSize = UART_LEN;               //设置DMA在传输时缓冲区的长度  
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设置DMA的外设地址不变 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;      //设置DMA的内存递增模式  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据字长 8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;    //内存数据字长8位   
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                //设置DMA的传输模式   
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;      //设置DMA的优先级别 最高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                 //设置DMA的2个memory中的变量互相访问  
	DMA_Init(DMA1_Channel6,&DMA_InitStructure);  

	DMA_Cmd(DMA1_Channel6,ENABLE);	
}


/***********************************************************
 * 函数名: USART2_NVIC_Configuration
 * 描述  ：串口中断配置
 * 输入  : 无
 * 输出  : 无
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
 * 函数名: USART2_GPIO_Configuration
 * 描述  ：串口GPIO配置
 * 输入  : 无
 * 输出  : 无
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
 * 函数名: USART2_MODE_Configuration
 * 描述  ：串口模式配置
 * 输入  : 无
 * 输出  : 无
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
 * 函数名: Usart2_Init
 * 描述  ：串口2配置
 * 输入  : 无
 * 输出  : 无
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
	
	USART_ITConfig(USART2,USART_IT_TC,DISABLE);    //禁止 
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);  //禁止 
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);   //开启
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);	//采用DMA方式接收  
 USART_Cmd(USART2, ENABLE);	
}


 /***********************************************************
 * 函数名: USART2_IRQHandler
 * 描述  ：配置串口2中断处理
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void USART2_IRQHandler(void)
{
	u32 Length = 0;
	u8 i;
	u16 crcdata,scrData; //校验生成的数据, 源校验的数据
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
	{  
		DMA_Cmd(DMA1_Channel6,DISABLE); 
		Length = USART2->SR;  
		Length = USART2->DR; //清USART_IT_IDLE标志  
		
		scrData = (Usart2buf[16]<<8)+Usart2buf[17];
		crcdata = msg_crc(Usart2buf,16); //数据校验
		
		if(crcdata == scrData)	
		Com2GetData();
		for(i=0;i<31;i++)
			Usart2buf[i] = 0;	
		
		DMA1_Channel6->CNDTR = UART_LEN;//重装填,并让接收地址偏址从0开始
		DMA_Cmd(DMA1_Channel6, ENABLE);//处理完,重开DMA   
	}
	__nop();
}

/***********************************************************
 * 函数名: Com2GetData
 * 描述  ：T88传过来的数据
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void Com2GetData()
{
	u8 i;
	if(Usart2buf[0]==0x24)
	{
			switch(Usart2buf[1])
		 {
				case 0x5a: //T90终端握手命令
									ConnectDevices = 2; 
									if(Reply_flag == 0)
									{
										replyAsk();
									}
									else
									{
										GPSInfoSend(); //上报GPS信息
									}
									Reply_flag = 1-Reply_flag;
				     ReadSog_Cog();
									break;
				
				case 0x58: //T88终端握手命令
									ConnectDevices = 1;
									replyAsk();
									break;
				
				case 0x32://读取
									
					    break;
				
				case 0x31: //注入
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
 * 函数名: Com2SendData
 * 描述  ：给T88发送数据
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void Com2SendData()
{
	u8 i;
	u16 data;
	
	Usart2_Output;
	//delay_us(800);
	delay_ms(1);
	if(Usart2buf[1]==0x51) //握手命令长度31
	{	
	 data = msg_crc(Usart2buf,29);	//CRC校验数据生成
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
	 data = msg_crc(Usart2buf,16);	//CRC校验数据生成
		Usart2buf[16] = data>>8;
 	Usart2buf[17] = data;
		for(i=0;i<18;i++)	
		{
			USART_SendData(USART2, Usart2buf[i]);
			while (!(USART2->SR & USART_FLAG_TXE));
		}
	}
 //BUF清空	
	for(i=0;i<32;i++)
		Usart2buf[i]=0;
	
	//delay_us(800);
	delay_ms(1);
	Usart2_Receive;
}

/***********************************************************
 * 函数名: Usart1Send
 * 描述  ：选择相应的网位仪串口发送数据
 * 输入  : 无
 * 输出  : 无
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

//打开串口命令
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


//读取命令
void ReadOffSet()
{
	u8 i;
	for(i=0;i<18;i++)
		Usart1buf[i] = 0;
	
	Usart1buf[0] = 0x24;
	Usart1buf[1] = 0x32;
	Usart1buf[7] = Net_Sel;
}
//关闭串口命令
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
 Usart2buf[2] = 1;  //上报航速航向，MMSI
//左舷MMSI 3~6
 for(i=0;i<4;i++)
		Usart2buf[i+3] = (u8)(MMSI[0]>>(24-i*8)); 
	
	//网尾MMSIs 7~10
	for(i=0;i<4;i++)
		Usart2buf[i+7] = (u8)(MMSI[1]>>(24-i*8));

	//右舷MMSI 11~14
	for(i=0;i<4;i++)
		Usart2buf[i+11] = (u8)(MMSI[2]>>(24-i*8));
		
		//三个T800的状态 15
		if(FirstReadFlag==0)
			Usart2buf[15] = (NetState[0]<<6)|(NetState[1]<<4)|(NetState[2]<<2);
		//航速航向取中值
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

//上报GPS信息
void GPSInfoSend()
{
	u8 i;
	for(i=0;i<31;i++)
		Usart2buf[i] = 0;
	Usart2buf[0] = 0x24;
	Usart2buf[1] = 0x51;
 Usart2buf[2] = 2;  //上报GPS信息
 GPS_invalid();
	
	Usart2buf[3] = EW;
	for(i=0;i<4;i++)
		Usart2buf[i+4] = longitude>>(24 - i*8); //经度4~7
	
	Usart2buf[8] = NS;
	for(i=0;i<4;i++)
		Usart2buf[i+9] = latitude>>(24 - i*8); //纬度 9~12
	
	for(i=0;i<4;i++)
	 Usart2buf[i+13] = UTCTime>>(24 - i*8); //UTC 时间 13~16
	
	for(i=0;i<4;i++)
	 Usart2buf[i+17] = UTCDate>>(24 - i*8); //UTC 日期17~20
	
	Com2SendData();
}


//读取航速航向信息，如果有三次没收到串口的应答，则判断该网位仪故障
//故障网位仪每个三秒进行一次读取判断，收到应答则判断该网位仪正常
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

//判断T800插入状态然后选择读取
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
		
	if(NetState[0]==1) //左舷正常
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
	else if(NetState[1]==1) //右舷正常
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
	else if(NetState[2]==1) //网尾
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