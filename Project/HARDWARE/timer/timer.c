#include "timer.h"

void time2_init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
	TIM_TimeBaseStructure.TIM_Period = 24999;   //1s
	TIM_TimeBaseStructure.TIM_Prescaler = 2879;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}

//��ȡ��ʱ�����ٴζ�ȡ���ܴ���������3��
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
		else //�ж�Ϊ����
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
			if(FirstReadFlag==0) //���ǵ�һ�ζ�ȡ
			{
				TIM_Cmd(TIM2,DISABLE);
			}
			else  //��һ�ζ�ȡ
			{
				Net_Sel++;
				if(Net_Sel<=3) //������ȡ��һ��
				{
					ReadNext();
				}
				else //������λ�Ƕ���ȡ��
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

//ʹ�ܶ�ʱ��2
void tim2Enable(void)
{
	ReadTime = 0;
	TIM2->CNT = 0;
	TIM_Cmd(TIM2,ENABLE);
}

void ReadNext(void)
{
	if(Net_Sel == 2 && Tail_Net) //�ڶ����ж��Ѳ���
	{
		Usart1buf[7] = Net_Sel;
		ReadOffSet();
		Usart1Send();
	}
	else if(Right_Net && Net_Sel == 2) //����������
	{
		Net_Sel++;
		Usart1buf[7] = Net_Sel;
		ReadOffSet();
		Usart1Send();		
	}
	else if(Net_Sel == 3 && Right_Net) //������
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

