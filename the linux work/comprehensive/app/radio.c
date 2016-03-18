#include <STC_12.h>
#include "tft_9325.h"
#include <stdio.h>
#include "rda5807.h"
#include "touch.h"
#include "app.h"
#include <gui.h>

void Delay()
{
	unsigned int i,j;
	for(i=0;i<500;i++)
		for(j=0;j<200;j++);
}

void Radio(void)
{
	unsigned int freq=914;
	unsigned int point[2];
	char str[20];
	TFT_Init();
	ClearScreen(White);
	DrawClose();
	show_aod();
	GUI_Rectangle(60,50,260,120,Black,0);			//���ƿ�ͼ
	GUI_Rectangle(61,51,259,119,Yellow,1);
	GUI_Rectangle(60,120,160,190,Black,0);
	GUI_Rectangle(160,120,260,190,Black,0);

	RDA5807_Init(914);
	sprintf(str,"%2.1fMHz",freq/10.0);			 //��ʼƵ������
	TFT_PutStr(135,85,str,Black,Yellow);
	while(1)
	{		
		T_GetPoint(point);
		/*�����������ť��Ƶ�ʼ�һ*/
		if(point[0]>60&&point[0]<160&&point[1]>120&&point[1]<190)
		{
			GUI_Rectangle(60,120,160,190,Red,0);
			if(freq>870)							
			freq=freq-1;
			Delay();
			RDA5807_Set_Freq(freq);
			sprintf(str,"%2.1fMHz",freq/10.0);
			TFT_PutStr(135,85,str,Black,Yellow);
			GUI_Rectangle(60,120,160,190,Black,0);
		}
		/*����������Ұ�ť��Ƶ�ʼ�һ*/
		if(point[0]>160&&point[0]<260&&point[1]>120&&point[1]<190)
		{
			GUI_Rectangle(160,120,260,190,Red,0);
			if(freq<1070)
			freq=freq+1;
			Delay();
			RDA5807_Set_Freq(freq);
			sprintf(str,"%2.1fMHz",freq/10.0);
			TFT_PutStr(135,85,str,Black,Yellow);
			GUI_Rectangle(160,120,260,190,Black,0);
			
		}
		

		T_GetPoint(point);					//��ȡ������Ļ������
		if(JugeClose(point)==1)		 //�Ƿ����˳���ť
			return;		
		
	}
	
}