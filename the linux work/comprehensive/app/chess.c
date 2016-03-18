

/*五子棋应用*/

#include <STC_12.h>
#include "tft_9325.h"
#include "touch.h"
#include <stdio.h>
#include "app.h"
#include <gui.h>
uchar ifput[11][11];
char str[20];

void DrawPlate();							  //绘制棋盘
uchar ifSomeOneWin(uchar x,uchar y);		  //判断输赢


/*五子棋入口函数*/
void Chess(void)
{
	unsigned char user;
	bit flag=0;
	unsigned int x,y;
	unsigned int point[2];
	TFT_Init();
	ClearScreen(Green);						//背景为绿色
	DrawPlate();						    //绘制棋盘
	DrawClose();						    //绘制中途退出按钮
	while(1)
	{
		T_GetPoint(point);					//获取按下屏幕的坐标
		if(JugeClose(point)==1)
			return;
		if(point[0]<=50||point[0]>=270||point[1]<=10||point[1]>=230)		  //如果点击棋盘以为的地方，没有任何反应
			continue;
		x=(point[0]-50)/22;			  //获取点击的位置位于棋盘的哪个地方
		y=(point[1]-10)/22;

		if(flag==0&&ifput[y+1][x+1]==0)
		{
			ifput[y+1][x+1]=1;
			GUI_Circle(x*22+50,y*22+10,11,Black,1);		  //根据点击的位置在相应的地方绘制棋子
			flag=~flag;
		}
		if(flag==1&&ifput[y+1][x+1]==0)
		{
			ifput[y+1][x+1]=2;
			GUI_Circle(x*22+50,y*22+10,11,White,1);
			flag=~flag;
		}
		user=ifSomeOneWin(y+1,x+1);
		/*如果有某位用户赢了，就停止游戏*/
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


/*判断胜利与否函数*/
uchar ifSomeOneWin(uchar x,uchar y)			
{
	uchar cor=ifput[x][y];
	uchar count=0;
	uchar i=x,j=y;
	/*判断下降形式的斜线上是否连成五颗相同的棋子*/
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
	/*判断直线上是否连成五颗相同的棋子*/
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
	/*判断上升形式的斜线上是否连成五颗相同的棋子*/
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

/*	绘制棋盘函数*/

void DrawPlate()
{
	uchar x=50,y=10;
	uchar i,j;
	for(i=0;i<11;i++)		  //初始化棋盘，给每个位置都置0
	{
		for(j=0;j<11;j++)
		{
			ifput[i][j]=0;
		}
	}
	for(i=1;i<11;i++)		   //绘制棋盘的格子
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