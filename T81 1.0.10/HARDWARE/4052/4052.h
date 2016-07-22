#ifndef _4052_H
#define _4052_H
#include "main.h"

void Switch_Init(void);
void GPIO_4052Init(void);

#define Right_Connect  GPIO_SetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)
#define Tail_Connect  GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define Left_Connect  GPIO_ResetBits(GPIOA,GPIO_Pin_11);GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#endif
