#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"
#include "sys.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "4052.h"
#include "Usart485.h"
#include "dectection.h"
#include "timer.h"

void NVIC_Configuration(void);

#define UART_LEN     21 
extern u8 Usart1buf[UART_LEN];  //����1����
extern u8 Usart2buf[UART_LEN];  //����2����
extern u8 FirstReadFlag;
extern u8 NetState[4];
extern u8 Net_Sel;
extern u8 ReadTime;
extern u8 ConnectDevices;

extern u16 LeftRightOffset;
extern u16 TailOffset;
extern u8 location;
extern u8 FirstWrite ;
extern u32 MMSI[3];
extern u32 SOG,COG;      
extern u32 sog_sample_interval;//���ٲ��ü��
extern u32 cog_sample_len;     //����ƽ��������

#endif
