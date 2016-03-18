#ifndef __rda5807_h
#define __rda5807_h

	#define H02_H 0xd0
	#define H02_L 0x09
	#define RST 0x02
	#define H03_H 0x00
	#define H03_L 0x13
	
	void RDA5807_Init(unsigned short ini_freq);
	unsigned char  RDA5807_Set_Freq(unsigned int freq);
#endif

