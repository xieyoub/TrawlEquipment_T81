#include "Usart485.h"

void Usart485_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	
//	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // T88 Control
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

//	
	Usart2_Receive;
}
