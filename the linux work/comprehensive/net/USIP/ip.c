#include "ip.h"
#include "tcp.h"
#include "udp.h"
/**********arp 映射表*******************/
static u8 MAC_WITH_IP[ARP_BUF_SIZE/11][11];
	
/***************本地IP*********************/
u8 Local_IP[4] = LOCAL_IP_ADDR;

//+20用于添加udp伪首部
static u8 buf1[MAX_IP_LEN+66];
static u8 buf2[MAX_IP_LEN+66];
MAC_BUF *TX_MAC_BUF;
MAC_BUF *RX_MAC_BUF;
IP_BUF *TX_IP_BUF;  
IP_BUF *RX_IP_BUF;
u8 IP_INT_TYPE;
u16 MAC_INT_TYPE;

/****************************************************************
函数名称：Bytes_Copy
函数功能：拷贝S1的Num个字节到S2
*****************************************************************/
void Bytes_Copy(u8* From_S1,u8* TO_S2,u16 Num)
{
	while(Num)
	{
		TO_S2[Num-1] = From_S1[Num-1];
		Num--;	
	}
}
u8 Bytes_Compare(u8* From_S1,u8* TO_S2,u16 Num)
{
	u8 p = 1;
	while(Num)
	{
		if(TO_S2[Num-1] != From_S1[Num-1])
			p = 0;
		Num--;	
	}
	return p;
}
u16 Word_CONV(u16 arg)
{
	//return (u16)((arg<<8)+(arg>>8));
	//大端格式不用转换
	return arg;
}
u16 Calculate_SUM(u16 *ptr,u16 len,u8 sum_skewing)
{
		u16 i;
    u32 sum = 0;
    u16 sum_t = 0;
    ptr[sum_skewing] = 0;
 
    for(i = 0;i<len;i++)
    {
     sum = ptr[i]+sum;
    }
		sum_t = sum&0x0000ffff;
		sum = sum>>16;
    sum_t = sum+sum_t;
    sum_t = (~sum_t);	
    ptr[sum_skewing] = sum_t;
    return sum_t;		
}

void Fill_MAC(u8* to_mac,u8* local_mac,u16 type)
{
    Bytes_Copy(to_mac,TX_MAC_BUF->TO_MAC,6);
    Bytes_Copy(local_mac,TX_MAC_BUF->LOCAL_MAC,6);
    TX_MAC_BUF->IP_TYPE= Word_CONV(type);
}



void Fill_IPADDR(u8* local_ip,u8* to_ip)
{
    Bytes_Copy(local_ip,TX_IP_BUF->LOCAL_IP,4);
    Bytes_Copy(to_ip,TX_IP_BUF->TO_IP,4);
}

void Fill_IP_SIZE(u8 head_size,u16 all_size)
{
    TX_IP_BUF->VER_HEAD_SIZE = 0x40+head_size;
    TX_IP_BUF->ALL_SIZE = Word_CONV(all_size);
	TX_IP_BUF->TOS = 0;
}

void Fill_Skewing(u8 flag,u16 skewing)
{
	  TX_IP_BUF->FLAG_SKEWING = (skewing&0x1f00)+(skewing&0xff);
    TX_IP_BUF->FLAG_SKEWING |= flag>>3;
	/*
    TX_IP_BUF->FLAG_SKEWING = ((skewing&0x1f00)>>8)+(skewing<<8);
    TX_IP_BUF->FLAG_SKEWING |= ((flag<<5)&0x00c0);
	*/
}
void Fill_TTL(u8 ttl,u8 tcp_type)
{
  TX_IP_BUF->TIME_OUT = ttl;
  TX_IP_BUF->TCP_TYPE = tcp_type;
}
u16 Fill_IP_SUM(void)
{
    return Calculate_SUM((u16*)TX_IP_BUF,10,5);
}
u8 Check_IP_SUM(void)
{
  u8 i;
  u32 sum = 0;
 
    for(i = 0;i<10;i++)
    {
     sum = ((u16*)RX_IP_BUF)[i]+sum;
    }
    sum = (sum>>16)+(sum&0x0000ffff);
    if(((~sum)&0xff7f)==0)
      return 1;
    else
      return 0;
}
void USIP_Init(void)
{
	TX_MAC_BUF = (MAC_BUF*)buf1;
	RX_MAC_BUF = (MAC_BUF*)buf2;
	TX_IP_BUF = ((IP_BUF*)(buf1+14));
	RX_IP_BUF = ((IP_BUF*)(buf2+14));
	UDP_Init();
	TCP_Init(); 
}
void Send_MAC(u8* to_ip,u8 tcp_type,u8* tcp_ptr,u16 len)
{
  TX_IP_BUF->SIGN++;
  Fill_MAC(ARP_Find_MAC(to_ip),LOCAL_MAC,IPTYPE_IP); 
  Fill_IPADDR(Local_IP,to_ip);
  Fill_IP_SIZE(5,len+20); 
  Fill_TTL(25,tcp_type);
  Fill_Skewing(0x00,0);
  Fill_IP_SUM();
  Bytes_Copy(tcp_ptr,(u8*)TX_MAC_BUF+34,len);
  enc28j60PacketSend(len+34,(u8*)TX_MAC_BUF);
}
	
u8* ARP_Find_MAC(u8* ip)
{
	u8 i;
	static u8 ARP_MAC[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x08,0x06,0x00,0x01,0x08,0x00,0x06,0x04};
	for(i = 0;i<(ARP_BUF_SIZE/11);i++)
	{
		if(Bytes_Compare(ip,MAC_WITH_IP[i],4))
		{
			MAC_WITH_IP[i][10] = 0;
			return MAC_WITH_IP[i]+4;
		}	
	}
	Bytes_Copy(ARP_MAC,TX_MAC_BUF->TO_MAC,20); 
	Bytes_Copy(ip,(u8*)TX_MAC_BUF+38,4); 


	Bytes_Copy(LOCAL_MAC,TX_MAC_BUF->LOCAL_MAC,6); 
	Bytes_Copy(LOCAL_MAC,(u8*)TX_MAC_BUF+22,6);
	Bytes_Copy(Local_IP,(u8*)TX_MAC_BUF+28,4);
	*((u8*)TX_MAC_BUF+20) = 0;
	*((u8*)TX_MAC_BUF+21) = 1;
	enc28j60PacketSend(42,(u8*)TX_MAC_BUF);
	return ARP_MAC;
}
void ARP_Process(void)			//arp处理函数
{
	u8 max = 0,i;
	if(((u8*)RX_MAC_BUF)[21]==1)	  //如果为arp请求
	{
		if(Bytes_Compare(Local_IP,(u8*)RX_MAC_BUF+38,4))
		{
			Bytes_Copy(RX_MAC_BUF->TO_MAC,TX_MAC_BUF->TO_MAC,20);//从接收帧拷贝前20字节到发送帧	
			Bytes_Copy(RX_MAC_BUF->LOCAL_MAC,TX_MAC_BUF->TO_MAC,6);	//填写目的地址
			Bytes_Copy(RX_MAC_BUF->LOCAL_MAC,(u8*)TX_MAC_BUF+32,6);
			Bytes_Copy((u8*)RX_MAC_BUF+28,(u8*)TX_MAC_BUF+38,4);
	
			Bytes_Copy(LOCAL_MAC,TX_MAC_BUF->LOCAL_MAC,6); 
			Bytes_Copy(LOCAL_MAC,(u8*)TX_MAC_BUF+22,6);
			Bytes_Copy(Local_IP,(u8*)TX_MAC_BUF+28,4);
	
			*((u8*)TX_MAC_BUF+20) = 0;
			*((u8*)TX_MAC_BUF+21) = 2;
			enc28j60PacketSend(42,(u8*)TX_MAC_BUF);
		}
	}
	if(((u8*)RX_MAC_BUF)[21]==2)	  //如果为arp应答
	{
		for(i = 0;i<(ARP_BUF_SIZE/11);i++)
		{
			if(MAC_WITH_IP[max][10]>MAC_WITH_IP[i][10])
			{
				max = i;			
			}
		}
		MAC_WITH_IP[max][10] = 0;
		Bytes_Copy(RX_MAC_BUF->LOCAL_MAC,MAC_WITH_IP[max]+4,6);
		Bytes_Copy((u8*)RX_MAC_BUF+28,MAC_WITH_IP[max],4);
		for(i = 0;i<(ARP_BUF_SIZE/11);i++)
		{
			if(MAC_WITH_IP[i][10]<250)
			{
				MAC_WITH_IP[i][10]++;
			}
		}	
	}
	MAC_INT_TYPE = 0x00;		
}
void ICMP_Process(void)
{
	u8* to_ip = RX_IP_BUF->LOCAL_IP;
	if(RX_IP_BUF->IP_DATA[0]==ICMPTYPE_PING)
	{

		Bytes_Copy((u8*)RX_IP_BUF,(u8*)TX_IP_BUF,RX_IP_BUF->ALL_SIZE);
    Fill_MAC(ARP_Find_MAC(to_ip),LOCAL_MAC,IPTYPE_IP); 
	  Fill_IPADDR(Local_IP,to_ip);
		TX_IP_BUF->IP_DATA[0] = 0;
		TX_IP_BUF->IP_DATA[RX_IP_BUF->ALL_SIZE-20] = 0;//ALL_SIZE：端格式区别
		Calculate_SUM((u16*)(TX_IP_BUF->IP_DATA),20,1);
		Packet_Send((RX_IP_BUF->ALL_SIZE+14),(u8*)TX_MAC_BUF);
		
	}
	IP_INT_TYPE = 0x00;	
}
u8 IP_Process(void)
{
	if(Check_IP_SUM())
	{
		if(RX_IP_BUF->TCP_TYPE==TCPTYPE_ICMP)
		{
			IP_INT_TYPE = IP_INT_ICMP;
			ICMP_Process();
			return IP_INT_ICMP;
		}	
		if(RX_IP_BUF->TCP_TYPE==TCPTYPE_UDP)
		{
			IP_INT_TYPE = IP_INT_UDP;
			UDP_Process();
			return IP_INT_UDP;		
		}
		if(RX_IP_BUF->TCP_TYPE==TCPTYPE_TCP)
		{
			IP_INT_TYPE = IP_INT_TCP;
			TCP_Process();
		  return IP_INT_TCP;
		}
		IP_INT_TYPE = UNDEF_INT;

		MAC_INT_TYPE = 0x00;
		return UNDEF_INT;
	}
	else
	{
		IP_INT_TYPE = 0x00;

		MAC_INT_TYPE = 0x00;
		return 0x00;
	}
}
u16 MAC_Process(void)
{
	u16 rev = 0;
	rev = Packet_Receive(MAX_IP_LEN+56,(u8*)RX_MAC_BUF);
	if(rev)
	{
		if(RX_MAC_BUF->IP_TYPE==IPTYPE_ARP)//arp
		{
			MAC_INT_TYPE = MAC_INT_ARP;
			ARP_Process();
			return MAC_INT_ARP;	
		}
		if(RX_MAC_BUF->IP_TYPE==IPTYPE_IP)//ip
		{		
			
			MAC_INT_TYPE = MAC_INT_IP;
			IP_Process();
			return MAC_INT_IP;		
		}
		MAC_INT_TYPE = UNDEF_INT;
		return UNDEF_INT;				
	}
	else
	{
		MAC_INT_TYPE = 0x00;
		return 0x00;
	}
}
