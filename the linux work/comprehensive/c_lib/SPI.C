#include "spi.h"

static u8 delay_count = 0x04;
void Delay_SD(u8 tp)
{
	while(tp)
	{
		tp--;
	}
}
void Spi_IOInit()
{
	
} 
void Spi_Init(void)
{
	delay_count = INIT_SPEED;
	Spi_IOInit();
	SD_CLK_L;
}
u8 SPIx_ReadWrite7bit(u8 tp)
{
	u8 i = 0;
	for(;i<7;i++)
	{
		if(tp&0x80)
		{
			SD_OUT_H;
		}
		else
		{
			SD_OUT_L;
		}
		Delay_SD(delay_count);
		SD_CLK_H;
		Delay_SD(delay_count);
		SD_CLK_L;
		Delay_SD(delay_count);
		tp = tp<<1;
		if(SD_IN)
		{
			tp++;	
		}	
	}
	return tp;
}
u8 SPIx_ReadWriteByte(u8 tp)
{
	u8 i = 0;
	for(;i<8;i++)
	{
		if(tp&0x80)
		{
			SD_OUT_H;
		}
		else
		{
			SD_OUT_L;
		}
		Delay_SD(delay_count);
		SD_CLK_H;
		Delay_SD(delay_count);
		SD_CLK_L;
		Delay_SD(delay_count);
		tp = tp<<1;
		if(SD_IN)
		{
			tp++;	
		}		
	}
	return tp;
}
void SPIx_SetSpeed(u8 SpeedSet)
{
	switch(SpeedSet)
	{
		case SPI_SPEED_2://二分频
			delay_count = 0;
			break;
		case SPI_SPEED_4://四分频
			delay_count = 1;//Fsck=Fpclk/4=18Mhz
			break;
		case SPI_SPEED_8://八分频
			delay_count = 2;//Fsck=Fpclk/8=9Mhz
			break;
		case SPI_SPEED_16://十六分频
			delay_count = 4;//Fsck=Fpclk/16=4.5Mhz
			break;
		case SPI_SPEED_256://256分频
			delay_count = 16;//Fsck=Fpclk/16=281.25Khz
			break;
	}		 	  
} 
