#ifndef __sock_h
#define __sock_h
#include "ip.h"
#include "udp.h"

#define SOCK_STATUS_RECV 	 		0x01
#define SOCK_STATUS_ON_CON 		0x02
#define SOCK_STATUS_ON_CLO 		0x04
#define SOCK_STATUS_LISTEN    0x08
#define SOCK_STATUS_HALF_OPEN 0x10

#define CONNECT_TYPE_CLT         0x80
#define CONNECT_TYPE_SERV        0x00

typedef struct {
	u8 TCP_TYPE;
	u16 LOCAL_PORT;
	u16 DST_PORT;
	u8 DST_IP[4];
	u32 SN;
	u8 SOCK_STATUS;
} SOCKET_T;
typedef SOCKET_T* LPSOCKET_T ;
u8 Socket_Process(SOCKET_T *sock);
u8 Socket_REG(SOCKET_T *sock);
u8 Socket_UNREG(SOCKET_T *sock);
void Socket_Layer_Init(void);
u8 Socket_Take_Mutex(u8 mutex,u8 time_out);
void Socket_Release_Mutex(u8 mutex);



#define Socket_Open Socket_REG
#define Socket_Close Socket_UNREG
void Socket_Process_All(void);
void Socket_Send_UDP(SOCKET_T* sock,u8* data_ptr,u16 data_size);
int Socket_Receive_UDP(SOCKET_T* sock,u8* data_ptr);
void Socket_Listen(SOCKET_T* sock);
void Socket_Stop_Listen(SOCKET_T* sock);
void Socket_Connect(SOCKET_T* sock);
void Socket_Finish(SOCKET_T* sock);
void Socket_Send_TCP(SOCKET_T* sock,u8* data_ptr,u16 data_size);
int Socket_Receive_TCP(SOCKET_T* sock,u8* data_ptr) ;
#include "tcp.h"
#endif
