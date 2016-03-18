
#ifndef __touch_h
#define __touch_h
#include"tft_9325.h"
#define MCU_51
#ifdef MCU_51

#define TOUT_H TOUT=1
#define TOUT_L TOUT=0

#define TCS_H TCS=1
#define TCS_L TCS=0


#define TCK_H TCK=1
#define TCK_L TCK=0
#endif
//
//
//#define TIN PBin(15)
//
//#define TOUT_H PBout(14)=1
//#define TOUT_L PBout(14)=0
//
//#define TCS_H PBout(9)=1
//#define TCS_L PBout(9)=0
//
//
//#define TCK_H PBout(13)=1
//#define TCK_L PBout(13)=0

void T_GetPoint(u16 point_t[2]);
//#define T_Pin_Init() GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,0x08)
#endif
