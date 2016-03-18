#include "tcp.h"
#include "sock.h"
extern IP_BUF *RX_IP_BUF;  
extern IP_BUF *TX_IP_BUF;

extern u16 MAC_INT_TYPE;
extern u8 Local_IP[4];
TCP_BUF* RX_TCP_BUF;
TCP_BUF* TX_TCP_BUF;
static TCP_VIR_HEAD* VIR_HEAD;
TCP_INT_MSG	TCP_INT;
u32 Word_CONV32(u32 arg)
{
	/*
	u32 ret;
	((u8*)(&ret))[0] = ((u8*)(&arg))[3];
	((u8*)(&ret))[1] = ((u8*)(&arg))[2];
	((u8*)(&ret))[2] = ((u8*)(&arg))[1];
	((u8*)(&ret))[3] = ((u8*)(&arg))[0];
	return ret;
	*/
	return arg;
}
void TCP_Init(void)
{
	RX_TCP_BUF = (TCP_BUF*)RX_IP_BUF->IP_DATA;
	TX_TCP_BUF = (TCP_BUF*)TX_IP_BUF->IP_DATA;	
	Socket_Layer_Init();
}
void Fill_TCP(u8 to_ip[4],u16 port_local,u16 port_distance,u32 sn,u32 ack_sn,u8 flags,u16 data_size,u8* usr_data)
{
	u16 tcp_all_double_size;
	TX_TCP_BUF->HEAD_SIZE = 0x50;
	TX_TCP_BUF->LOCAL_PORT = Word_CONV(port_local);
	TX_TCP_BUF->TO_PORT = Word_CONV(port_distance);
	TX_TCP_BUF->SN = Word_CONV32(sn);
	TX_TCP_BUF->ACK_SN = Word_CONV32(ack_sn);
	TX_TCP_BUF->FLAGS = flags;
	TX_TCP_BUF->WINDOW_SIZE = Word_CONV(1024);
	TX_TCP_BUF->UGC_PTR = 0;
	Bytes_Copy(usr_data,(u8*)(TX_TCP_BUF->TCP_DATA),data_size);
	
	//填充伪首部
	
	tcp_all_double_size = (data_size+(TX_TCP_BUF->HEAD_SIZE>>4)*4+1)/2;
	TX_TCP_BUF->TCP_DATA[data_size] = 0;
	VIR_HEAD = (TCP_VIR_HEAD*)((u8*)TX_TCP_BUF+tcp_all_double_size*2);
	Bytes_Copy(Local_IP,(u8*)(VIR_HEAD->LOCAL_IP),4);
	Bytes_Copy(to_ip,(u8*)(VIR_HEAD->TO_IP),4);
	VIR_HEAD->BE_ZERO = 0;
	VIR_HEAD->TCPTYPE = TCPTYPE_TCP;//TCP此处不和UDP一样
	VIR_HEAD->ALL_SIZE = Word_CONV(data_size+(TX_TCP_BUF->HEAD_SIZE>>4)*4);

	//计算并填充检验和
	Calculate_SUM((u16*)TX_TCP_BUF,tcp_all_double_size+6,8);
}
u8 Check_TCP_SUM(void)
{
	u16 tcp_all_double_size;
	u16 sum,sum_t = RX_TCP_BUF->TCP_SUM;

	//填充伪首部
	
	tcp_all_double_size = ((Word_CONV(RX_IP_BUF->ALL_SIZE)-20+1)/2);
	*(u8*)((u8*)RX_IP_BUF+Word_CONV(RX_IP_BUF->ALL_SIZE)) = 0;
	VIR_HEAD = (TCP_VIR_HEAD*)((u8*)RX_TCP_BUF+tcp_all_double_size*2);
	Bytes_Copy(RX_IP_BUF->LOCAL_IP,(u8*)(VIR_HEAD->LOCAL_IP),4);
	Bytes_Copy(RX_IP_BUF->TO_IP,(u8*)(VIR_HEAD->TO_IP),4);
	VIR_HEAD->BE_ZERO = 0;
	VIR_HEAD->TCPTYPE = TCPTYPE_TCP;//TCP此处不和UDP一样
	VIR_HEAD->ALL_SIZE = Word_CONV(Word_CONV(RX_IP_BUF->ALL_SIZE)-20);

	//计算并填充检验和
	sum = Calculate_SUM((u16*)RX_TCP_BUF,tcp_all_double_size+6,8);
	if(sum==sum_t)
	{
		return 1;
	}
	else 
	{
		return 0;
	}		
}

u8 TCP_Process(void)
{
	IP_INT_TYPE = 0;
	if(RX_IP_BUF->ALL_SIZE>(MAX_IP_LEN+40))
	{
		return 0;
	}
	if(Check_TCP_SUM())
	{
		
		TCP_INT.MSG_TYPE = RX_TCP_BUF->FLAGS|0x80;
		TCP_INT.RECV_SIZE = Word_CONV(RX_IP_BUF->ALL_SIZE)-20-((RX_TCP_BUF->HEAD_SIZE>>4)*4);
		TCP_INT.PORT = Word_CONV(RX_TCP_BUF->TO_PORT);
		TCP_INT.SN = Word_CONV32(RX_TCP_BUF->SN);
		TCP_INT.ACK_SN = Word_CONV32(RX_TCP_BUF->ACK_SN);
		TCP_INT.DST_PORT = Word_CONV(RX_TCP_BUF->LOCAL_PORT);
		*((u32*)(&TCP_INT.DST_IP)) = *(u32*)(&RX_IP_BUF->LOCAL_IP);
		return 1;		
	}			
	else 
	return 0;  
}
u16 Get_TCP_DST_Port(void)
{
	return TCP_INT.DST_PORT;
}
u8* Get_TCP_DST_IP(void)
{
	return TCP_INT.DST_IP;
}
u16 Get_TCP_Data(u8* buf)
{
	u16 rcv_size = TCP_INT.RECV_SIZE;
	Bytes_Copy(RX_TCP_BUF->TCP_DATA,buf,TCP_INT.RECV_SIZE);
	TCP_INT.RECV_SIZE = 0;
	return rcv_size;
}
void Send_TCP_CTL(SOCKET_T* sock,u8 flags,u8 sn)
{
	Fill_TCP(sock->DST_IP,sock->LOCAL_PORT,sock->DST_PORT,sock->SN,TCP_INT.SN+sn,flags,0,0);
	Send_MAC(sock->DST_IP,TCPTYPE_TCP,(u8*)TX_TCP_BUF,20);	
}
u32 Get_TCP_INFO(TCP_INT_MSG* buf2)
{
	u32 t;

	t =  TCP_INT.RECV_SIZE;
	t |= TCP_INT.MSG_TYPE<<16;
	*buf2 =  TCP_INT;
	TCP_INT.RECV_SIZE = 0;
	TCP_INT.MSG_TYPE = 0;
	return t; 
}
