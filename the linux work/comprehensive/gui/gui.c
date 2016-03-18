/*
		gui.c
	用户接口定义

*/
#include <gui.h>
#include <STC_12.h>
#include <touch.h>
extern code unsigned char gImage_zuo[6664];
extern code unsigned char gImage_tmp[7176];
extern code unsigned char gImage_net[6400];
extern code unsigned char gImage_you[6664];

/*
		显示菜单

*/
void show_menu()
{
	TFT_Init();
	ClearScreen(Black);
	TFT_Show_Bmp_8b(50,80,80,80,gImage_tmp);

	TFT_Show_Bmp_8b(190,80,80,80,gImage_net);
}

void show_aod()
{
	TFT_Show_Bmp_8b(61,121,98,68,gImage_zuo);
	TFT_Show_Bmp_8b(161,121,98,68,gImage_you);
}
/*
	获取被按下的图标

*/
unsigned char menu_select()
{
	unsigned int point[2];
	while(1)
	{
		T_GetPoint(point);
		if(point[0]>50&&point[0]<130&&point[1]>80&&point[1]<160)
		{
			GUI_Rectangle(50,80,130,160,Red,0);
			return 1;
		}
		if(point[0]>190&&point[0]<270&&point[1]>80&&point[1]<160)
		{
			GUI_Rectangle(190,80,270,160,Red,0);
			return 2;
		}
	}
}