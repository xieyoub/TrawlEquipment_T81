#include "SogTransport.h"

u32 SogData[3]={0};
u32 CogData[3]={0};

//���ٺ���ȡ��Ч����
void SOG_COG_Judge()
{
	if(NetState[0]==1 && SogData[0]!=0)//��������,���Ϻ�����Ч
	{
			SOG = SogData[0];
			COG = CogData[0];
			return ;
	} 
	
	if(NetState[1]==1 && SogData[1]!=0)//��β����,��β������Ч
	{
		SOG = SogData[1];
		COG = CogData[1];
		return ;
	}
	
	if(NetState[2]==1 && SogData[2]!=0)//��������,���Ϻ�����Ч
	{
		SOG = SogData[2];
		COG = CogData[2];
		return ;
	}
	 
	//����ʾλ�궼����Ч��Ϣ
	SOG = 0;
	COG = 0;
}

//GPS��Ч�ж�
void GPS_Judge()
{
	if(NetState[0]==1 && GPS.longitude[0]!=0) //��������
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

	//��������Ч
	EW = 0;
	longitude = 0;
	NS = 0;
	latitude = 0;
	UTCTime = 0;
	UTCDate = 0;
}