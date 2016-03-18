
/*	温度监控程序实现*/
#include <tft_9325.h>
#include <tmp75.h>
#include <stdio.h>
#include <string.h>
#include <STC_12.h>
#include <gui.h>

/*	温度监控程序入口*/

void Temp75()
{
	char str[20];
	float temperature;
	unsigned int point[2];
	TFT_Init();
	TMP75_Init();
	ClearScreen(White);
	DrawClose();			//绘制退出按钮
	while(1)
	{		
		T_GetPoint(point);
		if(JugeClose(point)==1)
		{
			P4=0XFF;
			return;
			}
		/*根据芯片手册可知，TMP75温度寄存器为12位的只读寄存器，
		*需要进行转换，由手册的转换表可知需要将得到的温度乘0.0625*/
		temperature=TMP75_Read()*0.0625;	  
		if(temperature>30)    /*如果温度超过了30度，那么四个LED灯亮起，提示或警告*/
		{
			P4=0X00;
		}
		if(temperature<=30)
		{
			P4=0XFF;
		}
		sprintf(str,"tempurature is:%.2f",temperature);
		TFT_PutStr(90,110,str,Black,White);
	}
}										 
