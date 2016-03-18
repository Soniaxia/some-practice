#include "sock.h"
extern UDP_INT_MSG UDP_INT;
extern TCP_INT_MSG TCP_INT;
static LPSOCKET_T SOCK_LIST[MAX_SOCK_NUM];
static u8 SOCK_BUSY = 0;
u8 Socket_Process(SOCKET_T *sock)
{

	switch (sock->TCP_TYPE&0x7f)
	{
		case TCPTYPE_UDP:
		{
			if((UDP_INT.PORT==sock->LOCAL_PORT)&&(UDP_INT.RECV_SIZE>0))
			{
				Bytes_Copy(Get_UDP_DST_IP(),(u8*)(&sock->DST_IP),4);
				sock->DST_PORT = Get_UDP_DST_Port();
				sock->SOCK_STATUS|=SOCK_STATUS_RECV;			
			}
			break;
		}
		case TCPTYPE_TCP:
		{
			if((TCP_INT.MSG_TYPE)&&(TCP_INT.PORT==sock->LOCAL_PORT))
			{
				TCP_INT.MSG_TYPE&=0x7f;	
				if(TCP_INT.RECV_SIZE>0)
				{
					sock->SOCK_STATUS|=SOCK_STATUS_RECV;
					Send_TCP_CTL(sock,TCP_FLAG_ACK,TCP_INT.RECV_SIZE);
				}
				if((TCP_INT.MSG_TYPE==TCP_FLAG_ACK)&&((sock->SN+1)==TCP_INT.ACK_SN))
				{
					sock->SN=TCP_INT.ACK_SN;
					sock->SOCK_STATUS|=SOCK_STATUS_ON_CON;

				}
				else if(TCP_INT.MSG_TYPE==TCP_FLAG_SYN)
				{
					if(((sock->TCP_TYPE&CONNECT_TYPE_CLT)==0)&&(sock->SOCK_STATUS&SOCK_STATUS_LISTEN)&&((sock->SOCK_STATUS&SOCK_STATUS_ON_CON)==0))
					{
						Bytes_Copy(Get_TCP_DST_IP(),(u8*)(&sock->DST_IP),4);
						sock->DST_PORT = Get_TCP_DST_Port();
						Send_TCP_CTL(sock,TCP_FLAG_ACK|TCP_FLAG_SYN,1);
					}
				}
				else if((TCP_INT.MSG_TYPE&TCP_FLAG_ACK)&&(TCP_INT.MSG_TYPE&TCP_FLAG_SYN))
				{
						sock->SN=TCP_INT.ACK_SN;
						if(sock->TCP_TYPE&CONNECT_TYPE_CLT)
						{
							Send_TCP_CTL(sock,TCP_FLAG_ACK,1);
						}
				}
				else if((TCP_INT.MSG_TYPE&TCP_FLAG_ACK)&&(TCP_INT.MSG_TYPE&TCP_FLAG_FIN))
				{
					sock->SN=TCP_INT.ACK_SN;
					Send_TCP_CTL(sock,TCP_FLAG_ACK,1);
					sock->SOCK_STATUS&= ~SOCK_STATUS_ON_CON;
				}
				else if(TCP_INT.MSG_TYPE&TCP_FLAG_ACK)
				{
						sock->SN=TCP_INT.ACK_SN;
				}
			}
			TCP_INT.MSG_TYPE = 0;
			break;
		}
		default : 
		{
		}
	}
	Socket_Release_Mutex(SOCK_BUSY);
	return sock->SOCK_STATUS;	
}
void Socket_Process_All(void)
{
	u8 i;
	if(Socket_Take_Mutex(SOCK_BUSY,1)==0)
	{
		return;
	}
	if(MAC_Process())
	{
		for(i = 0;i<MAX_SOCK_NUM;i++)
		{
				if(SOCK_LIST[i]!=0)
				Socket_Process(SOCK_LIST[i]);
		}
	}
	Socket_Release_Mutex(SOCK_BUSY);
}
u8 Socket_REG(SOCKET_T *sock)
{
	u8 i;
	for(i = 0;i<MAX_SOCK_NUM;i++)
	{
		if(SOCK_LIST[i]==0)
		{
			SOCK_LIST[i]=sock;
			sock->SOCK_STATUS = 0;
			sock->SN = 0;
			return 1;
		}
	}
	return 0;
}
u8 Socket_UNREG(SOCKET_T *sock)
{
	u8 i;
	for(i = 0;i<MAX_SOCK_NUM;i++)
	{
		if(SOCK_LIST[i]==sock)
		{
			SOCK_LIST[i]=0;
			return 1;
		}
	}
	return 0;	
}
void Socket_Send_UDP(SOCKET_T* sock,u8* data_ptr,u16 data_size)
{
	Socket_Take_Mutex(SOCK_BUSY,0);
	Send_UDP(sock->DST_IP,sock->DST_PORT,sock->LOCAL_PORT,data_ptr,data_size);
	Socket_Release_Mutex(SOCK_BUSY);
}

int Socket_Receive_UDP(SOCKET_T* sock,u8* data_ptr)
{
	Socket_Take_Mutex(SOCK_BUSY,0);
	if((UDP_INT.PORT==sock->LOCAL_PORT))//&&(UDP_INT.RECV_SIZE>0))
	{
		sock->SOCK_STATUS &= (~SOCK_STATUS_RECV);
		Socket_Release_Mutex(SOCK_BUSY);
		return Get_UDP_DATA(data_ptr);
	}
	else 
	{
		sock->SOCK_STATUS &= (~SOCK_STATUS_RECV);
		Socket_Release_Mutex(SOCK_BUSY);
		return -1;
	}
}
void Socket_Listen(SOCKET_T* sock)
{
	sock->SOCK_STATUS|=SOCK_STATUS_LISTEN;
}
void Socket_Stop_Listen(SOCKET_T* sock)
{
	sock->SOCK_STATUS&=(~SOCK_STATUS_LISTEN);
}
void Socket_Connect(SOCKET_T* sock)
{
	Socket_Take_Mutex(SOCK_BUSY,0);
	Send_TCP_CTL(sock,TCP_FLAG_SYN,0);
	Socket_Release_Mutex(SOCK_BUSY);
}
void Socket_Finish(SOCKET_T* sock)
{
		Socket_Take_Mutex(SOCK_BUSY,0);
		Send_TCP_CTL(sock,TCP_FLAG_ACK|TCP_FLAG_FIN,0);
		Socket_Release_Mutex(SOCK_BUSY);
}
void Socket_Send_TCP(SOCKET_T* sock,u8* data_ptr,u16 data_size)
{
	Socket_Take_Mutex(SOCK_BUSY,0);
	Fill_TCP(sock->DST_IP,sock->LOCAL_PORT,sock->DST_PORT,sock->SN,TCP_INT.SN+1,TCP_FLAG_PSH|TCP_FLAG_ACK,data_size,data_ptr);
	Send_MAC(sock->DST_IP,TCPTYPE_TCP,(u8*)TX_TCP_BUF,(TX_TCP_BUF->HEAD_SIZE>>4)*4+data_size);	
	Socket_Release_Mutex(SOCK_BUSY);
}
int Socket_Receive_TCP(SOCKET_T* sock,u8* data_ptr) 
{
	Socket_Take_Mutex(SOCK_BUSY,0);
	if((TCP_INT.PORT==sock->LOCAL_PORT))//&&(TCP_INT.RECV_SIZE))
	{
		sock->SOCK_STATUS &= (~SOCK_STATUS_RECV);
		Socket_Release_Mutex(SOCK_BUSY);
		return Get_TCP_Data(data_ptr);
	}
	else 
	{
		sock->SOCK_STATUS &= (~SOCK_STATUS_RECV);
		Socket_Release_Mutex(SOCK_BUSY);
		return -1;
	}
}
void Socket_Layer_Init(void)
{
	u8 i;
	for(i = 0;i<MAX_SOCK_NUM;i++)
	{
		SOCK_LIST[i]=0;	
	}	
}
u8 Socket_Take_Mutex(u8 mutex,u8 time_out)
{
	if(time_out==0)
	{
		while(1)
		{
			if(mutex)
			{
				//os_delay(1);
			}
			else 
			{
				mutex = 1;
				return 1;
			}
		}
	}
	else
	{
		while(time_out)
		{
			if(mutex)
			{
				time_out--;
				//os_delay(1);
			}
			else 
			{
				mutex = 1;
				return 1;
			}
		}
	}
	
	return 0;
}
void Socket_Release_Mutex(u8 mutex)
{
		mutex = 0;
}