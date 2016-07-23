#include "main.h"

u8 Usart1buf[UART_LEN];  //����1����
u8 Usart2buf[UART_LEN];  //����2����
u8 FirstReadFlag = 1;   //1��������һ�ζ�ȡ 
u8 NetState[4]={0};     //��λ�ǲ����״̬��1�����룬0�γ�
u8 Net_Sel = 0;         //��ǰ���ڲ�������λ�ǣ�1�����ϣ�2��β��3����
u8 ReadTime = 0;        //��ȡ����
u8 ConnectDevices=0;       //���ӵ��ն��豸 ��1��T88��2��T90
u8 FirstWrite = 0;          //����T800��һ��ע��
u32 MMSI[3] = {0};      //���ϣ�MMSI[0] ��β��MMSI[1] ����MMSI[2]

//t90
u16 LeftRightOffset;
u16 TailOffset;
u8 location;

int main(void)
{	
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Usart1_Init();//��T800ͨ��
	Usart2_Init();//��T88ͨ��
	time2_init();
	Switch_Init(); 
	T800Dectect(); // ���T800IO����
	ReadInit();  //��ȡ
	while(1)
	{
		
	}
}

