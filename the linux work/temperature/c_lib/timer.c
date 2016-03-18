#include <STC_12.h>
unsigned long seconds = 2;
void T0_Config(void)//定时1ms 方式0
{

	
	
	//       |定时器1相关位|定时器0相关位|
	//  bit:   7   6   5 4    3    2  1 0	
	// TMOD (GATE C/T M1 M0 GATE C/T M1 M0)
	// 设置定时器方式0（M1M0 = 00；GATE = 0；）
	// 方式0： 13位定时器,  C/T = 0; 计数时钟源为内部晶振12分频,24M晶振下，每个时钟周期1/(24M/12)=0.5us
	TMOD = 0x00;
	
	
	TH0 = (0x1fff-2000)>>5;//2的13次方-2000；   0.5us*2000 = 1ms 每个时钟周期加1，加2000次后寄存器加满溢出
	
	//相与之后0x1fff-1000 的低8位中的高三位被清0
	TL0 = (0x1fff-2000)&0x1f;
	
	
	TR0 = 1;//启动定时器0（13位计数器开始自加1）
	EA = 1;  //开中断总开关
	ET0 = 1; //开定时器中断开关
	
}
void INT_Process(void) interrupt 1
{
	static int count = 0;	
	//重装初值
	TH0 = (0x1fff-2000)>>5;//2的13次方-2000；
	TL0 = (0x1fff-2000)&0x1f;
	TR0 = 1;//启动定时器0（13位计数器开始自加1）
	count++;
	
	//如果是第1000次进中断，每定时1ms进一次中断，即1s时间已到
	if(count==1000)
	{
		seconds++;
		count = 0;
	}
}