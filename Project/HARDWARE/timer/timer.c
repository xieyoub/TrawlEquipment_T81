#include "timer.h"

void time2_init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_TimeBaseStructure.TIM_Period = 24999;   //1s
	TIM_TimeBaseStructure.TIM_Prescaler = 2879;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}

//读取超时处理，再次读取，总次数不超过3次
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		ReadTime++;
		if(ReadTime<=1) 
		{
			ReadOffSet();
			Usart1Send();
		}
		else //判断为故障
		{	
			switch(Net_Sel)
			{
				case 1:
									NetState[0] = 2;
									MMSI[0] = 0;
									break;
				case 2:
									NetState[1] = 2;
									MMSI[1] = 0;
									break;
				case 3:
									NetState[2] = 2;
									MMSI[2] = 0;
									break;	
			}
			if(FirstReadFlag==0) //不是第一次读取
			{
				TIM_Cmd(TIM2,DISABLE);
			}
			else  //第一次读取
			{
				Net_Sel++;
				if(Net_Sel<=3) //继续读取下一个
				{
					ReadNext();
				}
				else //三个网位仪都读取完
				{
					TIM_Cmd(TIM2,DISABLE);
					FirstReadFlag = 0;
				}
				ReadTime = 0;
			}
		}
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

//使能定时器2
void tim2Enable(void)
{
	ReadTime = 0;
	TIM2->CNT = 0;
	TIM_Cmd(TIM2,ENABLE);
}

void ReadNext(void)
{
	if(Net_Sel == 2 && Tail_Net) //第二个判断已插上
	{
		Usart1buf[7] = Net_Sel;
		ReadOffSet();
		Usart1Send();
	}
	else if(Right_Net && Net_Sel == 2) //第三个插上
	{
		Net_Sel++;
		Usart1buf[7] = Net_Sel;
		ReadOffSet();
		Usart1Send();		
	}
	else if(Net_Sel == 3 && Right_Net) //第三个
	{
		Usart1buf[7] = Net_Sel;
		ReadOffSet();
		Usart1Send();		
	}
	else 
	{
		FirstReadFlag = 0;
		Net_Sel = 0;
		TIM_Cmd(TIM2,DISABLE);
	}
  TIM2->CNT = 0;
}

