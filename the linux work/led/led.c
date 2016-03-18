#include <STC_12.h>		

void main()
{
	unsigned char i,ch;
	TMOD=0X10;
	TL1=-20000;
	TH1=(-20000)>>8;
	TR1=1;
	ch=0xfe;
	while(1)
	{

			for(i=0;i<200;i++)
			{
				while(TF1==0);
				TF1=0;
				TR1=0;
				TL1=-20000;
				TH1=(-20000)>>8;
				TR1=1;
			}
		P4=ch;
		ch=(ch<<1)+1;
		if(ch==0xef)
			ch=0xfe;
		
	}
}
								