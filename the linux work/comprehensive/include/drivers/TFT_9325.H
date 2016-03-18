#ifndef __tft_9325_h
#define	__tft_9325_h

//#define USING_BUS
#define MCU_51


#define DLAY_COUNT 200


#ifdef USING_BUS
//	#define    Write_Cmd(u8x)
//  #define    Write_Data_U16(u16y)
//  #define    Read_Data(void)
#else 
	#ifdef MCU_51
	/***********************51²¿·Ö******************************************/
	
		#define CS_PIN  P3^4
		#define RS_PIN  P3^5
		#define WR_PIN  P3^6
		#define RD_PIN  P3^7
		#define RST_PIN  P1^0	
	
		#define TRAN_DATA(bit16) P0=bit16;P2=bit16>>8
		#define RE_DATA() P0+(P2>>8)
	
	/*****************************************************************/
	
	#else
	/**********************STM32*******************************************/
		#define TRAN_DATA(bit16) P0=bit16;P2=bit16>>8	
		#define RS_H RS=1
		#define RS_L RS=0
		
		#define WR_H LWR=1
		#define WR_L LWR=0
		
		#define RD_H LRD=1
		#define RD_L LRD=0
		
		
		#define CS_H  CS=1
		#define CS_L  CS=0
		
		
		#define RST_H //RST=1
		#define RST_L //RST=0
	/*****************************************************************/
	#endif
#endif
#ifndef u8
	typedef unsigned short u16;
	typedef unsigned long u32;
	
	typedef unsigned char u8;
	typedef int s16;
#endif


#define ili9320_SetPoint TFT_ShowPoint1
#define WINDOW_XADDR_START	0x0050 // Horizontal Start Address Set
#define WINDOW_XADDR_END	0x0051 // Horizontal End Address Set
#define WINDOW_YADDR_START	0x0052 // Vertical Start Address Set
#define WINDOW_YADDR_END	0x0053 // Vertical End Address Set
#define GRAM_XADDR		    0x0020 // GRAM Horizontal Address Set
#define GRAM_YADDR		    0x0021 // GRAM Vertical Address Set
#define GRAMWR 			    0x0022 // memory write
/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#define Gray		   0xC0C0C0




//u16 get_point(int x,int y);
void ClearScreen(u16 bColor);
void TFT_Init(void);
void TFT_PutStr(unsigned short x, unsigned short y, char* p, unsigned int fColor, unsigned int bColor);
void TFT_ShowPoint2(int x0,int y0,int Color);
void TFT_ShowPoint1(int x0,int y0,int Color);
void TFT_Show_Bmp(unsigned int x,unsigned int y,unsigned int width,unsigned int high,unsigned short *ptr);
void TFT_Show_Bmp_8b(unsigned int x,unsigned int y,unsigned int width,unsigned int high,unsigned char *ptr);
u16 get_point(int x,int y);
void TFT_PutChar(unsigned short x, unsigned short y, char c, unsigned int fColor, unsigned int bColor);
void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1);
void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill);
void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);
#endif
