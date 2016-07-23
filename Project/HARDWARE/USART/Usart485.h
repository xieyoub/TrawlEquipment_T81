#ifndef _USARTCONTROL_H
#define _USARTCONTROL_H
#include "main.h"

#define Usart2_Receive GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define Usart2_Output  GPIO_SetBits(GPIOA,GPIO_Pin_1)

//#define Num1_Receive GPIO_ResetBits(GPIOA,GPIO_Pin_8)
//#define Num1_Output  GPIO_SetBits(GPIOA,GPIO_Pin_8)

//#define Num2_Receive GPIO_ResetBits(GPIOB,GPIO_Pin_15)
//#define Num2_Output  GPIO_SetBits(GPIOB,GPIO_Pin_15)

//#define Num3_Receive GPIO_ResetBits(GPIOB,GPIO_Pin_14)
//#define Num3_Output  GPIO_SetBits(GPIOB,GPIO_Pin_14)

void Usart485_Init(void);


#endif
