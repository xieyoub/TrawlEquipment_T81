#include "SogTransport.h"
#include "stdio.h"

u32 SogData[3]={0};
u32 CogData[3]={0};

u8 validData[3]={0}; //有效T800数据示位标

//航速航向取有效数据
void GPSInfo_Judge()
{
	validData[0] = leftNetJude();
	validData[1] = TailNetJude();
	validData[2] = RightNetJude();
	//三个都无效
	if(validData[0]==0 && validData[1]==0 && validData[2]==0)
	{
		SOG = 0;
		COG = 0;
		EW = 0;
		longitude = 0;
		NS = 0;
		latitude = 0;
		UTCTime = 0;
		UTCDate = 0;
		return ;
	}
	
	if(validData[0]==1 && validData[1]==0 && validData[2]==0)//只有左舷有效
	{
		SOG = SogData[0];
		COG = CogData[0];
		GPSInfoPack(0);
		return;
	}
	
	if(validData[0]==0 && validData[1]==1 && validData[2]==0)//只有网尾有效
	{
		SOG = SogData[1];
		COG = CogData[1];
		GPSInfoPack(1);
		return ;
	}
	
	if(validData[0]==0 && validData[1]==0 && validData[2]==1)//只有右舷有效
	{
		SOG = SogData[2];
		COG = CogData[2];
		GPSInfoPack(2);
		return;
	}
	
	//三个都有效,航速航向 取中值
	if(validData[0]==1&&validData[1]==1&&validData[2]==1)
	{
		SOG = midValue(SogData[0],SogData[1],SogData[2]);
		COG = midValue(CogData[0],CogData[1],CogData[2]);
		GPSInfoPack(0);
	}
	else //三台中两台有效数据 取小
	{
		if(validData[0]==1 && validData[1]==1)
		{
			SOG = CompareValue(SogData[0],SogData[1]);
			COG = CompareValue(CogData[0],CogData[1]);
			GPSInfoPack(0);
		}
		else if(validData[0]==1 && validData[2]==1)
		{
			SOG = CompareValue(SogData[0],SogData[2]);
			COG = CompareValue(CogData[0],CogData[2]);
			GPSInfoPack(0);
		}
		else if(validData[1]==1 && validData[2]==1)
		{
			SOG = CompareValue(SogData[1],SogData[2]);
			COG = CompareValue(CogData[1],CogData[2]);
			GPSInfoPack(1);
		}
	}
}


//判断左舷是否右舷
u8 leftNetJude()
{
	if(NetState[0]==1 && GPS.longitude[0]!=0)//左舷正常,左舷数据有效
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//判断网尾是否有效
u8 TailNetJude()
{
	if(NetState[1]==1 && GPS.longitude[1]!=0)//网尾正常,网尾数据有效
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//判断有效是否有效
u8 RightNetJude()
{
	if(NetState[2]==1 && GPS.longitude[2]!=0)//右舷正常,右舷数据有效
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//三个值取中值
u32 midValue(u32 value1,u32 value2,u32 value3)
{
	if(value1 == value2)
	{
		if(value1 == value3)
			return value1; 		//  三个值相等
		else if(value1>value3)
			return value3; 		//有两个值相等，取最小
		else
			return value1;
	}
	else if(value1 == value3)
	{
		if(value1>value2)
			return value2;
	}
	else if(value2 == value3)
	{
		if(value1>value2)
			return value2;
		else
			return value1;
	}
	else //三个值都不相等
	{
		if(value1>value2)
		{
			if(value1>value3) //value1最大
			{
				if(value2>value3)
					return value2;
				else
					return value3;
			}
			else
				return value1;
		}
		else if(value2>value3)
		{
			return value3;
		}
		else
		{
			return value2;
		}
	}
}

//两个取小
u32 CompareValue(u32 value1,u32 value2)
{
	if(value1>=value2)
		return value2;
	else
		return value1;
}

void GPSInfoPack(u8 i)
{
		EW = GPS.EW[i];
		longitude = GPS.longitude[i];
		NS = GPS.NS[i];
		latitude = GPS.latitude[i];
		UTCTime = GPS.UTCTime[i];
		UTCDate = GPS.UTCDate[i];
}