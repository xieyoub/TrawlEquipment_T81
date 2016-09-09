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
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 		
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

//T800����������
void T800Dectect()
{
 Dectect_GPIO_Config();
	Dectect_NIVC_Config();
	Dectect_EXTI_Config();
}


//���T800�������
void EXTI9_5_IRQHandler(void)
{
	//���ϼ��
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)         
	{
		delay_ms(1000); //�ȴ�T800�ȶ�
		if(Left_Net)//����
		{
			NetState[0] = 1;
//			Net_Sel = 1;
//			ReadOffSet();
//			Usart1Send();
//			tim2Enable();
		}
		else  //�γ�
		{
			MMSI[0] = 0;
			NetState[0] = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	
	 //��β���
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)     
	{
		delay_ms(1000); //�ȴ�T800�ȶ�
		if(Tail_Net)//����
		{
			NetState[1] = 1;
//			Net_Sel = 2;
//			ReadOffSet();
//			Usart1Send();
//			tim2Enable();
		}	
		else  //�γ�
		{
			MMSI[1] = 0;
			NetState[1] = 0;
		}
		
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	
	//���ϼ��
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)  
	{
		delay_ms(1000); //�ȴ�T800�ȶ�
		if(Right_Net) //����
		{
			NetState[2] = 1;
//			Net_Sel = 3;
//			ReadOffSet();
//			Usart1Send();
//			tim2Enable();
		}		
		else  //�γ�
		{
			MMSI[2] = 0;
			NetState[2] = 0;
		}
		
		//�����־λ
		EXTI_ClearITPendingBit(EXTI_Line5);
	}	
}

