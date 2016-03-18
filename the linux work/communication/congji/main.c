#include<stc_12.h>
#include<stdio.h>
#include".\include\TFT_9325.h"
#include".\include\tmp75.h"



unsigned char high,low;
unsigned char rhigh=0,rlow=0,spibuf,flag=0;
unsigned char count=0;
sbit SS=P1^4;
u16 temp,rtemp;

void SPI_Init()
{
	SPCTL=0xC3;
	SPSTAT=0xC0;
	EA=1;
	IE2=0x02;	//ÔËÐÐSPIÖÐ¶Ï
	SS=1;	  
}

void main ()
{
	u8 str[25];
	TMP75_Init();
	TFT_Init();
	SPI_Init();
	ClearScreen(White);
	while(1)
	{
		TFT_PutStr(30,70,"This is slave:",Black,White);
		temp=TMP75_Read();
		sprintf(str,"The temperature is %.2f",temp*0.0625);
		TFT_PutStr(60,100,str,Black,White);

		if(flag>=2)
		{
			flag=0;
			rtemp=rlow+rhigh*256;
			sprintf(str,"Recv temperature is %.2f",rtemp*0.0625);
			TFT_PutStr(60,120,str,Black,White);
		}
		low=temp;
		high=temp>>8;
		
	}
}


void spi()interrupt 9 
{
	flag++;
	spibuf=SPDAT;
	if(flag==1)
	{
		rhigh=spibuf;
		SPDAT=low;

	}
	if(flag==2)
	{
		rlow=spibuf;
		SPDAT=high;
	
	}
	SPSTAT=0xC0;
}
