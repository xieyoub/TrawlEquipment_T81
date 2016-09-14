#include "SogTransport.h"

u32 SogData[3]={0};
u32 CogData[3]={0};

//三个网位仪则取中值，两个网位仪取小的
void mid_value()
{
	//SOG
if(NetState[0]==1&&NetState[1]==1&&NetState[2]==1) //三个全部都OK
	{
		if(SogData[0]==SogData[1] && SogData[1]==SogData[2])
		{
			SOG = SogData[0];
		}

		if(SogData[0]==SogData[1] && SogData[0]>SogData[2])
		{
			SOG=SogData[2];
		}
		else 
		{
			SOG = SogData[0];
		}

		if(SogData[0]==SogData[2] && SogData[0]>SogData[1])
		{
			SOG = SogData[1];
		}
		else
		{
			SOG = SogData[0];
		}

		if(SogData[1]==SogData[2] && SogData[2]>SogData[0])
		{
			SOG = SogData[0];
		}
		else
		{
			SOG = SogData[2];
		}

		if(SogData[0]!=SogData[1] && SogData[0]!=SogData[2])
		{
			if(SogData[0]>SogData[1])
			{
				if(SogData[0]>SogData[2])
				{
					if(SogData[1]>SogData[2])
					{
						SOG = SogData[1];
					}
					else
					{
						SOG = SogData[2];
					}
				}
				else
				{
					SOG = SogData[0];
				}
			}
			else if(SogData[1]>SogData[2])
			{
				if(SogData[0]>SogData[2])
				{
					SOG = SogData[0];
				}
				else 
				{
					SOG = SogData[2];
				}
			}
			else
			{
				SOG = SogData[1];
			}
		}
	}
	else //只有两插入或一个
	{
		if(NetState[0]!=1)
		{
			if(NetState[1]==1&&NetState[2]==1)
			{
				if(SogData[1]>SogData[2])
				{
					SOG = SogData[2];
				}
				else
				{
					SOG = SogData[1];
				}
			}
			else
			{
				if(NetState[1]==1)
				{
					SOG=SogData[1];
				}
				else
				{
					SOG = SogData[2];
				}
			}
		}
		else if(NetState[1]!=1)
		{
			if(NetState[2]==1)
			{
				if(SogData[0]>SogData[2])
				{
					SOG = SogData[2];
				}
				else
				{
					SOG = SogData[0];
				}
			}
			else
			{
				SOG = SogData[0];
			}
		}
		else if(NetState[2]!=1)
		{
			if(SogData[0]>SogData[1])
			{
				SOG = SogData[1];
			}
			else
			{
				SOG = SogData[1];
			}
		}
	}
	
	//COG
if(NetState[0]==1&&NetState[1]==1&&NetState[2]==1) //三个全部都OK
	{
		if(CogData[0]==CogData[1] && CogData[1]==CogData[2])
		{
			COG = CogData[0];
		}

		if(CogData[0]==CogData[1] && CogData[0]>CogData[2])
		{
			COG=CogData[2];
		}
		else 
		{
			COG = CogData[0];
		}

		if(CogData[0]==CogData[2] && CogData[0]>CogData[1])
		{
			COG = CogData[1];
		}
		else
		{
			COG = CogData[0];
		}
		
		if(CogData[1]==CogData[2] && CogData[2]>CogData[0])
		{
			COG = CogData[0];
		}
		else 
		{
			COG = CogData[2];
		}

		if(CogData[0]!=CogData[1] && CogData[0]!=CogData[2])
		{
			if(CogData[0]>CogData[1])
			{
				if(CogData[0]>CogData[2])
				{
					if(CogData[1]>CogData[2])
						COG = CogData[1];
					else
						COG = CogData[2];
				}
				else
				{
					COG = CogData[0];
				}
			}
			else if(CogData[1]>CogData[2])
			{
				if(CogData[0]>CogData[2])
					COG = CogData[0];
				else 
					COG = CogData[2];
			}
			else
			{
				COG = CogData[1];
			}
		}
	}
	else //只有两插入或一个
	{
		if(NetState[0]!=1)
		{
			if(NetState[1]==1&&NetState[2]==1)
			{
				if(CogData[1]>CogData[2])
					COG = CogData[2];
				else
					COG = CogData[1];
			}
			else
			{
				if(NetState[1]==1)
					COG=CogData[1];
				else
					COG = CogData[2];
			}
		}
		else if(NetState[1]!=1)
		{
			if(NetState[2]==1)
			{
				if(CogData[0]>CogData[2])
					COG = CogData[2];
				else
					COG = CogData[0];
			}
			else
				COG = CogData[0];
		}
		else if(NetState[2]!=1)
		{
			if(CogData[0]>CogData[1])
				COG = CogData[1];
			else
				COG = CogData[1];
		}
	}
}
