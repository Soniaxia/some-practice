#ifndef __SPI_H
#define __SPI_H


//SPI1��ʼ��
void	SPI1_Init(void);
//SPI1��дһ�ֽ�����
unsigned char	SPI1_ReadWrite(unsigned char writedat);

void CS_H(void);
void CS_L(void);

#endif
