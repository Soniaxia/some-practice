#include <app.h>
#include <tft_9325.h>
#include <gui.h>
#include <rda5807.h>
void main()							//函数入口
{	
	unsigned char flag;
	while(1)
	{
		show_menu();				//显示图标
		flag=menu_select();			//选择图标
		if(flag==1)			  		//根据选择的图标进入功能
		{
			Temp75();
		}
		if(flag==2)
		{
			Radio();
		}
	}
}