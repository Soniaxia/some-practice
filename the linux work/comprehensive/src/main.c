#include <app.h>
#include <tft_9325.h>
#include <gui.h>
#include <rda5807.h>
void main()							//�������
{	
	unsigned char flag;
	while(1)
	{
		show_menu();				//��ʾͼ��
		flag=menu_select();			//ѡ��ͼ��
		if(flag==1)			  		//����ѡ���ͼ����빦��
		{
			Temp75();
		}
		if(flag==2)
		{
			Radio();
		}
	}
}