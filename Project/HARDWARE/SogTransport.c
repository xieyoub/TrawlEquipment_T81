#include "SogTransport.h"

u32 SogData[3]={0};
u32 CogData[3]={0};

//航速航向取有效数据
void SOG_COG_Judge()
{
	if(NetState[0]==1 && SogData[0]!=0)//左舷正常,左舷航速有效
	{
			SOG = SogData[0];
			COG = CogData[0];
			return ;
	} 
	
	if(NetState[1]==1 && SogData[1]!=0)//网尾正常,网尾航速有效
	{
		SOG = SogData[1];
		COG = CogData[1];
		return ;
	}
	
	if(NetState[2]==1 && SogData[2]!=0)//右舷正常,右舷航速有效
	{
		SOG = SogData[2];
		COG = CogData[2];
		return ;
	}
	 
	//三个示位标都无有效信息
	SOG = 0;
	COG = 0;
}

//GPS有效判断
void GPS_Judge()
{
	if(NetState[0]==1 && GPS.longitude[0]!=0) //左舷正常
	{

		EW = GPS.EW[0];
		longitude = GPS.longitude[0];
		NS = GPS.NS[0];
		latitude = GPS.latitude[0];
		UTCTime = GPS.UTCTime[0];
		UTCDate = GPS.UTCDate[0];
		return ;
	}
	
	if(NetState[1]==1 && GPS.longitude[1]!=0)
	{
		EW = GPS.EW[1];
		longitude = GPS.longitude[1];
		NS = GPS.NS[1];
		latitude = GPS.latitude[1];
		UTCTime = GPS.UTCTime[1];
		UTCDate = GPS.UTCDate[1];
		return ;
	}
	
	if(NetState[2]==1 && GPS.longitude[2]!=0)
	{
		EW = GPS.EW[2];
		longitude = GPS.longitude[2];
		NS = GPS.NS[2];
		latitude = GPS.latitude[2];
		UTCTime = GPS.UTCTime[2];
		UTCDate = GPS.UTCDate[2];
		return ;
	}

	//三个都无效
	EW = 0;
	longitude = 0;
	NS = 0;
	latitude = 0;
	UTCTime = 0;
	UTCDate = 0;
}