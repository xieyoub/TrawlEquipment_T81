#include "main.h"

u8 Usart1buf[UART_LEN];  //串口1缓存
u8 Usart2buf[UART_LEN];  //串口2缓存
u8 FirstReadFlag = 1;   //1：开机第一次读取 
u8 NetState[4]={0};     //网位仪插入的状态，1：插入，0拔出
u8 Net_Sel = 0;         //当前正在操作的网位仪，1：左舷，2网尾，3右舷
u8 ReadTime = 0;        //读取次数
u8 ConnectDevices=0;       //连接的终端设备 ，1：T88，2：T90
u8 FirstWrite = 0;          //插入T800第一次注入
u32 MMSI[3] = {0};      //左舷：MMSI[0] 网尾：MMSI[1] 右舷MMSI[2]

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
	time2_init();
	Switch_Init(); 
	T800Dectect(); // 检测T800IO配置
	ReadInit();  //读取
	while(1)
	{
		
	}
}

