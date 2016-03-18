#ifndef __iic_h
#define __iic_h
#ifndef u8
#define u8 unsigned char 
#endif
#ifndef u16
#define u16 unsigned short
#endif
#ifndef u32
#define u32 unsigned long
#endif


void IIC_init(void);
void IIC_start(void);
void IIC_answer(void);
void IIC_reanswer(void);
void IIC_stop(void);
void IIC_wr_B(u8 data1);
u8 IIC_re_B(void);


#endif 