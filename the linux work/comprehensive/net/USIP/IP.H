#ifndef __ip_h
#define __ip_h
#ifndef u32 
typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#endif
/****************��ֲ�޸���******************/

#define LOCAL_IP_ADDR {192,168,1,23}//������IP��ַ

#define MAX_IP_LEN  128  //����С��1460
#define ARP_BUF_SIZE    11*2 //ARP���档�������11,����11��������
#define MAX_SOCK_NUM    4 // ��ͬʱʹ�õ��׽Ӷ˿���
extern void enc28j60PacketSend(unsigned int len, unsigned char* packet);
extern unsigned int enc28j60PacketReceive(unsigned int maxlen, unsigned char* packet);
#define Packet_Send enc28j60PacketSend
#define Packet_Receive enc28j60PacketReceive 
/***************��ֲ�޸���END*************************/
extern u8 IP_INT_TYPE;
extern u8 LOCAL_MAC[6];
#define IPTYPE_IP   0x0800
#define IPTYPE_ARP  0x0806
#define IPTYPE_RARP 0x8035
typedef struct {
  u8 TO_MAC[6];
  u8 LOCAL_MAC[6];
  u16 IP_TYPE;
  u8 MAC_DATA[1500];
} MAC_BUF;



#define IPVER_IPV4 0x04

#define GET_IP_VER(VER_HEAD_SIZE) (0x0f&VER_HEAD_SIZE)
#define GET_HEAD_SIZE(VER_HEAD_SIZE) ((0xf0&VER_HEAD_SIZE)>>4)

#define TOS_ICMP 0x04//��߿ɿ���
#define TOS_FTP  0x08//�������������
#define TOS_DNS  0x10//��Сʱ��
#define TOS_NORMOL  0x00//һ��

#define GET_FLAG_MF 0x8000
#define GET_FLAG_MD 0x4000
#define GET_SKEWING(FLAG_SKEWING) (((FLAG_SKEWING&0xff00)>>8)|((FLAG_SKEWING&0x001f)<<8))

#define TCPTYPE_ICMP 0x01
#define ICMPTYPE_PING  0x08

#define TCPTYPE_IGMP 0x02
#define TCPTYPE_TCP  0x06
#define TCPTYPE_EGP  0x08
#define TCPTYPE_IGP  0x09
#define TCPTYPE_UDP  0x11
#define TCPTYPE_IPV6 0x29
#define TCPTYPE_OSPF 0x59

/**************************�ж�����ֵ*********************************/
#define MAC_INT_IP   0x0800
#define MAC_INT_ARP  0x0806
#define MAC_INT__RARP 0x8035

#define IP_INT_ICMP 0x01
#define IP_INT_IGMP 0x02
#define IP_INT_TCP  0x06
#define IP_INT_EGP  0x08
#define IP_INT_IGP  0x09
#define IP_INT_UDP  0x11
#define IP_INT_IPV6 0x29
#define IP_INT_OSPF 0x59

#define UNDEF_INT 0xff

/*********************************************************************/
typedef struct {
  u8 VER_HEAD_SIZE;
  u8 TOS;
  u16 ALL_SIZE;
  u16 SIGN;//��ʶ
  u16 FLAG_SKEWING;
  u8 TIME_OUT;
  u8 TCP_TYPE;
  u16 HEAD_SUM;
  u8 LOCAL_IP[4];
  u8 TO_IP[4];
  u8 IP_DATA[1480];
} IP_BUF;


void Bytes_Copy(u8* From_S1,u8* TO_S2,u16 Num); 
u16 Word_CONV(u16 arg);

void Fill_MAC(u8* to_mac,u8* local_mac,u16 type);
void Fill_IPADDR(u8* local_ip,u8* to_ip);
void Fill_IP_SIZE(u8 head_size,u16 all_size);

void Fill_Skewing(u8 flag,u16 skewing);
void Fill_TTL(u8 ttl,u8 tcp_type);
u16 Calculate_SUM(u16 *ptr,u16 len,u8 sum_skewing);
u16 Fill_IP_SUM(void);
u8 Check_IP_SUM(void);
void USIP_Init(void);
void Send_MAC(u8* to_ip,u8 tcp_type,u8* tcp_ptr,u16 len);
void ARP_Process(void);			//arp��������
u16 MAC_Process(void);
u8* ARP_Find_MAC(u8* ip);
u8 IP_Process(void);
#endif