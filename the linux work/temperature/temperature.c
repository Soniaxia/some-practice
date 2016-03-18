#include <stdio.h>
#include <STC_12.h>
#include <tft_9325.h>
#include <tmp75.h>

unsigned char str[20];
float temp;

void main()
{
	TFT_Init();
	ClearScreen(White);
	TMP75_Init();
	while(1)
	{
		temp=TMP75_Read()*0.0625;
		sprintf(str,"The temprature is :%.2f",temp);
		TFT_PutStr(70,110,str,Black,White);

	}
}

