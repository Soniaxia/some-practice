#include "rda5807.h"
#include "iic.h"
unsigned char  RDA5807_Set_Freq(unsigned int freq)
{
	if((freq>870)&&(freq<1080))
	{
		IIC_start();
		IIC_wr_B(0x20);
		IIC_wr_B(H02_H);
		IIC_wr_B(H02_L);
		IIC_wr_B(freq-870);
		IIC_wr_B(H03_L);
		IIC_wr_B(0x00);
		IIC_wr_B(0x40);
		IIC_wr_B(0x90);
		IIC_wr_B(0x88);
		IIC_stop();
		return 1;
		
	}
	else return 0;
}
void RDA5807_Init(unsigned short ini_freq)
{
	IIC_init();
	IIC_start();
	IIC_wr_B(0x20);
	IIC_wr_B(0);
	IIC_wr_B(0x02);
	RDA5807_Set_Freq(ini_freq);
}
