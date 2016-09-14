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
#include "SogTransport.h"


void NVIC_Configuration(void);

#define UART_LEN     32 
extern u8 Usart1buf[UART_LEN];  //串口1缓存
extern u8 Usart2buf[UART_LEN];  //串口2缓存
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
extern u32 SOG,COG,longitude,latitude,NS,EW,UTCTime,UTCDate;
extern u32 sog_sample_interval;//航速采用间隔
extern u32 cog_sample_len;     //航向平均化点数
extern u32 faultCount[3];
extern u8 Reply_flag;


typedef struct{
	long  latitude[3];  //经度
	long  longitude[3]; //纬度
	u8 	NS[3];
	u8 	EW[3];
	long UTCTime[3];
	long UTCDate[3];
}GPS_INFO;

extern GPS_INFO   GPS;
#endif
