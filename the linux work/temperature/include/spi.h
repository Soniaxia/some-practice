#ifndef __spi_h
#define __spi_h

//移植时只需提供以下4个IO口操作的宏，宏名不变，修改IO口电平操作的部分
//另外需修改sd.c中的spi_IOInit()函数，配置好相应管脚的输入输出模式，SD_IN
//脚应是带上拉输入，若是浮空输入，电路里需加上拉电阻

/******************************移植修改区**************************************/
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
