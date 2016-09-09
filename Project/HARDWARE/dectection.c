#include "dectection.h"


void Dectect_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //num1 ,num2 ,num3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
}
	
	
void Dectect_NIVC_Config()
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 		
}
	

void Dectect_EXTI_Config()
{
 EXTI_InitTypeDef EXTI_InitStructure;		
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_ClearITPendingBit(EXTI_Line5);
	EXTI_Init(&EXTI_InitStructure);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_ClearITPendingBit(EXTI_Line6);
	EXTI_Init(&EXTI_InitStructure);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_ClearITPendingBit(EXTI_Line7);
	EXTI_Init(&EXTI_InitStructure);		
}

//T800插入检测配置
void T800Dectect()
{
 Dectect_GPIO_Config();
	Dectect_NIVC_Config();
	Dectect_EXTI_Config();
}


//检测T800插入情况
void EXTI9_5_IRQHandler(void)
{
	//左舷检测
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)         
	{
		delay_ms(1000); //等待T800稳定
		if(Left_Net)//插入
		{
			NetState[0] = 1;
//			Net_Sel = 1;
//			ReadOffSet();
//			Usart1Send();
//			tim2Enable();
		}
		else  //拔出
		{
			MMSI[0] = 0;
			NetState[0] = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	
	 //网尾检测
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)     
	{
		delay_ms(1000); //等待T800稳定
		if(Tail_Net)//插入
		{
			NetState[1] = 1;
//			Net_Sel = 2;
//			ReadOffSet();
//			Usart1Send();
//			tim2Enable();
		}	
		else  //拔出
		{
			MMSI[1] = 0;
			NetState[1] = 0;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	
	//右舷检测
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)  
	{
		delay_ms(1000); //等待T800稳定
		if(Right_Net) //插入
		{
			NetState[2] = 1;
//			Net_Sel = 3;
//			ReadOffSet();
//			Usart1Send();
//			tim2Enable();
		}		
		else  //拔出
		{
			MMSI[2] = 0;
			NetState[2] = 0;
		}
		
		//清除标志位
		EXTI_ClearITPendingBit(EXTI_Line5);
	}	
}

