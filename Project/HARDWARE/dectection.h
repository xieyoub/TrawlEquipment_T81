#ifndef _DECTECTION_H
#define _DECTECTION_H
#include "main.h"

#define Left_Net   !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)  //вСоомЬн╩рг╡ЕхКеп╤о
#define Right_Net  !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)  //сроо
#define Tail_Net   !GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)  //мЬн╡

void T800Dectect(void);
void Dectect_GPIO_Config(void);
void Dectect_NIVC_Config(void);
void Dectect_EXTI_Config(void);
#endif
