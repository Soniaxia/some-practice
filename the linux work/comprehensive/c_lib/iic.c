/*
文件名：IIC.c
功能：IIC总线驱动
创建人：肖俊
说明：使用前请根据自己硬件更改管脚定义，需结合51库使用；
可用函数：
	IIC_init();
	IIC_start();
	IIC_wr_B();
	IIC_re_B();
	IIC_stop();
*/
#include <STC_12.h>
#include "iic.h"
sbit scl=P1^3;
sbit sda=P1^2;

static void delay(u8 tenus)
{
	u8 i,j;
	for(i=0;i<40;i++)
	for(j=0;j<tenus;j++);
}
void IIC_init(void)
{
	P1M0|=0x08;
	P1M1&=(~0x08);
	scl=1;
	delay(1);
	sda=1;
	delay(1);
}
void IIC_start(void)
{
	sda=1;
	delay(1);
	scl=1;
	delay(1);
	sda=0;
	delay(1);
	scl=0;
}
void IIC_answer(void)
{
	u8 i=0;
	scl=1;
	delay(1);
	while((sda==1));
	scl=0;
	delay(1);
}
void IIC_reanswer(void)
{
	sda=0;
	scl=1;
	delay(1);
	scl=0;
	delay(1);
}
void IIC_stop(void)
{
	sda=0;
	scl=1;
	delay(1);
	sda=1;
	delay(1);
	scl=0;
}

void IIC_wr_B(u8 data1)
{
	u8 i,temp;
	temp=data1;
	for(i=0;i<8;i++)
	{
		if((temp&0x80)==0x80)
		{
			sda=1;
			scl=1;
			delay(1);
			scl=0;
			sda=0;
			delay(1);
		}
		else
		{
			sda=0;
			scl=1;
			delay(1);
			scl=0;
			delay(1);
		}
		temp=temp<<1;
	}
   IIC_answer();
}
u8 IIC_re_B(void)
{
	u8 data1,i;
	sda=1;
	for(i=0;i<8;i++)
	{
		data1=data1<<1;
		scl=1;
		delay(1);
		if(sda==0)
		data1=~(1|~data1);
		
		else
		data1=1|data1;
	
		scl=0;

	}
	IIC_reanswer();
	return(data1);
}