#include <STC_12.h>
#include "tft_9325.h"
#include <stdio.h>
#include "rda5807.h"
#include "touch.h"

void Delay()
{
	unsigned int i,j;
	for(i=0;i<500;i++)
		for(j=0;j<200;j++);
}

void main(void)
{
	unsigned int freq=914;
	unsigned int point[2];
	char str[20];
	TFT_Init();
	ClearScreen(White);
	GUI_Square(260,50,30,Black,0);
	GUI_Line(263,65,287,65,Black);
	GUI_Line(275,53,275,77,Black);

	GUI_Square(260,100,30,Black,0);
	GUI_Line(263,115,287,115,Black);

	RDA5807_Init(914);
	sprintf(str,"%2.1fMHz",freq/10.0);
	TFT_PutStr(130,120,str,Black,White);
	while(1)
	{		
		T_GetPoint(point);
		if(point[0]>260&&point[0]<290&&point[1]>50&&point[1]<80)
		{
			if(freq>870)
			freq=freq+1;
			Delay();
			RDA5807_Set_Freq(freq);
			sprintf(str,"%2.1fMHz",freq/10.0);
			TFT_PutStr(130,120,str,Black,White);
		}
		if(point[0]>260&&point[0]<290&&point[1]>100&&point[1]<130)
		{
			if(freq<1070)
			freq=freq-1;
			Delay();
			RDA5807_Set_Freq(freq);
			sprintf(str,"%2.1fMHz",freq/10.0);
			TFT_PutStr(130,120,str,Black,White);
			
		}
		

	/*	if(Judge_Button(b1,point))
		{break;}		*/
		
	}
	
}