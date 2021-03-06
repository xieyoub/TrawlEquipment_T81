#include "main.h"

u8 Usart1buf[UART_LEN];  //串口1缓存
u8 Usart2buf[UART_LEN];  //串口2缓存
u8 FirstReadFlag = 1;   //1：开机第一次读取 
u8 NetState[4]={0};     //网位仪插入的状态，1：插入，0拔出 ,0:左舷，1：网尾，2：右舷
u8 Net_Sel = 0;         //当前正在操作的网位仪，1：左舷，2网尾，3右舷
u8 ReadTime = 0;        //读取次数
u8 ConnectDevices=0;       //连接的终端设备 ，1：T88，2：T90
u8 FirstWrite = 0;          //插入T800第一次注入
u32 MMSI[3] = {0};      //左舷：MMSI[0] 网尾：MMSI[1] 右舷MMSI[2]
u32 SOG=0,COG=0,longitude=0,latitude=0,NS=0,EW=0,UTCTime=0,UTCDate=0;      //航速，航向,经度，纬度,时间，日期
u32 sog_sample_interval=0;//航速采用间隔
u32 cog_sample_len=0;     //航向平均化点数
u32 faultCount[3];        //串口未接受到应答的次数
GPS_INFO GPS; //gps信息结构体
u8 Reply_flag=0;  

//t90
u16 LeftRightOffset;
u16 TailOffset;
u8 location;

int main(void)
{	
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Usart1_Init();//与T800通信
	Usart2_Init();//与T88通信
	//Usart3_Init(9600);
	time2_init();
	Switch_Init(); 
	T800Dectect(); // 检测T800IO配置
	ReadInit();  //读取
	while(1)
	{
		
	}
}

