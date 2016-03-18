
#include "touch.h"

#ifdef MCU_51
	#include <STC_12.h>

	sbit TCS = P5^2;
	sbit TCK = P1^7;
	sbit TIN = P1^6;
	sbit TOUT = P1^5;
#endif
static void Delay()
{
	u16 i;
  for(i = 15;i>1;i--);
}
u16 Re_y(void)
{
  unsigned char i,t1 = 0x93;
  u16 t2;
  TCK_L;
  TCS_L;
  for(i = 0;i<8;i++)
  {
    
    if((t1&0x80)==0x80)	 
      TOUT_H;
    else
      TOUT_L;
    TCK_H;
	Delay();
    TCK_L;
   
    t1 = t1<<1;
  }
  Delay();
  t2 = 0;
  for(i = 0;i<12;i++)
  {
    t2 = t2<<1;
    TCK_H; 
    Delay();
    TCK_L;
    Delay();
    if(TIN==1)
      t2 = t2|0x01;
  }
  for(i = 0;i<4;i++)
  {
 
    TCK_H; 
    Delay();
    TCK_L;
  }
  TCS_H;
  return t2;
}
u16 Re_x(void)
{
  unsigned char i,t1 = 0xd3;
 u16 t2;
  TCK_L;
  TCS_L;
  for(i = 0;i<8;i++)
  {
    
    if((t1&0x80)==0x80)
      TOUT_H;
    else
      TOUT_L;
    TCK_H;
	Delay();
    TCK_L;
   
    t1 = t1<<1;
  }
  Delay();
  t2 = 0;
  for(i = 0;i<12;i++)
  {
    t2 = t2<<1;
    TCK_H; 
    Delay();
    TCK_L;
    Delay();
    if(TIN==1)
      t2 = t2|0x01;
  }
  for(i = 0;i<4;i++)
  {
 
    TCK_H; 
    Delay();
    TCK_L;
  }
  TCS_H;
  return t2;
}
u16 Re_z1()
{
  unsigned char i,t1 = 0xb3;
u16 t2;
  TCK_L;
  TCS_L;
  for(i = 0;i<8;i++)
  {
    
    if((t1&0x80)==0x80)
      TOUT_H;
    else
      TOUT_L;
    TCK_H;
	Delay();
    TCK_L;
   
    t1 = t1<<1;
  }
  Delay();
  t2 = 0;
  for(i = 0;i<12;i++)
  {
    t2 = t2<<1;
    TCK_H; 
    Delay();
    TCK_L;
    Delay();
    if(TIN==1)
      t2 = t2|0x01;
  }
  for(i = 0;i<4;i++)
  {
 
    TCK_H; 
    Delay();
    TCK_L;
  }
  TCS_H;
  return t2;
}
void T_GetPoint(u16 point_t[2])
{
	u32 x = 0,y = 0,tx,ty;
    u8 i = 0;
    for(;i<6;i++)
    {
         if(Re_z1()>100)
         {
          ty = ((Re_x()*4)/59);
          tx = ((Re_y()*4)/44);
          if(Re_z1()>30)
          {
            x = x+tx;
            y = y+ty;
          }
          else 
          {
            break;
          }
         }
         else
         {
		 	point_t[0] = 0;
  			point_t[1] = 0;
           return;
         }
    }
   point_t[0] = (u16)(x/(i))-13;
  point_t[1] = 260 - (u16)(y/(i));
}
