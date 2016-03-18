
#include "net_spi.h"
#include <STC_12.h>
//typedef unsigned char u8;
//typedef unsigned int u16;

sbit SON = P1^6;// MISO
sbit SIN = P1^5 ;// MOSI
sbit SCKN = P1^7 ; // SCK
sbit CSN = P5^1	;// 28J60-- CS
void	SPI1_Init(void)
{
		CSN = 1;
		SCKN = 0;
}
void CS_L(void){CSN = 0;}
void CS_H(void){CSN = 1;}

unsigned char	SPI1_ReadWrite(unsigned char writedat)
{
	char i,ret = 0;
	for(i = 0;i<8;i++)
	{
		
		SIN = writedat&0x80;
		SCKN = 1;
		ret<<=1;
		ret|=SON;
		writedat = writedat<<1;
		SCKN = 0;
	}
	return ret;
}