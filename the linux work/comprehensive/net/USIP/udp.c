#include "udp.h"
UDP_BUF *RX_UDP_BUF;
UDP_BUF *TX_UDP_BUF;
static UDP_VIR_HEAD *VIR_HEAD;
extern IP_BUF *RX_IP_BUF;  
extern IP_BUF *TX_IP_BUF;
extern u8 IP_INT_TYPE;
extern u16 MAC_INT_TYPE;
UDP_INT_MSG UDP_INT;
extern u8 Local_IP[4];
void UDP_Init(void)
{
	UDP_INT.RECV_SIZE = 0;
	RX_UDP_BUF = (UDP_BUF *)(RX_IP_BUF->IP_DATA);
	TX_UDP_BUF = (UDP_BUF *)(TX_IP_BUF->IP_DATA);	
}
u16 Fill_UDP(u8 to_ip[4],u16 port_local,u16 port_distance,u8* usr_data,u16 data_size)
{
	//填充UDP报文
	TX_UDP_BUF->LOCAL_PORT = Word_CONV(port_local);
	TX_UDP_BUF->TO_PORT = Word_CONV(port_distance);
	TX_UDP_BUF->ALL_SIZE = Word_CONV(data_size+8);
	Bytes_Copy(usr_data,(u8*)TX_UDP_BUF->UDP_DATA,data_size);
	TX_UDP_BUF->UDP_DATA[data_size] = 0x00;//填充零字节

	//填充伪首部
	VIR_HEAD = (UDP_VIR_HEAD*)(TX_UDP_BUF->UDP_DATA+(((data_size+1)/2)*2));
	Bytes_Copy(Local_IP,(u8*)(VIR_HEAD->LOCAL_IP),4);
	Bytes_Copy(to_ip,(u8*)(VIR_HEAD->TO_IP),4);
	VIR_HEAD->BE_ZERO = 0;
	VIR_HEAD->TCPTYPE = TCPTYPE_UDP;
	VIR_HEAD->ALL_SIZE = TX_UDP_BUF->ALL_SIZE;

	//计算并填充检验和
	Calculate_SUM((u16*)TX_UDP_BUF,(data_size+21)/2,3);
	return (data_size+8);	
}
u8 Check_UDP_SUM(void)
{
	u16 sum_t,t;
	sum_t = RX_UDP_BUF->UDP_SUM;
	RX_UDP_BUF->UDP_SUM = 0;
	RX_UDP_BUF->UDP_DATA[Word_CONV(RX_UDP_BUF->ALL_SIZE)] = 0;
	VIR_HEAD = (UDP_VIR_HEAD*)((u8*)RX_UDP_BUF+((Word_CONV(RX_UDP_BUF->ALL_SIZE)+1)/2)*2);

	Bytes_Copy(RX_IP_BUF->LOCAL_IP,(u8*)(VIR_HEAD->LOCAL_IP),4);
	Bytes_Copy(RX_IP_BUF->TO_IP,(u8*)(VIR_HEAD->TO_IP),4);
	VIR_HEAD->BE_ZERO = 0;
	VIR_HEAD->TCPTYPE = TCPTYPE_UDP;
	VIR_HEAD->ALL_SIZE = RX_UDP_BUF->ALL_SIZE;
	t = Calculate_SUM((u16*)RX_UDP_BUF,(u16)(Word_CONV(RX_UDP_BUF->ALL_SIZE)+13)/2,3);
	if(t==sum_t)
	{
		return 1;	
	}
	else
	{
		return 0;
	}
}

u16 UDP_Process(void)
{
	if(Check_UDP_SUM())
	{
		UDP_INT.PORT = Word_CONV(RX_UDP_BUF->TO_PORT);
		UDP_INT.RECV_SIZE = Word_CONV(RX_UDP_BUF->ALL_SIZE)-8;
		UDP_INT.DST_PORT = Word_CONV(RX_UDP_BUF->LOCAL_PORT);
		*((u32*)(&UDP_INT.DST_IP)) = *(u32*)(&RX_IP_BUF->LOCAL_IP);
		IP_INT_TYPE = 0;
		return UDP_INT.PORT;
	}
	else 
	{	
		IP_INT_TYPE = 0;
		return 0;
	}	
}
u16 Get_UDP_DATA(u8* buf)
{
	u16 t;
	Bytes_Copy(RX_UDP_BUF->UDP_DATA,buf,UDP_INT.RECV_SIZE);
	t =  UDP_INT.RECV_SIZE;
	UDP_INT.RECV_SIZE = 0;	
	return t;
}
u16 Get_UDP_DST_Port(void)
{
	return UDP_INT.DST_PORT;
}
u8* Get_UDP_DST_IP(void)
{
	return UDP_INT.DST_IP;
}
u16 Send_UDP(u8 to_ip[4],u16 port_distance,u16 port_local,u8* usr_data,u16 data_size)
{
	Fill_UDP(to_ip,port_local,port_distance,usr_data,data_size);
	Send_MAC(to_ip,TCPTYPE_UDP,(u8*)TX_UDP_BUF,data_size+8);
	return 0;
}
