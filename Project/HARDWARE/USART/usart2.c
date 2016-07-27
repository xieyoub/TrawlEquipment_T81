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
		for(i=0;i<18;i++)
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
								 replyAsk();
									break;
				
				case 0x58: //T88终端握手命令
									ConnectDevices = 1;
									replyAsk();
									break;
				
				case 0x32://读取
									
					    break;
				
				case 0x31: //注入
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
 * 函数名: Com2SendData
 * 描述  ：给T88发送数据
 * 输入  : 无
 * 输出  : 无
 ***********************************************************/
void Com2SendData()
{
	u8 i;
	u16 data;
	
	data = msg_crc(Usart2buf,16);	//CRC校验数据生成
	Usart2buf[16] = data>>8;
	Usart2buf[17] = data;
	
	Usart2_Output;
	delay_ms(5);
	for(i=0;i<18;i++)	
	{
		USART_SendData(USART2, Usart2buf[i]);
		while (!(USART2->SR & USART_FLAG_TXE));
	}
 //BUF清空	
	for(i=0;i<18;i++)
		Usart2buf[i]=0;
	
	delay_ms(3);
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
//注入
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
	for(i=0;i<18;i++)
		Usart2buf[i] = 0;
	
	Usart2buf[0] = 0x24;
	Usart2buf[1] = 0x51;

//左舷MMSI
 for(i=0;i<4;i++)
		Usart2buf[i+2] = (u8)(MMSI[0]>>(24-i*8)); 
	
	//网尾MMSIs
	for(i=0;i<4;i++)
		Usart2buf[i+6] = (u8)(MMSI[1]>>(24-i*8));

	//右舷MMSI
	for(i=0;i<4;i++)
		Usart2buf[i+10] = (u8)(MMSI[2]>>(24-i*8));
		
		//三个T800的状态
		if(FirstReadFlag==0)
			Usart2buf[14] = (NetState[0]<<6)|(NetState[1]<<4)|(NetState[2]<<2);
	Com2SendData();
}

