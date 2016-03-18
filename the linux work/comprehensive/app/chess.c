

/*������Ӧ��*/

#include <STC_12.h>
#include "tft_9325.h"
#include "touch.h"
#include <stdio.h>
#include "app.h"
#include <gui.h>
uchar ifput[11][11];
char str[20];

void DrawPlate();							  //��������
uchar ifSomeOneWin(uchar x,uchar y);		  //�ж���Ӯ


/*��������ں���*/
void Chess(void)
{
	unsigned char user;
	bit flag=0;
	unsigned int x,y;
	unsigned int point[2];
	TFT_Init();
	ClearScreen(Green);						//����Ϊ��ɫ
	DrawPlate();						    //��������
	DrawClose();						    //������;�˳���ť
	while(1)
	{
		T_GetPoint(point);					//��ȡ������Ļ������
		if(JugeClose(point)==1)
			return;
		if(point[0]<=50||point[0]>=270||point[1]<=10||point[1]>=230)		  //������������Ϊ�ĵط���û���κη�Ӧ
			continue;
		x=(point[0]-50)/22;			  //��ȡ�����λ��λ�����̵��ĸ��ط�
		y=(point[1]-10)/22;

		if(flag==0&&ifput[y+1][x+1]==0)
		{
			ifput[y+1][x+1]=1;
			GUI_Circle(x*22+50,y*22+10,11,Black,1);		  //���ݵ����λ������Ӧ�ĵط���������
			flag=~flag;
		}
		if(flag==1&&ifput[y+1][x+1]==0)
		{
			ifput[y+1][x+1]=2;
			GUI_Circle(x*22+50,y*22+10,11,White,1);
			flag=~flag;
		}
		user=ifSomeOneWin(y+1,x+1);
		/*�����ĳλ�û�Ӯ�ˣ���ֹͣ��Ϸ*/
		if(user==1)
		{
			sprintf(str,"Black WIN!!");
			TFT_PutStr(110,100,str,Black,Red);
			if(JugeClose(point)==1)
				return;
		}
		if(user==2)
		{
			sprintf(str,"White  WIN!!");
			TFT_PutStr(110,100,str,Black,Red);
			if(JugeClose(point)==1)
				return;
		}

	}
}


/*�ж�ʤ�������*/
uchar ifSomeOneWin(uchar x,uchar y)			
{
	uchar cor=ifput[x][y];
	uchar count=0;
	uchar i=x,j=y;
	/*�ж��½���ʽ��б�����Ƿ����������ͬ������*/
	while (ifput[i][j] == cor)
	{
		count++;
		i -= 1;
		j -= 1;
	}
	i = x + 1, j = y + 1;
	while (ifput[i][j] == cor)
	{
		count++;
		i += 1;
		j += 1;
	}
	if (count >= 5)
		return cor;
	/*�ж�ֱ�����Ƿ����������ͬ������*/
	i = x, j = y;
	count = 0;
	while (ifput[i][j] == cor)
	{
		count++;
		i -= 1;
	}
	i = x + 1, j = y;
	while (ifput[i][j] == cor)
	{
		count++;
		i += 1;
	}
	if (count >= 5)
		return cor;
	i = x, j = y;
	count = 0;
	while (ifput[i][j] == cor)
	{
		count++;
		j -= 1;
	}
	i = x, j = y + 1;
	while (ifput[i][j] == cor)
	{
		count++;
		j += 1;
	}
	if (count >= 5)
		return cor;
	/*�ж�������ʽ��б�����Ƿ����������ͬ������*/
	i = x, j = y;
	count = 0;
	while (ifput[i][j] == cor)
	{
		count++;
		i+=1;
		j -= 1;
	}
	i = x-1, j = y + 1;
	while (ifput[i][j] == cor)
	{
		count++;
		i-=1;
		j += 1;
	}
	if (count >= 5)
		return cor;
	return 0;
}	 

/*	�������̺���*/

void DrawPlate()
{
	uchar x=50,y=10;
	uchar i,j;
	for(i=0;i<11;i++)		  //��ʼ�����̣���ÿ��λ�ö���0
	{
		for(j=0;j<11;j++)
		{
			ifput[i][j]=0;
		}
	}
	for(i=1;i<11;i++)		   //�������̵ĸ���
	{
		for(j=1;j<11;j++)
		{
			GUI_Rectangle(x,y,x+22,y+22,Black,Black);
			x+=22;
		}
		x=50;
		y+=22;
	}
}