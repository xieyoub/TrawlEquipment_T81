#ifndef _SOGTRANSPORT_H
#define _SOGTRANSPORT_H

#include "main.h"

extern u32 SogData[3];
extern u32 CogData[3];
void GPSInfo_Judge(void);
u8 leftNetJude(void);
u8 RightNetJude(void);
u8 TailNetJude(void);
u32 midValue(u32 value1,u32 value2,u32 value3);
u32 CompareValue(u32 value1,u32 value2);
void GPSInfoPack(u8 i);
#endif
