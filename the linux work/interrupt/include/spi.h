#ifndef __spi_h
#define __spi_h

//��ֲʱֻ���ṩ����4��IO�ڲ����ĺ꣬�������䣬�޸�IO�ڵ�ƽ�����Ĳ���
//�������޸�sd.c�е�spi_IOInit()���������ú���Ӧ�ܽŵ��������ģʽ��SD_IN
//��Ӧ�Ǵ��������룬���Ǹ������룬��·�������������

/******************************��ֲ�޸���**************************************/
#include <reg52.h>
#define SD_IN P1&0x40

#define SD_CLK_H P1|=0x80
#define SD_CLK_L P1&=0x7f

#define SD_OUT_H P1|=0x20
#define SD_OUT_L P1&=0xdf



/********************************************************************************/ 
#ifndef u8
#define u8 unsigned char 
#endif
#ifndef u16
#define u16 unsigned int
#endif
#ifndef u32
#define u32 unsigned long
#endif

#define INIT_SPEED 0 

#define SPI_SPEED_2   0
#define SPI_SPEED_4   1
#define SPI_SPEED_8   2
#define SPI_SPEED_16  4
#define SPI_SPEED_256 8
void Delay_SD(u8 tp);
void Spi_IOInit();
void Spi_Init(void);
u8 SPIx_ReadWrite7bit(u8 tp);
void SPIx_SetSpeed(u8 SpeedSet);
u8 SPIx_ReadWriteByte(u8 tp);
#endif
