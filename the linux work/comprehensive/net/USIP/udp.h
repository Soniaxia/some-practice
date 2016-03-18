#ifndef __udp_h
#define __udp_h
#include "ip.h"

typedef struct {
	u16 LOCAL_PORT;
	u16 TO_PORT;
	u16 ALL_SIZE;
	u16 UDP_SUM;
	u8 UDP_DATA[1480-8+20];

} UDP_BUF;


typedef struct {
	u8 LOCAL_IP[4];
	u8 TO_IP[4];
	u8 BE_ZERO;
	u8 TCPTYPE;
	u16 ALL_SIZE;

} UDP_VIR_HEAD;


typedef struct{
	u16 PORT;
	u16 DST_PORT;
	u8 DST_IP[4];
	u16 RECV_SIZE;
} UDP_INT_MSG;

extern UDP_BUF *TX_UDP_BUF;
void UDP_Init(void);
u16 Get_UDP_DATA(u8* buf);
u16 Get_UDP_DST_Port(void);
u8* Get_UDP_DST_IP(void);
u16 Send_UDP(u8 to_ip[4],u16 port_distance,u16 port_local,u8* usr_data,u16 data_size);

u16 Fill_UDP(u8 to_ip[4],u16 port_local,u16 port_distance,u8* usr_data,u16 data_size);
u16 UDP_Process(void);

#endif
