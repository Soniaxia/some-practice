/*
*		gui.h	
*	图形用户接口控件

*/
#ifndef GUI_H
#define GUI_H
#include "tft_9325.h"
#include "touch.h"
#include <stdio.h>
#include "app.h"
typedef struct button
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int high;
}button;
extern void DrawClose();
extern char JugeClose(unsigned int point[2]);
void DrawMin();
char JugeMin(unsigned int point[2]);
extern void show_menu();
extern unsigned char menu_select();
extern void create_button(unsigned int x,unsigned int y,const char *str,int len);
extern void show_aod();
#endif