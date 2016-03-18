#include<STC_12.h>
#include <tft_9325.h>
#include <stdio.h>

unsigned char str[20];
unsigned int num=0;

void Delay()
{
	unsigned int i,j;
	for(i=0;i<1000;i++)
		for(j=0;j<400;j++);

}
void IT1_Init()
{
	EA=1;
	EX1=1;
	IT1=0;
}							 

void main()
{
	IT1_Init();
	TFT_Init();
	ClearScreen(White);
	while(1)
	{
		sprintf(str,"The number is %d",num);
		TFT_PutStr(100,100,str,Black,White);
	}
}

void AddNumber() interrupt 2
{
	EA=0;
	num++;
	Delay();
	EA=1;
}
