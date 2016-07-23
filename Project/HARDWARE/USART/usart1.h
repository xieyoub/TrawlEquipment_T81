#ifndef _USART1_H
#define _USART1_H
#include "main.h"

void Usart1_Init(void);
void	USART1_DMA_Configuration(void);
void	USART1_NVIC_Configuration(void);
void	USART1_GPIO_Configuration(void);
void	USART1_MODE_Configuration(void);
void Com1GetData(void);
void Com1SendData(void);
u16 msg_crc(u8 *ptr,u8 num);
void ReadInit(void);
void FirstRead(void);

#endif
