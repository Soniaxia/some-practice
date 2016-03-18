#ifndef __tcp_h
#define __tcp_h
#include "ip.h"
#include "udp.h"
#include "sock.h"
typedef UDP_VIR_HEAD TCP_VIR_HEAD; 

#define TCP_FLAG_URG 0x20
#define TCP_FLAG_ACK 0x10
#define TCP_FLAG_PSH 0x08
#define TCP_FLAG_RST 0x04
#define TCP_FLAG_SYN 0x02
#define TCP_FLAG_FIN 0x01


typedef struct{
	u16 LOCAL_PORT;
	u16 TO_PORT;
	u32 SN;
	u32 ACK_SN;
	u8 HEAD_SIZE;//占高4位，低四位保留
	u8 FLAGS;//高2位保留，低六位用作TCP数据报标志
	u16 WINDOW_SIZE;
	u16 TCP_SUM;
	u16 UGC_PTR;//紧急指针
	u8 TCP_DATA[MAX_IP_LEN-20-20];
} TCP_BUF;
typedef struct{
	u8 MSG_TYPE;
	u16 PORT;
	u16 DST_PORT;
	u8 DST_IP[4];
	u32 SN;
	u32 ACK_SN;
	u16 RECV_SIZE;
} TCP_INT_MSG;
extern TCP_BUF *TX_TCP_BUF;
u32 Word_CONV32(u32 arg);
void TCP_Init(void);
void Fill_TCP(u8 to_ip[4],u16 port_local,u16 port_distance,u32 sn,u32 ack_sn,u8 flags,u16 data_size,u8* usr_data);
u8 TCP_Process(void);
u32 Get_TCP_INFO(TCP_INT_MSG* buf2);
u16 Get_TCP_DST_Port(void);
u16 Get_TCP_Data(u8* buf);
u8* Get_TCP_DST_IP(void);
void Send_TCP_CTL(SOCKET_T* sock,u8 flags,u8 sn);

#endif 
