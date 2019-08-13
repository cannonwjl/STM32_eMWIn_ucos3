#include "LowRFM.h"
#include "usart.h"
#include "Platform_Configure.h"
#include "string.h"
#include "system_platform.h"
#include "stm32f10x.h"

struct LowRF_callbacks *LowRF_callback;

static uint8_t LowRF_received_framebuf[78];
//static uint8_t RecvframeDA[2]={0,0};
//static uint8_t RecvframeLSD[2]={0,0};
//static uint8_t RecvframeLength[1]={0};
static uint8_t RecvgroupId[1]={0};//sinkid
//static uint8_t RecvframeType[1]={0};//am type

//static uint8_t SerialframeHead[3]={0x7e,0x45,0x00};
//static uint8_t SerialframeDA[2]={0xff,0xff};
//static uint8_t SerialframeLSA[2]={0,0};
//static uint8_t SerialframeLength[1]={0};
static uint8_t groupid[1]={0};//sinkid
//static uint8_t SerialframeType[1]={0x0b};//am type
//static uint8_t SerialframeCRC[2]={0,0};
//static uint8_t SerialframeTail[1]={0x7e};

// static uint8_t LowRF_frame_buf[58];
// static uint8_t hdlc_frame_buf[58];

u8 TIME[]="TIME";
u8 READ_U[]="READ";
u8 WRITE[]="WRITE";
u8 MODE[]="MODE";
u8 RFFREQ[]="RFFREQ";
u8 STA_U[]="STA";
u8 STAIP_U[]="STAIP";
u8 SOCK_U[]="SOCK";
u8 RETRYNUM[]="RETRYNUM";
u8 ADDR[]="ADDR";
u8 LCD_U[]="LCD";
u8 CCS811[]="CCS";
u8 HCHO[]="HCHO";
u8 COZ[]="COZ";
u8 WAITWIFI[]="WAITWIFI";
u8 RES_U[]="RES";
u8 RESTORE[]="RESTORE";
u8 WIFISENDT[]="WIFISENDT";

u8 RF[]="RF";
u8 WIFI[]="WIFI";

u8 sink_id;//重要 发给那个网关的 注册之后要记下
u8 sink_id2[4];//重要 发给那个网关的 注册之后要记下
u32 GatewayTime;//网关得到的时间戳
uint16_t tmpseq;


//MsgAck msgack;
MsgAckEchoV2 msgack2;
u8 Draw_param3(u8 *ptr,u8 size);
extern u16 rece_seqnum_V2;//新版协议收到包序
extern u16 send_seqnum_V2;
void Reply_configure(u8 WifiOrUart,uint8_t *pData,uint16_t size);
u8 Draw_param1(u8 *ptr,u8 size);
u8 Draw_param2(u8 *ptr,u8 size);
u32 param1,param2,param3;
extern uint8_t WIFI_Send_TIME_Range[4];
uint8_t getRecvgroupId(void)
{
	return RecvgroupId[0];
}

void setLowRFGroupID(uint8_t gid)
{
   groupid[0]=gid;
   return;
}

uint8_t getLowRFGroupID(){
	return groupid[0];
}

uint16_t hdlc_decode(uint8_t * hdlcbuf, uint8_t* buf, uint16_t len)//加入转义的转换，将第一个参数hdlcbuf变成buf
{
	uint16_t i,j;
	if(hdlcbuf[0]!=0x7e && hdlcbuf[len-1]!=0x7e) return 0;
	if(len<3) return 0;
	buf[0]=0x7e;
	for(i=1,j=1;i<len-1;j++)
	{
		if(hdlcbuf[i]!=0x7d) //7d需要转义
		{
			buf[j]=hdlcbuf[i];
			i++;
		}
		else
		{
			if(hdlcbuf[i+1]<=0x5d) buf[j]=0x7d;
			else buf[j]=0x7e;
			i+=2;
		}
	}
	buf[j++]=0x7e;
	return j;
}

uint16_t hdlc_encode(uint8_t* buf, uint8_t* hdlcbuf, uint16_t len)
{
	uint16_t i,j;
	if(buf[0]!=0x7e && buf[len-1]!=0x7e) return 0;
	for(i=0,j=0;i<len;i++)
	{
		if(i==0 || i==len-1) hdlcbuf[j++]=0x7e;
		else
		{
			if(buf[i]==0x7e)
			{
				hdlcbuf[j++]=0x7d;
				hdlcbuf[j++]=0x5e;

			}
			else if(buf[i]==0x7d)
			{
				hdlcbuf[j++]=0x7d;
				hdlcbuf[j++]=0x5d;
			}
			else
			{
				hdlcbuf[j++]=buf[i];
			}
		}
	}
	return j;
}

uint16_t crc_cal(uint8_t* buf, uint16_t len)
{
	uint16_t crc=0;
	uint16_t temp=0;
	uint16_t i,j;
	for(i=0;i<len;i++)
	{
		temp=((uint16_t)buf[i])<<8;
		crc^=temp;
		for(j=0;j<8;j++)
		{
			if((crc&0x8000)!=0)
			{
				crc=(crc<<1)^0x1021;
			}
			else
			{
				crc=crc<<1;
			}
		}
	}
	return crc;
}

int check_crc(uint8_t *buf,uint16_t len)
{   
	uint16_t origin_crc,crc;
	crc = 0;
	origin_crc=(uint16_t)(buf[len-2]<<8)+(uint16_t)buf[len-1];	//获取原数据里面的CRC,这里的len是到CRC的长度
  //  for(i=0;i<len;i++) printf("%x ",*(buf+i));
	crc=crc_cal(buf, len-2);	//计算获取到的数据里面的CRC，len是到payload的长度
//	printf("%d,%d",origin_crc,crc);
//	printf("%c",crc);
//	printf("%d",len);
//	printf("%c",buf[0]);printf("%c",buf[1]);printf("%c",buf[2]);
	if(origin_crc==crc) 
		return 1;
	else
	{
		printf("crc wrong,%c%c",crc>>8,crc);
		return 0;
	}
}

u8 LowRF_input(char *LowRF_input_data,uint8_t len)//返回1为收到注册包 返回2为数据包
{
	uint16_t j=0;
	u8 LowRF_input_returnflag=RECEBack_ERROR;
  j=hdlc_decode((uint8_t*)LowRF_input_data, LowRF_received_framebuf,len);//将串口接收到的数据进行转义，变到第二个参数里面  //检查消息类型是否为0x0b

//	memcpy(&msgack2,LowRF_received_framebuf+17,j);

  if(j>0)	//j是转义之后的原始数据长度，判断原始命令是不是坏的		
	{
    if(check_crc(LowRF_received_framebuf+1,j-2))	//crc校验没问题  j-2是什么意思
	  {
//			printf("\r\n CRC \r\n");
//				printf("	0X%2X  0X%2X  0X%2X ", LowRF_received_framebuf[1],LowRF_received_framebuf[2]);
			if(LowRF_received_framebuf[1]==0x44)	//检查前缀0x7e,44
			{
//				tmpseq=(uint16_t)(msgack2.packetnum [0]<<8)+(uint16_t)msgack2.packetnum[1];//获取传来数据中的seq_num，循环使用的包编号
				tmpseq=(uint16_t)(LowRF_received_framebuf[j-5]<<8)+(uint16_t)LowRF_received_framebuf[j-4];//获取传来数据中的seq_num，循环使用的包编号
				printf("\r\n  ID: tmpseq:%d",tmpseq);
					if((LowRF_received_framebuf[6]==(u8)(Address_ID>>24)) && (LowRF_received_framebuf[7]==(u8)(Address_ID>>16))&& (LowRF_received_framebuf[8] ==(u8)(Address_ID>>8)) && (LowRF_received_framebuf[9] ==(u8)(Address_ID)))
					{
//							printf("\r\n  mine  \r\n");
//							printf("\r\n receve Gateway package: \r\n");
//						for( int k=0; k<j; k++ )
//								printf(" %2X", LowRF_input_data[k]);
						
						if(LowRF_received_framebuf[10]==0x12)
						{
							sink_id2[0]=LowRF_received_framebuf[2];//获取网关ID
							sink_id2[1]=LowRF_received_framebuf[3];
							sink_id2[2]=LowRF_received_framebuf[4];
							sink_id2[3]=LowRF_received_framebuf[5];
							
							Is_register_flag=1;//注册成功
							GatewayTime=(u32)(LowRF_received_framebuf[20])+(u32)(LowRF_received_framebuf[19]<<8)+(u32)(LowRF_received_framebuf[18]<<16)+(u32)(LowRF_received_framebuf[17]<<24);
							printf("\r\n  first register GatewayTime：%d sink_id2:%d %d %d %d",GatewayTime,sink_id2[0],sink_id2[1],sink_id2[2],sink_id2[3]);
							LowRF_input_returnflag= RECEBack_Echo;
						}
						else if(LowRF_received_framebuf[10]==0x13)//收到传感数据回包
						{
							if((packet_sequence_send==tmpseq) && (Is_register_flag==1))
							{
								LowRF_input_returnflag= RECEBack_Data;
								packet_sequence_send=0;//防止收到重复包 再删一次
							}
						}
						else
							LowRF_input_returnflag= 0;
						
					}
			 
			}
//			else //不知道弄啥的
//			{//non-protocal frame
//				recv_callback(LowRF_received_framebuf,len);// callback function
//			}//end 0x45
    }//end crc 
  }//end hdlc
	return LowRF_input_returnflag;
}


u8 check_WIFI_pack_single(char *LowRF_input_data,uint8_t len)/////解析WIFI 收到的单个包
{
	uint16_t j=0;
//	uint8_t i;
	u8 LowRF_input_returnflag=0;
  j=hdlc_decode((uint8_t*)LowRF_input_data, LowRF_received_framebuf,len);//将串口接收到的数据进行转义，变到第二个参数里面  //检查消息类型是否为0x0b

//	for(i=0;i<j;i++)
//	{
//		printf(" %02X",LowRF_received_framebuf[i]);
//	}
//	memcpy(&msgack2,LowRF_received_framebuf+17,j);

  if(j>0)	//j是转义之后的原始数据长度，判断原始命令是不是坏的		
	{
    if(check_crc(LowRF_received_framebuf+2,j-3))	//crc校验没问题  j-2是什么意思
	  {
//			printf("\r\n CRC \r\n");
//				printf("	0X%2X  0X%2X  0X%2X ", LowRF_received_framebuf[1],LowRF_received_framebuf[2]);
			if(LowRF_received_framebuf[1]==0x44)	//检查前缀0x7e,44
			{
//				tmpseq=(uint16_t)(msgack2.packetnum [0]<<8)+(uint16_t)msgack2.packetnum[1];//获取传来数据中的seq_num，循环使用的包编号
				tmpseq=(uint16_t)(LowRF_received_framebuf[j-5]<<8)+(uint16_t)LowRF_received_framebuf[j-4];//获取传来数据中的seq_num，循环使用的包编号
				printf("\r\n  ID: tmpseq:%d  \r\n",tmpseq);
					if((LowRF_received_framebuf[6]==(u8)(Address_ID>>24)) && (LowRF_received_framebuf[7]==(u8)(Address_ID>>16))&& (LowRF_received_framebuf[8] ==(u8)(Address_ID>>8)) && (LowRF_received_framebuf[9] ==(u8)(Address_ID)))
					{
							printf("\r\n  mine  \r\n");
//							printf("\r\n receve Gateway package: \r\n");
//						for( int k=0; k<j; k++ )
//								printf(" %2X", LowRF_input_data[k]);
						
						if(LowRF_received_framebuf[10]==0x12)
						{
							sink_id2[0]=LowRF_received_framebuf[2];//获取网关ID
							sink_id2[1]=LowRF_received_framebuf[3];
							sink_id2[2]=LowRF_received_framebuf[4];
							sink_id2[3]=LowRF_received_framebuf[5];
							
							Is_register_flag=1;//注册成功
							GatewayTime=(u32)(LowRF_received_framebuf[20])+(u32)(LowRF_received_framebuf[19]<<8)+(u32)(LowRF_received_framebuf[18]<<16)+(u32)(LowRF_received_framebuf[17]<<24);
							printf("\r\n  first register GatewayTime：%d sink_id2:%d %d %d %d \r\n",GatewayTime,sink_id2[0],sink_id2[1],sink_id2[2],sink_id2[3]);
							LowRF_input_returnflag= 1;
						}
						else if(LowRF_received_framebuf[10]==0x17)//收到传感数据回包
						{
							if((packet_sequence_send==tmpseq) && (Is_register_flag==1))
							{
								LowRF_input_returnflag= 2;
								packet_sequence_send=0;//防止收到重复包 再删一次
							}
						}
						else
							LowRF_input_returnflag= 0;
						
					}
			 
			}
//			else //不知道弄啥的
//			{//non-protocal frame
//				recv_callback(LowRF_received_framebuf,len);// callback function
//			}//end 0x45
    }//end crc 
  }//end hdlc
	return LowRF_input_returnflag;
}

u8 Need_COnfig_WIFI_Flag=0;
u8 USART_input_param(char *LowRF_input_data,uint8_t len)//串口收到配置参数
{
//	uint16_t j=0;
	u8 i,l,n,k=0;
//	u8 lenthparam=0;
	u8 reply_flag=0;
	u8  USART_inputflag=0;	
	u8  PARAM_RX_STA=0;    //接收状态标记	
	char PARAM_RX_BUF[100];
	u8 PARAM_RX_BUF2[100];
	u8 PARAM_RX_BUF3[100];
	char replyStr[100];
//	u8 sendlenth;
//	char *rece_buff[6];
	memcpy(PARAM_RX_BUF, LowRF_input_data, len);
	l=0;
	n=0;
	k=0;
//	u8 flagnum=0;
//	for(i=0;i<len;i++)
//	{
//		printf(":%c",PARAM_RX_BUF[i]);
//	}
	for(i=0;i<len;i++)
	{
//		printf("\r\n i=%d",i);
		if((PARAM_RX_STA==0)&&(PARAM_RX_BUF[i]=='A')&&(PARAM_RX_BUF[i+1]=='T')&&(PARAM_RX_BUF[i+2]=='+'))
		{
			PARAM_RX_STA=1;
//			printf("\r\n AT+  i=%d",i);
//			k=i;
			i=i+2;			
		}
		else if((PARAM_RX_STA==1)&&(PARAM_RX_BUF[i]>='A')&&(PARAM_RX_BUF[i]<='Z'))
		{
//			printf(":%c %d %d",PARAM_RX_BUF[i],i,len);
			PARAM_RX_BUF2[l]=PARAM_RX_BUF[i];
			l++;
		}
		else if((PARAM_RX_STA==1)&&(PARAM_RX_BUF[i]=='='))
		{
			PARAM_RX_STA=2;
//			printf("====");
		}
		else if((PARAM_RX_STA==2)&&(PARAM_RX_BUF[i]!='\r')&&(PARAM_RX_BUF[i]!='\n'))
		{
//			printf(":%c %d",PARAM_RX_BUF[i+k],i);
			PARAM_RX_BUF3[n]=PARAM_RX_BUF[i+k];
			n++;
		}
	}
	PARAM_RX_BUF2[l]='\0';
	PARAM_RX_BUF3[n]='\0';
	if(Buffercmp((u8*)PARAM_RX_BUF2,TIME,5)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=4;
		}
		else
		{
			reply_flag=1;
//			flagnum=0;
			Draw_param3(PARAM_RX_BUF3,n);
			collection_time_min=param1;
			GettimeEcho_time_Hour=param3;Re_Registration_time_min=param2;
			printf("\r\nparam:%u %u %u",param1,param2,param3);
			USART_inputflag=3;
		}				
		if(reply_flag>0)
		{
			sprintf(replyStr,"TIME=%u,%u,%u\r\n" ,collection_time_min,Re_Registration_time_min,GettimeEcho_time_Hour);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,READ_U,5)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
//			if(CCS811_HaveOrNot_State==1)
//				sprintf(replyStr,"READ=EZ261,C\r\n");
//			else
				sprintf(replyStr,"READ=EZ261\r\n");
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
		USART_inputflag=1;
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,WRITE,6)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			sprintf(replyStr,"WRITE=OK\r\n");
			delay_ms(4);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
		USART_inputflag=2;
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,MODE,5)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			USART_inputflag=6;
			reply_flag=1;
//			printf("\r\n ?");
		}
		else if(Buffercmp((u8*)PARAM_RX_BUF3,RF,n)==0)
		{
			reply_flag=2;
			USART_inputflag=5;
			WIFI_or_RF_SendMode=RF_SEND;
//			printf("\r\n RF");
		}
		else if(Buffercmp((u8*)PARAM_RX_BUF3,WIFI,n)==0)
		{
			reply_flag=3;
			USART_inputflag=5;
			WIFI_or_RF_SendMode=WIFI_SEND;
//			printf("\r\n WIFI");
		}
		if(WIFI_or_RF_SendMode==WIFI_SEND)
		{
			sprintf(replyStr,"MODE=WIFI\r\n");
		}
		else if(WIFI_or_RF_SendMode==RF_SEND)
		{
			sprintf(replyStr,"MODE=RF\r\n");
		}
		delay_ms(2);//不加会卡死
		if(reply_flag>0)
		{
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,RFFREQ,7)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=8;
		}
		else
		{
			Draw_param1(PARAM_RX_BUF3,n);
			if((param1>410)&&(param1<525))
			{
				reply_flag=1;
				Rf_fequence=param1;
				USART_inputflag=7;
			}
			else
				reply_flag=0;

		}				
		if(reply_flag>0)
		{
			sprintf(replyStr,"RFFREQ=%d\r\n" ,Rf_fequence);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(strcmp((char*)PARAM_RX_BUF2,(char*)STA_U)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=10;
		}
		else
		{	
			memset(sta,'\0',70);
			reply_flag=1;
			USART_inputflag=9;
			memcpy(sta,PARAM_RX_BUF3,n);
		}				
		if(reply_flag>0)
		{
			sprintf(replyStr,"STA=%s\r\n",sta);
			Need_COnfig_WIFI_Flag=1;
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(strcmp((char*)PARAM_RX_BUF2,(char*)STAIP_U)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=12;
		}
		else
		{	
			memset(staip,'\0',100);
			reply_flag=1;
			memcpy(staip,PARAM_RX_BUF3,n);
			USART_inputflag=11;
		}				
		if(reply_flag>0)
		{
			sprintf(replyStr,"STAIP=%s\r\n",staip);
			Need_COnfig_WIFI_Flag=1;
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,SOCK_U,5)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=14;
		}
		else
		{	
			memset(sock,'\0',70);
			reply_flag=1;
			memcpy(sock,PARAM_RX_BUF3,n);
			USART_inputflag=13;
		}				
		if(reply_flag>0)
		{
			sprintf(replyStr,"SOCK=%s\r\n",sock);
			Need_COnfig_WIFI_Flag=1;
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,RETRYNUM,8)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=16;
		}
		else
		{
			Draw_param1(PARAM_RX_BUF3,n);
			if((param1>0)&&(param1<6))
			{
				reply_flag=1;
				retry_Register_num=param1;
				USART_inputflag=15;
			}
			else
				reply_flag=0;

		}				
		if(reply_flag>0)
		{
			sprintf(replyStr,"RETRYNUM=%d\r\n" ,retry_Register_num);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,ADDR,5)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=18;
		}
		else
		{
			Draw_param1(PARAM_RX_BUF3,n);
			if((param1>0)&&(param1<=4294967294u))
			{
				reply_flag=1;
				Address_ID=param1;
				USART_inputflag=17;
			}
			else
				reply_flag=0;

		}				
		if(reply_flag>0)
		{
			sprintf(replyStr,"ADDR=%u\r\n" ,Address_ID);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,LCD_U,4)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=20;
		}
		else
		{
			Draw_param2(PARAM_RX_BUF3,n);
			if(param1==2)
			{
				reply_flag=1;
				display_mode=ENABLE;
				lcd_time_min=param2;
				USART_inputflag=19;
			}
			else if(param1==1){
				reply_flag=1;
				display_mode=DISABLE;
				USART_inputflag=19;
			}
			else
				reply_flag=0;
		}				
		if(reply_flag>0)
		{
			if(display_mode==ENABLE)
				sprintf(replyStr,"LCD=ENABLE,%u\r\n",lcd_time_min);
			else
				sprintf(replyStr,"LCD=DISABLE,%u\r\n",lcd_time_min);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,CCS811,4)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=22;
		}
		else
		{
			Draw_param2(PARAM_RX_BUF3,n);
			if(param1==2)
			{
				reply_flag=1;
				VOC_State=ENABLE;
				VOC_collection_time_min=param2;
				USART_inputflag=21;
			}
			else if(param1==1){
				reply_flag=1;
				VOC_State=DISABLE;
				USART_inputflag=21;
			}
			else
				reply_flag=0;
		}				
		if(reply_flag>0)
		{
			if(VOC_State==ENABLE)
				sprintf(replyStr,"CCS=ENABLE,%u\r\n",VOC_collection_time_min);
			else
				sprintf(replyStr,"CCS=DISABLE,%u\r\n",VOC_collection_time_min);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,HCHO,5)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=24;
		}
		else
		{
			Draw_param2(PARAM_RX_BUF3,n);
			if(param1==2)
			{
				reply_flag=1;
				HCHO_State=ENABLE;
				HCHO_collection_time_min=param2;
				USART_inputflag=23;
			}
			else if(param1==1){
				reply_flag=1;
				HCHO_State=DISABLE;
				USART_inputflag=23;
			}
			else
				reply_flag=0;
		}				
		if(reply_flag>0)
		{
			if(HCHO_State==ENABLE)
				sprintf(replyStr,"HCHO=ENABLE,%u\r\n",HCHO_collection_time_min);
			else
				sprintf(replyStr,"HCHO=DISABLE,%u\r\n",HCHO_collection_time_min);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,COZ,4)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=26;
		}
		else
		{
			Draw_param2(PARAM_RX_BUF3,n);
			if(param1==2)
			{
				reply_flag=1;
				COZ_State=ENABLE;
				COZ_collection_time_min=param2;
				USART_inputflag=25;
			}
			else if(param1==1){
				reply_flag=1;
				COZ_State=DISABLE;
				USART_inputflag=25;
			}
			else
				reply_flag=0;
		}				
		if(reply_flag>0)
		{
			if(COZ_State==ENABLE)
				sprintf(replyStr,"COZ=ENABLE,%u\r\n",COZ_collection_time_min);
			else
				sprintf(replyStr,"COZ=DISABLE,%u\r\n",COZ_collection_time_min);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(Buffercmp((u8*)PARAM_RX_BUF2,WAITWIFI,9)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			reply_flag=2;
			USART_inputflag=28;
		}
		else
		{
			Draw_param1(PARAM_RX_BUF3,n);
			if((param1>=5)&&(param1<=13))
			{
				reply_flag=1;
				Wait_WIFI_ConnectTime=param1*10;
				USART_inputflag=7;
			}
			else
				reply_flag=0;
		}				
		if(reply_flag>0)
		{
			sprintf(replyStr,"WAITWIFI=%d\r\n" ,Wait_WIFI_ConnectTime/10);
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		} 
		else
		{
			sprintf(replyStr,"error\r\n" );
			delay_ms(2);//不加会卡死
			Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		}
	}
	else if(strcmp((char*)PARAM_RX_BUF2,(char*)RES_U)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			USART_inputflag=29;
			reply_flag=1;
//			printf("\r\n ?");
		}
		else
			reply_flag=0;
		if(reply_flag==1)
			sprintf(replyStr,"RES=OK\r\n");
		else
			sprintf(replyStr,"error\r\n");
		delay_ms(2);//不加会卡死
		Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		if(Config_By_WifiOrUart==REPLY_CONFIG_WIFI)
			delay_ms(300);
	}
		else if(strcmp((char*)PARAM_RX_BUF2,(char*)RESTORE)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			USART_inputflag=30;
			reply_flag=1;
//			printf("\r\n ?");
		}
		else
			reply_flag=0;
		if(reply_flag==1)
			sprintf(replyStr,"RESTORE=OK\r\n");
		else
			sprintf(replyStr,"error\r\n");
		delay_ms(2);//不加会卡死
		Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
		if(Config_By_WifiOrUart==REPLY_CONFIG_WIFI)
			delay_ms(300);
	}
		else if(strcmp((char*)PARAM_RX_BUF2,(char*)WIFISENDT)==0)
	{
		if(PARAM_RX_BUF3[0]=='?')
		{
			USART_inputflag=32;
			reply_flag=2;
		}
		else
		{
			reply_flag=1;
			Draw_param3(PARAM_RX_BUF3,n);
			WIFI_Send_TIME_Range[0]=((param1/1000)*10)+(param1%1000/100);
			WIFI_Send_TIME_Range[1]=((param1%100/10)*10)+(param1%10);
			
			WIFI_Send_TIME_Range[2]=((param2/1000)*10)+(param2%1000/100);
			WIFI_Send_TIME_Range[3]=((param2%100/10)*10)+(param2%10);
			
			printf("\r\nparam:%d %d %d %d",WIFI_Send_TIME_Range[0],WIFI_Send_TIME_Range[1],WIFI_Send_TIME_Range[2],WIFI_Send_TIME_Range[3]);
//			printf("\r\nparam:%u %u %u",param1,param2,param3);
			USART_inputflag=31;
		}
			
		if(reply_flag>0)
			sprintf(replyStr,"WIFISENDT=%04d,%04d\r\n",WIFI_Send_TIME_Range[0]*100+WIFI_Send_TIME_Range[1],WIFI_Send_TIME_Range[2]*100+WIFI_Send_TIME_Range[3]);
		else
			sprintf(replyStr,"error\r\n");
		delay_ms(2);//不加会卡死
		Reply_configure(Config_By_WifiOrUart,(uint8_t*)replyStr,strlen(replyStr));
	}
	
	
	return USART_inputflag;
}



void Reply_configure(u8 WifiOrUart,uint8_t *pData,uint16_t size)
{
	if(WifiOrUart==REPLY_CONFIG_WIFI)
	{
		WIFI_send(pData,size);
	}
	else if (WifiOrUart==REPLY_CONFIG_UART)
	{
		UART_send(pData,size);
	}
}

u8 Draw_param3(u8 *ptr,u8 size)
{
	u8 i;
	u8 num=0;
	char ptr1[10];
	char ptr2[10];
	char ptr3[10];
	u8 n1=0;
	u8 n2=0;
	u8 n3=0;
	u32 sumn1=0;
	u32 sumn2=0;
	u32 sumn3=0;
	u32 sum1=0;
		for(i=0;i<size;i++)
	{
		if(ptr[i]==',')
			num++;
		else
		{
			if(num==0)
			{
				ptr1[n1]=ptr[i];
				n1++;
//				printf("\r\n ptr1:%c %d ",ptr[i],n1);
			}
			if(num==1)
			{
				ptr2[n2]=ptr[i];
				n2++;
//				printf("\r\n ptr2:%c %d ",ptr[i],n2);
			}
			if(num==2)
			{
				ptr3[n3]=ptr[i];
				n3++;
//				printf("\r\n ptr3:%c %d ",ptr[i],n3);
			}
		}
	}
	
	ptr1[n1]='\0';
	ptr2[n2]='\0';
	ptr3[n3]='\0';
	
	for(i=0;i<n1;i++)
	{
//		printf("\r\n %d  %d  %d",sum,ptr1[i]-'0',n1-1-i);
		sum1=pow(10,n1-1-i);
		sumn1=sumn1+(ptr1[i]-'0')*sum1;//(ptr1[i]-'0')*(10^(n1-1-i));
//		printf("\r\n %c",ptr1[i]);
		
	}
	param1=sumn1;
//	printf("\r\nptr1:%d ",sumn1);
	for(i=0;i<n2;i++)
	{
//		printf("\r\n %d  %d  %d",sum,ptr2[i]-'0',n2-1-i);
		sum1=pow(10,n2-1-i);
		sumn2=sumn2+(ptr2[i]-'0')*sum1;//(ptr1[i]-'0')*(10^(n1-1-i));	
	}
	param2=sumn2;
//	printf("\r\nptr2:%d ",sumn2);
	for(i=0;i<n3;i++)
	{
//		printf("\r\n %d  %d  %d",sum,ptr3[i]-'0',n3-1-i);
		sum1=pow(10,n3-1-i);
		sumn3=sumn3+(ptr3[i]-'0')*sum1;//(ptr1[i]-'0')*(10^(n1-1-i));
//		printf("\r\n %c",ptr1[i]);
	}
	param3=sumn3;
//	printf("\r\nptr3:%d ",sumn3);
//	printf("\r\nptr:%d   %d   %d",*pr1,*pr2,*pr3);
	
	
return 1;
}
u8 Draw_param1(u8 *ptr,u8 size)
{
	u8 i;
	u32 sumn1=0;
	u32 sum1=0;
	for(i=0;i<size;i++)
	{
//		printf("\r\n %d  %d  %d",sum,ptr1[i]-'0',n1-1-i);
		sum1=(u32)pow(10,size-1-i);
		sumn1=sumn1+(u32)(ptr[i]-'0')*sum1;//(ptr1[i]-'0')*(10^(n1-1-i));
//		printf("\r\n %c",ptr1[i]);
		
	}
	param1=sumn1;

return 1;
}

u8 Draw_param2(u8 *ptr,u8 size)
{
	u8 i,j;
	u32 sumn1=0;
	u32 sum1=0;
	if((ptr[0]=='E')&&(ptr[1]=='N'))
	{
		param1=2;
//		printf("\r\nparam1= %u",param1);
	}
	else if((ptr[0]=='D')&&(ptr[1]=='I')&&(ptr[2]=='S'))
	{
		param1=1;
//		printf("\r\nparam1= %u",param1);
	}
	for(i=0;i<size;i++)
	{
		if(ptr[i]==',')
		{
			j=i;break;
		}
	}
//	printf("\r\n j= %u",j);
	for(i=j+1;i<size;i++)
	{
//		printf("\r\n %d  %d  %d",sum1,ptr[i]-'0',size-1-i);
		sum1=(u32)pow(10,size-1-i);
		sumn1=sumn1+(u32)(ptr[i]-'0')*sum1;//(ptr1[i]-'0')*(10^(n1-1-i));
//		printf("\r\n %c",ptr1[i]);
	}
	param2=sumn1;
//  printf("\r\n %u",param2);
	return 1;
}

