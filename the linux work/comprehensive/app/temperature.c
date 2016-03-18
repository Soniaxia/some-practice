
/*	�¶ȼ�س���ʵ��*/
#include <tft_9325.h>
#include <tmp75.h>
#include <stdio.h>
#include <string.h>
#include <STC_12.h>
#include <gui.h>

/*	�¶ȼ�س������*/

void Temp75()
{
	char str[20];
	float temperature;
	unsigned int point[2];
	TFT_Init();
	TMP75_Init();
	ClearScreen(White);
	DrawClose();			//�����˳���ť
	while(1)
	{		
		T_GetPoint(point);
		if(JugeClose(point)==1)
		{
			P4=0XFF;
			return;
			}
		/*����оƬ�ֲ��֪��TMP75�¶ȼĴ���Ϊ12λ��ֻ���Ĵ�����
		*��Ҫ����ת�������ֲ��ת�����֪��Ҫ���õ����¶ȳ�0.0625*/
		temperature=TMP75_Read()*0.0625;	  
		if(temperature>30)    /*����¶ȳ�����30�ȣ���ô�ĸ�LED��������ʾ�򾯸�*/
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
