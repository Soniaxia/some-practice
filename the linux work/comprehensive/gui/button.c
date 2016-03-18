/*各种按钮功能实现*/
#include <gui.h>
/*关闭按钮的绘制*/
void DrawClose()
{
	GUI_Rectangle(285,10,305,30,Yellow,1);
	GUI_Rectangle(285,10,305,30,Black,0);
	GUI_Line(285,10,304,29,Black);
	GUI_Line(305,10,285,30,Black);
}	
/*关闭按钮的功能,检测是否按下，如果按下就退出并回到首页*/
char JugeClose(unsigned int point[2])
{
	if(point[0]>285&&point[0]<305&&point[1]>10&&point[1]<30)
	{
		GUI_Rectangle(285,10,305,30,Red,0);		/*按下时按钮边框会变红*/
		return 1;
	}
	return 0;
}


/*		绘制通用按钮  */
void create_button(unsigned int x,unsigned int y,const char *str,int len)
{
	button btn;
	btn.x=x;
	btn.y=y;
	btn.width=len*8;
	btn.high=10;
  TFT_PutStr(x,y,str,Black,White);
}