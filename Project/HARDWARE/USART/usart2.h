#ifndef _USART2_H
#define _USART2_H
#include "main.h"

void Usart2_Init(void);
void USART2_DMA_Configuration(void);
void USART2_NVIC_Configuration(void);
void USART2_GPIO_Configuration(void);
void USART2_MODE_Configuration(void);
void Com2GetData(void);
void Com2SendData(void);
void Usart1Send(void);
void OpenSerial(void);
void ReadOffSet(void);
void CloseSerial(void);
void replyAsk(void);
void WriteOffset(void);
void ReadSog_Cog(void);
void NetStatejudge(void);
void GPSInfoSend(void);
void GPS_invalid(void);

#endif
