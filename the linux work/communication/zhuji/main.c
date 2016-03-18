#include<stc_12.h>
#include<stdio.h>
#include".\include\TFT_9325.h"
#include".\include\tmp75.h"


void delay()
{
	int i;
	i=10000;
	while(i--);
}

void SPI_Init()
{								  
	SPCTL=0xD3;		//1101 0011 MSTR确定主从机，SPI使能，数据字的最高位先发送，确定为主机，时钟频率选择为128
	SPDAT=0;
	SPSTAT=0xC0;		 //通过软件置“1”清“0”
}
unsigned char SPI_Swap(u8 dat)
{	
		SPDAT=dat;
		while((SPSTAT&0xC0==0));
		SPSTAT=0xC0;
		return SPDAT;
}

void main ()
{	u8 str[25];
	u16 temp,rtemp;
	u8 high,low,rhigh=0,rlow=0;
	TMP75_Init();
	TFT_Init();
	SPI_Init();
	ClearScreen(White);

	while(1)
	{
		TFT_PutStr(30,70,"This is master:",Black,White);
		temp=TMP75_Read();
		sprintf(str,"The temperature is %.2f",temp*0.0625);
		TFT_PutStr(60,100,str,Black,White);
		
		
		high=temp>>8;		
		low=temp;
		
	
		rlow=SPI_Swap(high);
		delay();

		rhigh=SPI_Swap(low);
		delay();
		
		rtemp=rlow+rhigh*256;
		sprintf(str,"Recv temperature is %.2f",rtemp*0.0625);
		TFT_PutStr(60,120,str,Black,White);	 
		TFT_PutStr(260,120,"   ",White,White);
		
	}
}
