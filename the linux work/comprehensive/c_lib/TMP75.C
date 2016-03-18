#include "iic.h"
#include <STC_12.h>
u8 TMP75_address = 0x90;
#define   TMP75_cmd_TR         0x00//Temperature Register (READ Only)	 
#define   TMP75_cmd_CONFIG     0x01//Configuration Register (READ/WRITE) default set to a count = 0x60
#define   TMP75_cmd_TL         0x02//T(low) Register (READ/WRITE)
#define   TMP75_cmd_TH         0x03//T(high) Register (READ/WRITE)

void IIC_Delay(u8 tenus)
{
	u8 i,j;
	for(i=0;i<20;i++)
	for(j=0;j<tenus;j++);
}
void Write_TMP75(u8 cmd_r,u8 dat)
{
	IIC_start();
	IIC_wr_B(TMP75_address);
	IIC_wr_B(cmd_r);
	IIC_wr_B(dat>>8);
	IIC_wr_B(dat);
	IIC_stop();
}
u32 TMP75_Init(void)
{
	IIC_init();
	IIC_start();
	IIC_wr_B(TMP75_address);
	IIC_wr_B(TMP75_cmd_CONFIG);
	IIC_wr_B(0x6a);
	IIC_stop();
	return 0;
}
u16 TMP75_Read(void)
{
	u16 t;
	IIC_start();
	IIC_wr_B(TMP75_address);
	IIC_wr_B(TMP75_cmd_TR);
	IIC_start();
	IIC_wr_B(TMP75_address+1);
	t = IIC_re_B();
	t = t<<8;
	IIC_Delay(100);
	t = t+IIC_re_B();
	IIC_stop();
	t = t>>4;
	return t;
}
