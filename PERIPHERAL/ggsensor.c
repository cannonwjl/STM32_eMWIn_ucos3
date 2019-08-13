#include "ggsensor.h"
#include "spi.h"
//#include "rtc.h"
#include "timer.h"
#include "eeprom.h"
#include "stdlib.h"
#include "stdio.h"
//#include "adc.h"
#include "usart.h"
#include "sx12xxUserfunc.h"
#include "sht3x.h"
#include "VEML6070.h"

#include "VEML6030.h"
#include "math.h"
#include "include.h"
#include "Platform_Configure.h"
#include "system_platform.h"

int16_t tem_temp;//温度有负数
uint16_t tem_humi,voltage_value;//传感器参数 --温度 湿度 电压
uint16_t Light;//传感器参数 --光照
uint16_t ultraviolet;//紫外
uint16_t tem_temp_display,tem_humi_display;//液晶显示 温湿度
uint16_t tvocv=0,eco2v=400;
float uwADCxConvertedValue;//计算得到的电压值
float formaldehyde;
uint16_t co2v=400;
uint16_t PM2_5,PM10;//传感器参数 --PM2.5  PM10
/*****此部分为FLASH读出的数据*********/
u32 timestamp_flash;//传感器参数 --时间戳
int16_t tem_temp_flash;
uint16_t tem_humi_flash,voltage_value_flash,formaldehyde_flash;//传感器参数 --温度 湿度 电压
uint16_t Light_flash;//传感器参数 --光照
uint16_t ultraviolet_flash;//紫外
uint16_t tvocv_flash,eco2v_flash=400;
u16 send_seqnum_1;
float tem_temp_displayf,tem_humi_displayf;
u32 timestampf;
uint16_t co2v_flash=400;
uint16_t PM2_5_flash,PM10_flash;//传感器参数 --PM2.5  PM10


uint16_t Light_save;
u16 packet_sequence_flash=0;//存FLASH的包序
u16 packet_sequence_send=0;//存FLASH的包序

u32 last_collect_timestamp;
_EEPROM_pos EEPROM_pos_buf;
_EEPROM_param EEPROM_param_buf;
_FLASH_data FLASH_data_buf;
_EEPROM_COZdata EEPROM_CO2_data;
_EEPROM_LIGHTdata EEPROM_LIGHT_data;
const u32 eeprom_paramflag=0x0127575A;// 0x2a079acf(相同) EEPROM中存参数的标志位  改变该值可以重新写入默认参数
const u32 eeprom_posflag=0xd6c8a873;// 0xd5f7a473 EEPROM中存数据发送和接收地址的标志位  改变该值可以重新写入默认地址

const u32 eeprom_pos_permit_flag=0;//为1时，无论eeprom_posflag是否相同 都不更改pos  只有为0 且 eeprom_posflag不同时才能更改参数
const u32 eeprom_param_permit_flag=0;//相当于为1时  对参数形成一个保护

const u32 Start_configmode_flag=0x55667788;
const u32 Start_normalmode_flag=0x11223344;
const u32 flash_dataflag=0x20a02554;//FLASH存数据的标志位 

u8 FLASH_first_write_data=0;
const u8 POS_WR_Addr=0;//存EEPROM 数据的写和发送的地址
const u8 PARAM_WR_Addr=30;//存EEPROM 参数地址
const u8 Start_mode_Addr=20;//存启动模式标志位
const u16 CO2_WR_Addr=330;//存CO2值
const u16 LIGHT_WR_Addr=335;//存LIGHT值

extern u8 RF_is_rece;//RF是否接收

//u32 timestamp;//传感器参数 --时间戳
uint8_t RTC_CurrentYear,RTC_CurrentDate,RTC_CurrentMonth,RTC_CurrentHour,RTC_CurrentMinute,RTC_CurrentSecond;//RTC_TimeShow();之后的系统时间
uint8_t CurrentYear,CurrentDate,CurrentMonth,CurrentHour,CurrentMinute,CurrentSecond;
u32 SYSTimestamp;//传感器参数 --系统时间戳

extern enum ADC_STATE ADC_state;
extern enum VEML6070_STATE  VEML6070_state;
 unsigned int eseqno;
//static uint32_t  timertick;///perioid
//static uint8_t poweroff_time;

enum M25P80_STATE M25P80_state;//M25P80的状态 休眠时不能读写
enum M25P80_RUNSTATE M25P80_runstate;//M25P80的运行状态  闲和忙

//enum RF_STATE RF_S_state;
enum PERIPHER_STATE PERIPHER_state=PERIPHER_S_ON;

u8 clock_adjust=0;//时钟校准
unsigned long time2[3]={0};//测试时间
uint8_t rLight[4];

int flag_ggsensor_et;
u8 flag_LCDDisp;
u8 RF_STATE;
u8 RF_Rece_Buff[200];//SX1278 收到数
u16 RF_Rece_Lenth;

u32 Last_SendTime;//上次一次发送时间戳
u32 Next_SendTime;//下一次发送时间戳

u32 Last_CollectTime;//上次一次传感器采集时间戳
u32 Next_CollectTime;//下一次传感器采集时间戳

u32 Last_SendpacketTime;//上一次发包时间
u32 Next_SendpacketTime;//下一次发包时间
u32 Last_LcdrefreshTime;//上一次液晶刷新时间
u32 Next_LcdrefreshTime;//下一次液晶刷新时间

u32 Last_LcdCollectTime;
u32 Next_LcdCollectTime;

u32 Last_HchoCollectTime;
u32 Next_HchoCollectTime;
u8 HCHO_start_collect_flag=0;
u32 HCHO_startCollect_time=0;

u32 Last_CozCollectTime;
u32 Next_CozCollectTime;
u8 COZ_start_collect_flag=0;
u32 COZ_startCollect_time=0;

u32 Last_VocCollectTime;
u32 Next_VocCollectTime;
u8 VOC_start_collect_flag=0;
u32 VOC_startCollect_time=0;

u8 check_wifi_send_or_not_flag;//WIFI是否工作标志位
u8 LCD_DISPLAY_SWITCH_FLAG=0;


int flag_receive;
int flag_receive2;
int sleepCount = 0;


u32 update_time;
u8 WIFI_check_buf[100];
u16 rece_packet_seqno;


extern u8 ADC_collect_flag;//0不采集  1：需要采集
//extern u8 RF_STATE;
uint8_to_uint32 u8to32data;

SensorMsgV2 smg2;
SensorEchoV2 se2;
SensorMsgV2NoCCS811 smg22;

extern u8 COZ_start_collect_flag;
extern u8 HCHO_start_collect_flag;
u16 Wifi_echopack_num;//wifi 发送的注册包包序
u16 Wifi_datapack_num;//WIFI发送的数据包包序
u8 Is_register_flag;//网关注册成功为1 不成功为0

extern u8 sink_id;//重要 发给那个网关的 注册之后获取要记下
extern u8 sink_id2[4];//重要 发给那个网关的 注册之后要记下
extern u16 rece_seqnum_V2;//新版协议收到包序
extern u16 send_seqnum_V2;
u8 SysStart_mode_flag=0;
u16 EquipmentType;//设备型号 由参数算出来
/*-------------------------------可修改重要参数-----------------------------------
*
*修改之后 修改 eeprom_paramflag 或 eeprom_posflag 两个标志位才能重新写入
*/
	// FLASH参数地址  eeprom_posflag
	u8 save_next_addr=0;				//该页存放的地址
	u16 save_next_addr_page=0;		//扇区第多少页
	u8 save_next_addr_sect=0;   //存放在第几个扇区

	u8 send_next_addr=0;			//该页存放的下一个要发送的地址
	u16 send_next_addr_page=0;	//要发送的数据在第几个页
	u8 send_next_addr_sect=0;	//要发送的数据在第几个扇区
  u8  FLASH_full_flag=0;// 

 // 节点自身参数  eeprom_paramflag
	u32 Address_ID=0x0A080001;//节点自身的地址
	u16  Rf_fequence=450;//433参数
	u16  collection_time_min=10;		//数据采集周期 min
	u16  send_time_min=10;		//数据发送周期 min
	
	u16 GettimeEcho_time_Hour=100; //== GettimeEcho_time/3600
	u16 Re_Registration_time_min=10;//断网之后需要重新注册的  发送包的周期 重要参数==send_time_min

  u32 NB_first_down_waittime=1000;
	u8 NB_echo_sec=25;//NB注册等待时间   单位S
	u32 NB_seng_time=6000;//  单位ms  NB发送时等待回包时间

	u8 WIFI_or_RF_SendMode=RF_SEND;//发送模式选择 必须写入flash
	u8 sta[70]="Ebyte,2,E61TTL1W";
	u8 staip[100]="DHCP,192.168.1.1,255.255.255.0,192.168.1.1,192.168.1.1";
	u8 sock[70]="TCP,SERVER,192.168.1.2,8887,8889";
	
	u8 retry_Register_num=2;//注册不成功 重发注册包的次数 
	
	u8  display_mode=ENABLE;//液晶屏打开|关闭  1|0
	u32 lcd_time=60;//液晶屏刷新周期
	u16 lcd_time_min=1;//液晶屏刷新周期
	
	u8 VOC_State=DISABLE; //VOC是否开启
	u32 VOC_collection_time=100;
	u16 VOC_collection_time_min=15;
	
	
	u8 HCHO_State= DISABLE; //HCHO是否开启
	u32 HCHO_collection_time=100;
	u16 HCHO_collection_time_min=15;
	
	u8 COZ_State= DISABLE; //COZ是否开启
	u32 COZ_collection_time=100;
	u16 COZ_collection_time_min=15;
	
	u8 Wait_WIFI_ConnectTime=70;//等待WIFI连上服务器的时间 （WIFI启动时间） *100ms
	
	float minimum_operate_voltage=2.8;//最低的工作电压 

  u16 Config_timeS=50; //配置阶段超时时间

	u32 collection_time=10;		//数据采集周期 
	u32 send_time=60;		//数据采集周期 
	
	u32 GettimeEcho_time=300;  //新版协议获取时间注册的周期  
	u32 Re_Registration_time=300;//断网之后需要重新注册的  发送包的周期 重要参数
	
	u8 retry_senddata_num=2;//发送数据不成功 重新发送的次数 == retry_Register_num
	u8 WIFI_RETRY_SEC=2;//WIFI连接服务器重发的次数 ==retry_Register_num
	u32 RetryRegisterTime=120;//上电第一次注册失败(RTC 没有时间) 重新注册时间 (12s) ==断网之后的注册周期 Re_Registration_time
	u32 HARDWARE_ERROR_sleeptime=900;//(S)进入HARDWARE_ERROR休眠的时间 时间到了检测电压  ==collection_time
	
	uint8_t WIFI_Send_TIME_Range[4]={8,2,06,04};//wifi在某个时间工作的参数，全为0时不使用该功能
	
//	uint8_t RegionAddr[2]={0x06,0x06};
//	u8  node_id=0x0f;	    			//节点编号
/*-------------------------------可修改重要参数-----------------------------------*/
 extern uint32_t LoRa_FREQENCY_R;
	uint8_t HCHO_NeedCollectTime=150;//甲醛采集需要的时间
	uint8_t COZ_NeedCollectTime=80;//CO2采集需要的时间
	u16 VOC_collect_need_time=20;//重要参数
	
//		u16 ccs811_collect_need_time=17;//重要参数
//	uint8_t HCHO_NeedCollectTime=150;//甲醛采集需要的时间
//	uint8_t COZ_NeedCollectTime=30;//CO2采集需要的时间
	
//	uint8_t HCHO_NeedCollectTime=18;//甲醛采集需要的时间
//	uint8_t COZ_NeedCollectTime=18;//CO2采集需要的时间
	
	#ifdef EZ267_WEATHER_STATION
		u8 NB_node_or_weather_mode=WEATHER_STATION;//如果是气象站则设置为WEATHER_STATION  其余节点一律为NORMAL_NODE
	#else
		u8 NB_node_or_weather_mode=NORMAL_NODE;
	#endif
	
	
	u8 Network_Flag=0;//0无网 1有网
 u8 Network_counter;//没网的次数
 
	
  uint8_t Data[14];
	u32 Send_Addr;
  u32 Save_Addr;
//	u8 Lap_Falg;
	uint16_t Light_Last;

u8  sendflag=0;//是否发送成功

enum SYS_STAT sys_stat;

static uint8_t SerialframeCRC[2]={0,0};
static uint8_t SerialframeTail[1]={0x7e};

MsgHeaderV2 Msgheader;
static uint8_t SerialframeHead2[2]={0x7e,0x46};
//static uint8_t SerialframeLSA2[2]={0,0};
static uint8_t SerialframeDA_lora2[4]={0xff,0xff,0xff,0xff};
static uint8_t SerialframeDA_Wifi2[4]={0x00,0x00,0x00,0x00};
static uint8_t MessageType2_10[1]={0x10};// type 节点注册
static uint8_t MessageType2_11[1]={0x11};// type 节点数据
static uint8_t MessageType2_15[1]={0x15};// type 节点数据
static uint8_t Serialreserve2[2]={0,0};

static uint8_t SerialpayloadLength2[1]={0};

static uint8_t SerialframeCRC2[2]={0,0};
static uint8_t SerialframeTail2[1]={0x7e};


static uint8_t LowRF_frame_buf[58];
uint8_t Gateway_echodata_buf[58];
uint8_t Gateway_echodata_len;

uint8_t Gateway_sensordata_buf[58];
uint8_t Gateway_sensordata_len;

void GateWay_Register(u8 MAX_count);//形参为最大注册次数	
void GateWay_Sensor(u8 Max_num);//等待接收传感器数据的回包

//void basic_init(void) //update_20171013
//{
//	
//	MX_USART2_UART_Init();
//	SystemPower_Config();

//	SHT25_GPIO_Init();	//I2C初始化
//	PWEN_GPIO_Init();
//	M25P80_GPIO_Init();

//	SENSE_PWEN_ON;
//  LED_GPIO_init();
//  MX_RTC_Init();
//  MX_ADC_Init();
//	M25P80_PWEN_ON;	

//}
//void parameter_init(void)
//{
////	Get_CustomChipID();
//	Init_pos_eeprom();
//	Init_param_eeprom();
//	Read_param_eeprom();
//	
//	Read_Data_from_eeprom_last();//读上次CO2的值

////	sensor_time();
//	Next_HchoCollectTime=SYSTimestamp+20;
//	Next_CozCollectTime=SYSTimestamp+20;
//	Next_VocCollectTime=SYSTimestamp+20;
//	Next_SendTime=SYSTimestamp;
//	Next_CollectTime=SYSTimestamp;
//	last_collect_timestamp=SYSTimestamp-7;
//}
void parameter_init(void)
{
	Get_CustomChipID();
	Init_pos_eeprom();
	Init_param_eeprom();
	Read_param_eeprom();
	
//	Read_Data_from_eeprom_last();//读上次CO2的值

//	sensor_time();
//	Next_HchoCollectTime=SYSTimestamp+20;
//	Next_CozCollectTime=SYSTimestamp+20;
//	Next_VocCollectTime=SYSTimestamp+20;
//	Next_SendTime=SYSTimestamp;
//	Next_CollectTime=SYSTimestamp;
//	last_collect_timestamp=SYSTimestamp-7;
}
u8 sensor_init(void)  //update_20171013
{
	SHT3X_Init(0x44); 
//	M25P80_PWEN_ON;	
//	SpiInit();	
	//M25P80_SLEEP();
	
	#ifdef EZ265
	if(WIFI_or_RF_SendMode==RF_SEND)
	{
		SX1276InitIo();
		RF_Init();
		SX1278Sleep();//用wifi也得让SX1278 休眠
		printf(" RFInit ");
	}	
	if(WIFI_or_RF_SendMode==WIFI_SEND)
	{
		WIFI_GPIO_Init();
		MX_USART4_UART_Init();
		WIFI_PWEN_ON;
		printf(" WIFIInit ");
	}
	#endif
	#ifdef EZ267
		USART4_5_FLAG=4;
		NB_UART_Init();
		if(NB_node_or_weather_mode==WEATHER_STATION)
		{
			WeatherSTA_UART_Init();
		}
			
	#endif


//		printf("\r\n usart");
	delay_ms(10);//下载用 一定要有	

//	while(1);
	//LED1=0;delay_ms(1000);LED1=1;// 一定要有	 一定要在write_startmode_normal_eeprom();之前
	
	return 1;
}



void assemble_echo_pkt2(SensorEchoV2 *septr)//注册网关 加载Payload V2版协议 ^^^^^^
{
	u8 Voltageint;
	Voltageint=uwADCxConvertedValue*100;
  septr->voltage[0] =Voltageint/100;
	septr->voltage[1] =Voltageint%100;
	
	septr->hop=0;

	septr->reserve[0]=(uint8_t)(EquipmentType>>8);
	septr->reserve[1]=(uint8_t)EquipmentType;
	
	++eseqno;// xunhuan number

	septr->packetnum[0]=(uint8_t)(eseqno>>8);
	septr->packetnum[1]=(uint8_t)(eseqno);
}

void send_se2(SensorEchoV2 septr1)//发送注册网关的包  加载报头  V2版协议
{
	uint16_t framelen,crc;
	uint8_t datalen;
	SerialpayloadLength2[0] = sizeof(SensorEchoV2)+4;
	datalen = SerialpayloadLength2[0]-4;

	
	memcpy(LowRF_frame_buf,SerialframeHead2,2);
	
//	memcpy(LowRF_frame_buf+2,(void*)(Address_ID>>24),1);
//	memcpy(LowRF_frame_buf+3,(void*)(Address_ID>>16),1);
//	memcpy(LowRF_frame_buf+4,(void*)(Address_ID>>8),1);
//	memcpy(LowRF_frame_buf+5,(void*)(Address_ID),1);
	LowRF_frame_buf[2]=(Address_ID>>24);
	LowRF_frame_buf[3]=(Address_ID>>16);
	LowRF_frame_buf[4]=(Address_ID>>8);
	LowRF_frame_buf[5]=(Address_ID);
//	memcpy(LowRF_frame_buf+2,(void *)&Address_ID,4);//区域
//	memcpy(LowRF_frame_buf+4,SerialframeLSA2,1);//00
//	memcpy(LowRF_frame_buf+5,&node_id,1);//节点地址
	
	memcpy(LowRF_frame_buf+6,SerialframeDA_lora2,4);//目的地址
	memcpy(LowRF_frame_buf+10,MessageType2_10,1);//消息类型
	memcpy(LowRF_frame_buf+11,Serialreserve2,1);//保留字节	
	memcpy(LowRF_frame_buf+12,SerialpayloadLength2,1);//payload长度
	
//	memcpy(LowRF_frame_buf+13,(void *)&Address_ID,4);//区域
	LowRF_frame_buf[13]=(Address_ID>>24);
	LowRF_frame_buf[14]=(Address_ID>>16);
	LowRF_frame_buf[15]=(Address_ID>>8);
	LowRF_frame_buf[16]=(Address_ID);
	
	memcpy(LowRF_frame_buf+17,&septr1,datalen);//payload
	
	crc=crc_cal(LowRF_frame_buf+1, 16+datalen);
	SerialframeCRC2[0]=(uint8_t)(crc>>8);
  SerialframeCRC2[1]=(uint8_t)crc;
	
	memcpy(LowRF_frame_buf+17+datalen,SerialframeCRC2,2);
  memcpy(LowRF_frame_buf+17+datalen+2,SerialframeTail2,1);
	framelen=17+datalen+3;

  Gateway_echodata_len= hdlc_encode(LowRF_frame_buf, Gateway_echodata_buf, framelen);
		
	if(Gateway_sensordata_len>60)
	{
		Gateway_sensordata_len=50;
	}
	
//  printf(" send echo to gateway\r\n");

//	for(int k=0; k<Gateway_echodata_len; k++ )
//		printf(" %02X", Gateway_echodata_buf[k]);


//hdlcframelen= hdlc_encode(LowRF_frame_buf, hdlc_frame_buf, framelen);
//  printf(" send echo to gateway\r\n");

//	for(int k=0; k<hdlcframelen; k++ )
//		printf(" %02X", hdlc_frame_buf[k]);
	
//	rfrece_flag2=RF_Send_Rouse(LowRF_frame_buf,hdlcframelen);

	
//	rfrece_flag2=RF_Send_Rouse(LowRF_frame_buf,hdlcframelen);
	
//	if(rfrece_flag2==RF_TIMEOUT)
//	{
//		printf("\r\n 88888888888888888888888888888 SX1278_RESET  8888888888888888888888888888888888");
//		Reset_sx1276();
//	}
//	else
//	{
//		printf(" RF_StartRX");
//		RF_StartRX();//配置成接受 
//		rfrece_flag1= RF_Receive_general(RF_Rece_Buff,&RF_Rece_Lenth);
//	}
//	if(rfrece_flag1==RF_SUCCESS)
//	{
//		printf(" Read_RSSI=%f",Read_RSSI()); 
//		send_event(EVENT_RF_PACK_IN,1);
////		printf("\r rece: ");
////		for(i=0;i < RF_Rece_Lenth;i++)
////		{
////			printf(" %02X",RF_Rece_Buff[i]);
////		}
////		printf("\n");
//		
//	}

	
}


void  WIFI_send_echo2(void)
{
	SensorEchoV2_All msg1;
	u16 len,crc,i;
	u8 GPRS_tx_buf[40];
	u8* pbuf=(u8*)&msg1;
//Wifi_echopack_num++;
	++eseqno;
	msg1.frame[0] = 0x7e;
	msg1.frame[1] = 0x46;
	msg1.lsadrr[0] =Address_ID>>24;
	msg1.lsadrr[1] =Address_ID>>16;
	msg1.lsadrr[2] =Address_ID>>8;
	msg1.lsadrr[3] =Address_ID;
	
	msg1.dtadrr[0]=0; 
	msg1.dtadrr[1]=0; 
	msg1.dtadrr[2]=0; 
	msg1.dtadrr[3]=0; 
	
	msg1.payloadlen=0x0B;
	msg1.subtype =0x10;
  msg1.Reserve =0;
	msg1.seadrr[0] =Address_ID>>24;
	msg1.seadrr[1] =Address_ID>>16;
	msg1.seadrr[2] =Address_ID>>8;
	msg1.seadrr[3] =Address_ID;
	
	msg1.data.voltage [0]=(u8)(voltage_value>>8);
	msg1.data.voltage [1]=(u8)voltage_value;
	msg1.data.hop=0;
	msg1.data.reserve[0]=(uint8_t)(EquipmentType>>8);
	msg1.data.reserve[1]=(uint8_t)(EquipmentType);
	msg1.data.packetnum[0]=(u8)(eseqno>>8); 
	msg1.data.packetnum[1]=(u8)eseqno; 
	
	crc=crc_cal(pbuf+1,sizeof(SensorEchoV2_All)-4); //0x7e is not included;
	msg1.crc[0]=(u8)(crc>>8);
	msg1.crc[1]=(u8)(crc);
	msg1.end=0x7e;
	
	printf(" CRC=%04X",crc);
	
	len=hdlc_encode(pbuf,GPRS_tx_buf,sizeof(SensorEchoV2_All));
	
	WIFI_send(GPRS_tx_buf, len);
	 for(i=0;i<len;i++)
	printf(" %02X",GPRS_tx_buf[i]);
	printf("\r\n  WIFI send  data echo\r\n");
	return;
	
}


void WIFI_send_data2(void)
{
//	GatewayMsgV2 msg1;
//	GatewayMsgV2NoCCS msg11;
//	u16 len,crc1;
//	u8 rfrece_flag1;
	u8 data[60];
	uint16_t framelen,crc;
	uint8_t datalen;
	u8 seq[2];
	data[0] = (Address_ID>>24);
	data[1] = (Address_ID>>16);
	data[2] = (Address_ID>>8);
	data[3] = (Address_ID);
	data[4] = 0;//Hop
	
	data[5]=(u8)(timestamp_flash>>24);	
	data[6]=(u8)(timestamp_flash>>16);
	data[7]=(u8)(timestamp_flash>>8);
	data[8]=(u8)(timestamp_flash);
		
	data[9]=0x81;
	data[10]=(u8)(voltage_value_flash>>8);
	data[11]=(u8)(voltage_value_flash);
	
	data[12]=0x82; 
	data[13]=tem_temp_flash>>8;
	data[14]=tem_temp_flash;
	
	data[15]=0x83; 
	data[16]=(u8)(tem_humi_flash>>8);
	data[17]=(u8)(tem_humi_flash);

	data[18]=0x84; 
	data[19]=(u8)(Light_flash>>8);
	data[20]=(u8)(Light_flash);
	
	data[21]=0x85; 
	data[22]=(u8)(ultraviolet_flash>>8);
	data[23]=(u8)(ultraviolet_flash);
	datalen=24;
	if(VOC_State==ENABLE)
	{
		data[datalen++]=0x86; 
		data[datalen++]=(u8)(tvocv_flash>>8);
		data[datalen++]=(u8)(tvocv_flash);
		
//		data[datalen++]=0x87; 
//		data[datalen++]=(u8)(eco2v_flash>>8);
//		data[datalen++]=(u8)(eco2v_flash);	
	}

	if(HCHO_State==ENABLE)
	{
		data[datalen++]=0x88; 
		data[datalen++]=(u8)(formaldehyde_flash>>8);
		data[datalen++]=(u8)(formaldehyde_flash);
	}
	if(COZ_State==ENABLE)
	{	
		data[datalen++]=0x89; 
		data[datalen++]=(u8)(co2v_flash>>8);
		data[datalen++]=(u8)(co2v_flash);
	}
	SerialpayloadLength2[0]=datalen+6;//2字节包序

	memcpy(LowRF_frame_buf,SerialframeHead2,2);
	LowRF_frame_buf[2]=(Address_ID>>24);
	LowRF_frame_buf[3]=(Address_ID>>16);
	LowRF_frame_buf[4]=(Address_ID>>8);
	LowRF_frame_buf[5]=(Address_ID);
	
	
	memcpy(LowRF_frame_buf+6,SerialframeDA_Wifi2,4);//目的地址
	memcpy(LowRF_frame_buf+10,MessageType2_15,1);//消息类型
	memcpy(LowRF_frame_buf+11,Serialreserve2,1);//保留字节	
	memcpy(LowRF_frame_buf+12,SerialpayloadLength2,1);//payload长度
	memcpy(LowRF_frame_buf+13,Serialreserve2,2);//网关电压 模拟网关包发0即可
	
	memcpy(LowRF_frame_buf+15,&data,datalen); 
	
//	memcpy(LowRF_frame_buf+datalen+13,&packet_sequence_send,2); 
printf("\r\n WIFI packet_sequence_send=%d",packet_sequence_send);
	seq[0]=packet_sequence_send>>8;
	seq[1]=packet_sequence_send;
	memcpy(LowRF_frame_buf+datalen+15,&seq,2); 
	memcpy(LowRF_frame_buf+datalen+17,&seq,2);
	crc=crc_cal(LowRF_frame_buf+1, 18+datalen);
	SerialframeCRC[0]=(uint8_t)(crc>>8);
  SerialframeCRC[1]=(uint8_t)crc;
	
	memcpy(LowRF_frame_buf+19+datalen,SerialframeCRC,2);
  memcpy(LowRF_frame_buf+21+datalen,SerialframeTail,1);
	
	framelen=22+datalen;
//	printf("hdlc_encode and send: ");
//	while(1);	
	
  Gateway_sensordata_len= hdlc_encode(LowRF_frame_buf, Gateway_sensordata_buf, framelen);
//	for(i=0;i < hdlcframelen;i++)
//	{
//		printf(" %02X",hdlc_frame_buf[i]);
//	}
//	WIFI_send(hdlc_frame_buf,hdlcframelen);

}
void NB_send_data_weather(void)
{
	int k;
	u8 data[60];
	uint16_t framelen,crc;
	uint8_t datalen;
	u8 seq[2];
	data[0] = (Address_ID>>24);
	data[1] = (Address_ID>>16);
	data[2] = (Address_ID>>8);
	data[3] = (Address_ID);
	data[4] = 0;//Hop
	
	data[5]=(u8)(timestamp_flash>>24);	
	data[6]=(u8)(timestamp_flash>>16);
	data[7]=(u8)(timestamp_flash>>8);
	data[8]=(u8)(timestamp_flash);
		
	data[9]=0x81;
	data[10]=(u8)(voltage_value_flash>>8);
	data[11]=(u8)(voltage_value_flash);
	
	data[12]=0x82; 
	data[13]=tem_temp_flash>>8;
	data[14]=tem_temp_flash;
	
	data[15]=0x83; 
	data[16]=(u8)(tem_humi_flash>>8);
	data[17]=(u8)(tem_humi_flash);

	data[18]=0x84; 
	data[19]=(u8)(Light_flash>>8);
	data[20]=(u8)(Light_flash);
	
	data[21]=0x85; 
	data[22]=(u8)(ultraviolet_flash>>8);
	data[23]=(u8)(ultraviolet_flash);
	datalen=24;
//	if(VOC_State==ENABLE)
//	{
//		data[datalen++]=0x86; 
//		data[datalen++]=(u8)(PM2_5_flash>>8);
//		data[datalen++]=(u8)(PM2_5_flash);
//		
//		data[datalen++]=0x87; 
//		data[datalen++]=(u8)(PM10_flash>>8);
//		data[datalen++]=(u8)(PM10_flash);	
//	}

//	if(HCHO_State==ENABLE)
//	{
//		data[datalen++]=0x88; 
//		data[datalen++]=(u8)(formaldehyde_flash>>8);
//		data[datalen++]=(u8)(formaldehyde_flash);
//	}
//	if(COZ_State==ENABLE)
//	{	
//		data[datalen++]=0x89; 
//		data[datalen++]=(u8)(co2v_flash>>8);
//		data[datalen++]=(u8)(co2v_flash);
//	}

		data[datalen++]=0x8A; 
		data[datalen++]=(u8)(PM2_5_flash>>8);
		data[datalen++]=(u8)(PM2_5_flash);

		data[datalen++]=0x8B; 
		data[datalen++]=(u8)(PM10_flash>>8);
		data[datalen++]=(u8)(PM10_flash);

	SerialpayloadLength2[0]=datalen+6;//2字节包序

	memcpy(LowRF_frame_buf,SerialframeHead2,2);
	LowRF_frame_buf[2]=(Address_ID>>24);
	LowRF_frame_buf[3]=(Address_ID>>16);
	LowRF_frame_buf[4]=(Address_ID>>8);
	LowRF_frame_buf[5]=(Address_ID);
	
	
	memcpy(LowRF_frame_buf+6,SerialframeDA_Wifi2,4);//目的地址
	memcpy(LowRF_frame_buf+10,MessageType2_15,1);//消息类型
	memcpy(LowRF_frame_buf+11,Serialreserve2,1);//保留字节	
	memcpy(LowRF_frame_buf+12,SerialpayloadLength2,1);//payload长度
	memcpy(LowRF_frame_buf+13,Serialreserve2,2);//网关电压 模拟网关包发0即可
	
	memcpy(LowRF_frame_buf+15,&data,datalen); 
	
//	memcpy(LowRF_frame_buf+datalen+13,&packet_sequence_send,2); 
printf("\r\n NB packet_sequence_send=%d",packet_sequence_send);
	seq[0]=packet_sequence_send>>8;
	seq[1]=packet_sequence_send;
	memcpy(LowRF_frame_buf+datalen+15,&seq,2); 
	memcpy(LowRF_frame_buf+datalen+17,&seq,2);
	crc=crc_cal(LowRF_frame_buf+1, 18+datalen);
	SerialframeCRC[0]=(uint8_t)(crc>>8);
  SerialframeCRC[1]=(uint8_t)crc;
	
	memcpy(LowRF_frame_buf+19+datalen,SerialframeCRC,2);
  memcpy(LowRF_frame_buf+21+datalen,SerialframeTail,1);
	
	framelen=22+datalen;
//	printf("hdlc_encode and send: ");
//	while(1);	
	
  Gateway_sensordata_len= hdlc_encode(LowRF_frame_buf, Gateway_sensordata_buf, framelen);
//	for(i=0;i < hdlcframelen;i++)
//	{
//		printf(" %02X",hdlc_frame_buf[i]);
//	}
//	WIFI_send(hdlc_frame_buf,hdlcframelen);
	for( k=0; k<Gateway_sensordata_len; k++ )
			printf(" %02X", Gateway_sensordata_buf[k]);
			
			
		
		
//				u8 data[60];
//	uint16_t framelen,crc;
//	uint8_t datalen;
//	u8 seq[2];
//	data[0] = (Address_ID>>24);
//	data[1] = (Address_ID>>16);
//	data[2] = (Address_ID>>8);
//	data[3] = (Address_ID);
//	data[4] = 0;//Hop
//	
//	data[5]=(u8)(timestamp_flash>>24);	
//	data[6]=(u8)(timestamp_flash>>16);
//	data[7]=(u8)(timestamp_flash>>8);
//	data[8]=(u8)(timestamp_flash);
//		
//	data[9]=0x81;
//	data[10]=(u8)(voltage_value_flash>>8);
//	data[11]=(u8)(voltage_value_flash);
//	
//	data[12]=0x82; 
//	data[13]=tem_temp_flash>>8;
//	data[14]=tem_temp_flash;
//	
//	data[15]=0x83; 
//	data[16]=(u8)(tem_humi_flash>>8);
//	data[17]=(u8)(tem_humi_flash);

//	data[18]=0x84; 
//	data[19]=(u8)(Light_flash>>8);
//	data[20]=(u8)(Light_flash);
//	
//	data[21]=0x85; 
//	data[22]=(u8)(PM2_5_flash>>8);
//	data[23]=(u8)(PM2_5_flash);
//	datalen=24;
//	SerialpayloadLength2[0]=datalen+6;//2字节包序
//	memcpy(LowRF_frame_buf,SerialframeHead2,2);
//	LowRF_frame_buf[2]=(Address_ID>>24);
//	LowRF_frame_buf[3]=(Address_ID>>16);
//	LowRF_frame_buf[4]=(Address_ID>>8);
//	LowRF_frame_buf[5]=(Address_ID);
//	
//	memcpy(LowRF_frame_buf+6,SerialframeDA_Wifi2,4);//目的地址
//	memcpy(LowRF_frame_buf+10,MessageType2_15,1);//消息类型
//	memcpy(LowRF_frame_buf+11,Serialreserve2,1);//保留字节	
//	memcpy(LowRF_frame_buf+12,SerialpayloadLength2,1);//payload长度
//	memcpy(LowRF_frame_buf+13,Serialreserve2,2);//网关电压 模拟网关包发0即可
//	
//	memcpy(LowRF_frame_buf+15,&data,datalen); 

//	printf("\r\n NB packet_sequence_send=%d",packet_sequence_send);
//	seq[0]=packet_sequence_send>>8;
//	seq[1]=packet_sequence_send;
//	memcpy(LowRF_frame_buf+datalen+15,&seq,2); 
//	memcpy(LowRF_frame_buf+datalen+17,&seq,2);
//	crc=crc_cal(LowRF_frame_buf+1, 18+datalen);
//	SerialframeCRC[0]=(uint8_t)(crc>>8);
//  SerialframeCRC[1]=(uint8_t)crc;
//	memcpy(LowRF_frame_buf+19+datalen,SerialframeCRC,2);
//  memcpy(LowRF_frame_buf+21+datalen,SerialframeTail,1);
//	framelen=22+datalen;
//	
//  Gateway_sensordata_len= hdlc_encode(LowRF_frame_buf, Gateway_sensordata_buf, framelen);
//			
					
			
}



void All_sensor_collect(void)//温度湿度 电压 时间等
{ 
//	float ultraviolet_f;
//	uint16_t ultraviolet1;
//	if( 1)
	if( abs(last_collect_timestamp-SYSTimestamp)>5)
	{
//			adc_collect1();
//			I2C_PWEN_Init();
//			
//			delay_ms(1);
//			enable_sensor_veml6030();
//			initialize_VEML6070();
//			delay_ms(310);//打开所有传感器
//			sensor_time();
//			timestampf=SYSTimestamp;
////			last_light=Read_Data_from_eeprom_lastLight();
//			Light=veml6030_read();
//			
//			SHT3X_getHumidityTemperature();
//			tem_temp=(int16_t)(tem_temp_displayf*100);
//			tem_humi=(uint16_t)(tem_humi_displayf*100);
//			printf("\r T=%02X H=%02X",tem_temp,tem_humi);
//			tem_temp=(tem_temp/10)*10;//取小数后一位
//			tem_humi=(tem_humi/10)*10;
//			printf("\r T=%02X H=%02X",tem_temp,tem_humi);
////			voltage_value=uwADCxConvertedValue*100;
//			ultraviolet=read_uvs_step();
//				
//			printf("Vol=%fV,light=%d,T=%f,H=%f,ult=%d,time,%d\n",uwADCxConvertedValue,Light,tem_temp_displayf,tem_humi_displayf,ultraviolet,SYSTimestamp);
////			SENSE_PWEN_OFF;
////			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
////			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);

//		last_collect_timestamp=SYSTimestamp;
	}
	else
	{
		printf("---just  collect--");
	}
	

	
}

void sensor_disp(void)//液晶屏显示 采集后显示
{
	All_sensor_collect();
}
//void Srand(void)
//{
//	srand((tem_temp%10)*3 + (tem_humi%10)*3 + Light%10 + (u8)Address_ID + (voltage_value%10)+(u8)(Address_ID>>8)+(u8)(Address_ID>>16));//随机数的种子
//}

void Init_pos_eeprom(void)  //从STM32 内部 EEPROM 初始化FLASH写数据和发送数据的地址  //update_20171013
{
	u8 tetsbuf[40];
//    u8 j;
//    u32 i[4];
//	printf("\r\n Init_pos_eeprom \r\n");
//	u8 testbuf[200];
	u8 write_num;
	u32 read_startmode_flag;
	EEPROM_Read(0,tetsbuf,sizeof(_EEPROM_pos)+Start_mode_Addr+4);
	memcpy((u8*)&read_startmode_flag,tetsbuf+Start_mode_Addr,4);
//    printf("\r\n %#x\r\n ",read_startmode_flag);
//    for(i=0;i<sizeof(_EEPROM_pos)+Start_mode_Addr+4;i++)
//    {
//        printf("\r\n tetsbuf0=%d \r\n",tetsbuf[i]);
//    }
//	printf("\r\n tetsbuf0=%s\r\n",tetsbuf);
	if(read_startmode_flag == Start_configmode_flag)
	{
		printf(" configure mode \n");
		EEPROM_Write(Start_mode_Addr,(u8*)&Start_normalmode_flag,4);
		SysStart_mode_flag=1;
	}
	else
	{
		printf(" normal mode-----------------------ggsensor.c \t  \n");
		EEPROM_Write(Start_mode_Addr,(u8*)&Start_configmode_flag,4);
		SysStart_mode_flag=0;
	}
    
//    EEPROM_Write(Start_mode_Addr,(u8*)&Start_configmode_flag,4);//    const u32 Start_normalmode_flag=0x11223344;

//	EEPROM_Read(Start_mode_Addr,(u8*)i,4);
//    for(j=0;j<1;j++)
//    {
//        printf("\r\n j[%d] =%#x \r\n",j,i[j]);
//    }
    
    
    
	EEPROM_Read(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
//    printf("\r\n EEPROM_pos_buf1 =%d \r\n",EEPROM_pos_buf.flag);
	
	if((EEPROM_pos_buf.flag != eeprom_posflag)&&(eeprom_pos_permit_flag==0))
	{
		printf("\r\n first init eeprom \r\n");

		EEPROM_pos_buf.Save_next_addr 			=	save_next_addr;		//该页存放的地址
		EEPROM_pos_buf.Save_next_addr_page 	= save_next_addr_page;		//扇区第多少页
		EEPROM_pos_buf.Save_next_addr_sect 	= save_next_addr_sect;   //存放在第几个扇区
		
		EEPROM_pos_buf.Send_next_addr 			= send_next_addr;		//该页存放的下一个要发送的地址
		EEPROM_pos_buf.Send_next_addr_page 	= send_next_addr_page;		//要发送的数据在第几个页
		EEPROM_pos_buf.Send_next_addr_sect 	= send_next_addr_sect;	
		
		EEPROM_pos_buf.fullflag =	FLASH_full_flag;
		EEPROM_pos_buf.flag  =	eeprom_posflag;
        
        
        
		EEPROM_Write(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
		EEPROM_Read(POS_WR_Addr,tetsbuf,sizeof(_EEPROM_pos));
//    for(j=0;j<sizeof(_EEPROM_pos);j++)
//    {
//        printf("\r\n j[%d] =%#x \r\n",j,tetsbuf[j]);
//    }
//        printf("\r\n tetsbuf =%s \r\n",tetsbuf);
//        printf("\r\n EEPROM_pos_buf =%d \r\n",EEPROM_pos_buf.flag);	
        
        
        
		if(Buffercmp(tetsbuf,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos)))
		{
			printf("\r\n write error 11 !\r\n");
			write_num=5;
			while(write_num)
			{
				delay_ms(30);
				EEPROM_Write(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
				EEPROM_Read(POS_WR_Addr,tetsbuf,sizeof(_EEPROM_pos));
				if(Buffercmp(tetsbuf,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos))==0)
				{
					printf("\r\n write OK11!\r\n");
					break;
				}
				else
				write_num--;
			}	
		}
	}
	else
	{
//		printf("\r\n read  eeprom pos \r\n");
	  save_next_addr=EEPROM_pos_buf.Save_next_addr;		//该页存放的地址0
		save_next_addr_page=EEPROM_pos_buf.Save_next_addr_page;		//扇区第多少页1
		save_next_addr_sect=EEPROM_pos_buf.Save_next_addr_sect;   //存放在第几个扇区2
		
		send_next_addr=EEPROM_pos_buf.Send_next_addr;		//该页存放的下一个要发送的地址3
		send_next_addr_page=EEPROM_pos_buf.Send_next_addr_page;		//要发送的数据在第几个页4
		send_next_addr_sect=EEPROM_pos_buf.Send_next_addr_sect;	
		
		FLASH_full_flag=EEPROM_pos_buf.fullflag;
	}
	printf("\n save_next_addr:%d save_next_addr_page:%d  save_next_addr_sect:%d ---------ggsensor.c \t\n",save_next_addr,save_next_addr_page,save_next_addr_sect);
printf("\n send_next_addr:%d send_next_addr_page:%d  save_next_addr_sect:%d -----------ggsensor.c \t \n",send_next_addr,send_next_addr_page,send_next_addr_sect);
printf("\n FLASH_full_flag:%d --------ggsensor.c \t \r\n",FLASH_full_flag);

}

void Read_pos_eeprom(void)  //从STM32 内部 EEPROM 读取FLASH写数据和发送数据的地址
{	
//	u8 completeflag=0;
	EEPROM_Read(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
	
//	printf("\r\n read  eeprom pos \r\n");
	save_next_addr=EEPROM_pos_buf.Save_next_addr;		//该页存放的地址
	save_next_addr_page=EEPROM_pos_buf.Save_next_addr_page;		//扇区第多少页
	save_next_addr_sect=EEPROM_pos_buf.Save_next_addr_sect;   //存放在第几个扇区
		
	send_next_addr=EEPROM_pos_buf.Send_next_addr;		//该页存放的下一个要发送的地址
	send_next_addr_page=EEPROM_pos_buf.Send_next_addr_page;		//要发送的数据在第几个页
	send_next_addr_sect=EEPROM_pos_buf.Send_next_addr_sect;	
		
	FLASH_full_flag=EEPROM_pos_buf.fullflag;
}
u8 Write_pos_eeprom(void)  //从STM32 内部 EEPROM 写FLASH写数据和发送数据的地址
{
	u8 completeflag=0;
	
	u8 tetsbuf[sizeof(_EEPROM_pos)];
	u8 write_num;
//	printf("\r\n write  eeprom pos \r\n");
	EEPROM_pos_buf.Save_next_addr 			=	save_next_addr;		//该页存放的地址
	EEPROM_pos_buf.Save_next_addr_page 	= save_next_addr_page;		//扇区第多少页
	EEPROM_pos_buf.Save_next_addr_sect 	= save_next_addr_sect;   //存放在第几个扇区
		
	EEPROM_pos_buf.Send_next_addr 			= send_next_addr;		//该页存放的下一个要发送的地址
	EEPROM_pos_buf.Send_next_addr_page 	= send_next_addr_page;		//要发送的数据在第几个页
	EEPROM_pos_buf.Send_next_addr_sect 	= send_next_addr_sect;	
		
	EEPROM_pos_buf.fullflag =	FLASH_full_flag;
	EEPROM_pos_buf.flag  =	eeprom_posflag;

//	EEPROM_Write_Repeate(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
	
	EEPROM_Write(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
	EEPROM_Read(POS_WR_Addr,tetsbuf,sizeof(_EEPROM_pos));
		
		if(Buffercmp(tetsbuf,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos)))
		{
			printf("\r\n write error 11 !\r\n");
			write_num=5;
			while(write_num)
			{
				delay_ms(30);
				EEPROM_Write(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
				EEPROM_Read(POS_WR_Addr,tetsbuf,sizeof(_EEPROM_pos));
				if(Buffercmp(tetsbuf,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos))==0)
				{
					printf("\r\n write OK11!\r\n");
					completeflag=1;
					break;
				}
				else
				write_num--;
			}	
		}
		else
			completeflag=1;
		
		return completeflag;
}

u8 Write_pos_eeprom_Clear(void)  //从STM32 内部 EEPROM 把FLASH写数据和发送数据的地址清零
{
	u8 completeflag=0;
	
	u8 tetsbuf[sizeof(_EEPROM_pos)];
	u8 write_num;
	printf("\r\n write  eeprom pos \r\n");
	EEPROM_pos_buf.Save_next_addr 			=	0;		//该页存放的地址
	EEPROM_pos_buf.Save_next_addr_page 	= 0;		//扇区第多少页
	EEPROM_pos_buf.Save_next_addr_sect 	= 0;   //存放在第几个扇区
		
	EEPROM_pos_buf.Send_next_addr 			= 0;		//该页存放的下一个要发送的地址
	EEPROM_pos_buf.Send_next_addr_page 	= 0;		//要发送的数据在第几个页
	EEPROM_pos_buf.Send_next_addr_sect 	= 0;	
		
	EEPROM_pos_buf.fullflag =	0;
	EEPROM_pos_buf.flag  =	eeprom_posflag;

	EEPROM_Write(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
	EEPROM_Read(POS_WR_Addr,tetsbuf,sizeof(_EEPROM_pos));
		if(Buffercmp(tetsbuf,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos)))
		{
			printf("\r\n write error 11 !\r\n");
			write_num=5;
			while(write_num)
			{
				delay_ms(30);
				EEPROM_Write(POS_WR_Addr,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos));
				EEPROM_Read(POS_WR_Addr,tetsbuf,sizeof(_EEPROM_pos));
				if(Buffercmp(tetsbuf,(u8*)&EEPROM_pos_buf,sizeof(_EEPROM_pos))==0)
				{
					printf("\r\n write OK11!\r\n");
					completeflag=1;
					break;
				}
				else
				write_num--;
			}	
		}
		else
			completeflag=1;
		
		return completeflag;
}

void Init_param_eeprom(void)  //从STM32 内部 EEPROM 初始化 自身参数 节点ID  硬件编号等
{
	u8 tetsbuf[400];
//	u8 write_num;
//	u8 tetsbuf1[sizeof(_EEPROM_param)];
	EEPROM_Read(0,tetsbuf,sizeof(_EEPROM_param)+PARAM_WR_Addr);
	memcpy((u8*)&EEPROM_param_buf,tetsbuf+PARAM_WR_Addr,sizeof(_EEPROM_param));
	
	if((EEPROM_param_buf.flagp != eeprom_paramflag)&&(eeprom_param_permit_flag==0))
	{
		printf("\r\n first init eeprom  param------------------ggsensor.c \t  \r\n");
		Re_Registration_time_min =send_time_min;
		EEPROM_param_buf.flagp =eeprom_paramflag;
		EEPROM_param_buf.Collection_time_Min =collection_time_min;
		EEPROM_param_buf.GetTimeEcho_Time_Hour = GettimeEcho_time_Hour;
		EEPROM_param_buf.Re_Registration_Time_Min  = Re_Registration_time_min;
		EEPROM_param_buf.Address =Address_ID;
		EEPROM_param_buf.Rf_feq  =Rf_fequence;
		
		EEPROM_param_buf.WIFI_RF_Mode = WIFI_or_RF_SendMode;

		memcpy(EEPROM_param_buf.Sta,sta,70);
		memcpy(EEPROM_param_buf.Staip,staip,100);
		memcpy(EEPROM_param_buf.Sock,sock,70);
		
		EEPROM_param_buf.Retry_Send_num =retry_Register_num;	
		
		EEPROM_param_buf.Display_mode =display_mode;
		EEPROM_param_buf.Lcd_time_Min  =lcd_time_min;
		
		EEPROM_param_buf.Ccs811_mode=VOC_State;
		EEPROM_param_buf.Ccs811_time_Min=VOC_collection_time_min;
		EEPROM_param_buf.Hcho_mode=HCHO_State;
		EEPROM_param_buf.hcho_time_Min=HCHO_collection_time_min;
		EEPROM_param_buf.Coz_mode=COZ_State;
		EEPROM_param_buf.Coz_time_Min=COZ_collection_time_min;
		EEPROM_param_buf.Wait_Wifi_ConnectTime = Wait_WIFI_ConnectTime/10;
		
//		memcpy(EEPROM_param_buf.Wifi_Send_TimeRange,WIFI_Send_TIME_Range,4);有BUG
		
		EEPROM_param_buf.Wifi_Send_TimeRange[0]=WIFI_Send_TIME_Range[0];
		EEPROM_param_buf.Wifi_Send_TimeRange[1]=WIFI_Send_TIME_Range[1];
		EEPROM_param_buf.Wifi_Send_TimeRange[2]=WIFI_Send_TIME_Range[2];
		EEPROM_param_buf.Wifi_Send_TimeRange[3]=WIFI_Send_TIME_Range[3];
		
		EEPROM_Write_Repeate(PARAM_WR_Addr,(u8*)&EEPROM_param_buf,sizeof(_EEPROM_param));
//		delay_ms(2);
//		EEPROM_Read(0,tetsbuf,sizeof(_EEPROM_param)+PARAM_WR_Addr);
//		memcpy(tetsbuf1,tetsbuf+PARAM_WR_Addr,sizeof(_EEPROM_param));
//		if(Buffercmp(tetsbuf1,(u8*)&EEPROM_param_buf,sizeof(_EEPROM_param)))
//		{
//			printf("\r\n write error 11 !\r\n");
//			write_num=5;
//			while(write_num)
//			{
//				delay_ms(30);
//				EEPROM_Write(PARAM_WR_Addr,(u8*)&EEPROM_param_buf,sizeof(_EEPROM_param));
//				EEPROM_Read(0,tetsbuf,sizeof(_EEPROM_param)+PARAM_WR_Addr);
//				memcpy(tetsbuf1,tetsbuf+PARAM_WR_Addr,sizeof(_EEPROM_param));
//				if(Buffercmp(tetsbuf1,(u8*)&EEPROM_param_buf,sizeof(_EEPROM_param))==0)
//				{
//					printf("\r\n write OK11!\r\n");
//					break;
//				}
//				else
//				write_num--;
//			}	
//		}
	}
	else
	{
//		printf("\r\n read eeprom param \r\n");
		
		collection_time_min				=EEPROM_param_buf.Collection_time_Min;
		GettimeEcho_time_Hour			=EEPROM_param_buf.GetTimeEcho_Time_Hour;
		Re_Registration_time_min	=EEPROM_param_buf.Re_Registration_Time_Min;
		Address_ID			=EEPROM_param_buf.Address;
		Rf_fequence	=EEPROM_param_buf.Rf_feq;
		
		WIFI_or_RF_SendMode	=EEPROM_param_buf.WIFI_RF_Mode;

		memcpy(sta,EEPROM_param_buf.Sta,70);
		memcpy(staip,EEPROM_param_buf.Staip,100);
		memcpy(sock,EEPROM_param_buf.Sock,70);
		
		retry_Register_num=EEPROM_param_buf.Retry_Send_num;	
		
		display_mode=EEPROM_param_buf.Display_mode;
		lcd_time_min=EEPROM_param_buf.Lcd_time_Min;
		
		VOC_State=EEPROM_param_buf.Ccs811_mode;
		VOC_collection_time_min=EEPROM_param_buf.Ccs811_time_Min;
		HCHO_State=EEPROM_param_buf.Hcho_mode;
		HCHO_collection_time_min=EEPROM_param_buf.hcho_time_Min;
		COZ_State=EEPROM_param_buf.Coz_mode;
		COZ_collection_time_min=EEPROM_param_buf.Coz_time_Min;
		Wait_WIFI_ConnectTime=EEPROM_param_buf.Wait_Wifi_ConnectTime*10;
		
//		memcpy(WIFI_Send_TIME_Range,EEPROM_param_buf.Wifi_Send_TimeRange,4);
		
		WIFI_Send_TIME_Range[0]=EEPROM_param_buf.Wifi_Send_TimeRange[0];
		WIFI_Send_TIME_Range[1]=EEPROM_param_buf.Wifi_Send_TimeRange[1];
		WIFI_Send_TIME_Range[2]=EEPROM_param_buf.Wifi_Send_TimeRange[2];
		WIFI_Send_TIME_Range[3]=EEPROM_param_buf.Wifi_Send_TimeRange[3];
		
		send_time_min=Re_Registration_time_min;
		send_time=Re_Registration_time_min*60;
		
		collection_time=collection_time_min*60;
		GettimeEcho_time=GettimeEcho_time_Hour*3600;	
		Re_Registration_time=	 Re_Registration_time_min*60;
		lcd_time= lcd_time_min*60;
		VOC_collection_time=VOC_collection_time_min*60;
		HCHO_collection_time=HCHO_collection_time_min*60;
		COZ_collection_time=COZ_collection_time_min*60;
		retry_senddata_num=retry_Register_num;//发送数据不成功 重新发送的次数 == retry_Register_num
		WIFI_RETRY_SEC=retry_Register_num;//WIFI连接服务器重发的次数 ==retry_Register_num
		RetryRegisterTime=Re_Registration_time;//上电第一次注册失败(RTC 没有时间) 重新注册时间 (12s) ==断网之后的注册周期 Re_Registration_time
		HARDWARE_ERROR_sleeptime=collection_time;//(S)进入HARDWARE_ERROR休眠的时间 时间到了检测电压  ==collection_time
		LoRa_FREQENCY_R=Rf_fequence*1000000;
		
	}
//	printf("\r\n WIFI_or_RF_SendMode =%d  Wait_WIFI_ConnectTime =%d\r\n",WIFI_or_RF_SendMode,Wait_WIFI_ConnectTime);
//	printf("\r\n collection_time:%d  display_mode:%d minimum_operate_voltage:%f\r\n",collection_time,display_mode,minimum_operate_voltage);
//	printf("\r\n node_id:%d  RegionAddr[0]:%d RegionAddr[1]:%d retry_Register_num:%d\r\n",node_id,RegionAddr[0],RegionAddr[1],retry_Register_num);
//	printf("\r\n GettimeEcho_time:%d  retry_senddata_num:%d WIFI_RETRY_SEC:%d\r\n",GettimeEcho_time,retry_senddata_num,WIFI_RETRY_SEC);
//	printf("\r\n RetryRegisterTime:%d  HARDWARE_ERROR_sleeptime:%d \r\n",RetryRegisterTime,HARDWARE_ERROR_sleeptime);
//	printf("\r\n RF_FREQENCY:%d  power:%d  SB:%d SF:%d  EC:%d\r\n",(u16)((rf_param[0]<<8)+rf_param[1]),rf_param[2],rf_param[3],rf_param[4],rf_param[5]);
	
}
u8 Wirte_param_eeprom(void)  //从STM32 内部 EEPROM 写 自身参数 节点ID  硬件编号等
{
	u8 completeflag=0;
	
//	u8 tetsbuf[400];
//	u8 tetsbuf1[sizeof(_EEPROM_param)];
//	u8 write_num;
		Re_Registration_time_min =send_time_min;
		EEPROM_param_buf.flagp =eeprom_paramflag;
		EEPROM_param_buf.Collection_time_Min =collection_time_min;
		EEPROM_param_buf.GetTimeEcho_Time_Hour = GettimeEcho_time_Hour;
		EEPROM_param_buf.Re_Registration_Time_Min  = Re_Registration_time_min;
		EEPROM_param_buf.Address =Address_ID;
		EEPROM_param_buf.Rf_feq  =Rf_fequence;
		
		EEPROM_param_buf.WIFI_RF_Mode = WIFI_or_RF_SendMode;

		memcpy(EEPROM_param_buf.Sta,sta,70);
		memcpy(EEPROM_param_buf.Staip,staip,100);
		memcpy(EEPROM_param_buf.Sock,sock,70);
		
		EEPROM_param_buf.Retry_Send_num =retry_Register_num;	
		
		EEPROM_param_buf.Display_mode =display_mode;
		EEPROM_param_buf.Lcd_time_Min  =VOC_collection_time_min;
		
		EEPROM_param_buf.Ccs811_mode=VOC_State;
		EEPROM_param_buf.Ccs811_time_Min=VOC_collection_time_min;
		EEPROM_param_buf.Hcho_mode=HCHO_State;
		EEPROM_param_buf.hcho_time_Min=HCHO_collection_time_min;
		EEPROM_param_buf.Coz_mode=COZ_State;
		EEPROM_param_buf.Coz_time_Min=COZ_collection_time_min;
		EEPROM_param_buf.Wait_Wifi_ConnectTime = Wait_WIFI_ConnectTime/10;
//		memcpy(EEPROM_param_buf.Wifi_Send_TimeRange,WIFI_Send_TIME_Range,4);
		EEPROM_param_buf.Wifi_Send_TimeRange[0]=WIFI_Send_TIME_Range[0];
		EEPROM_param_buf.Wifi_Send_TimeRange[1]=WIFI_Send_TIME_Range[1];
		EEPROM_param_buf.Wifi_Send_TimeRange[2]=WIFI_Send_TIME_Range[2];
		EEPROM_param_buf.Wifi_Send_TimeRange[3]=WIFI_Send_TIME_Range[3];
		completeflag=EEPROM_Write_Repeate(PARAM_WR_Addr,(u8*)&EEPROM_param_buf,sizeof(_EEPROM_param));

		return completeflag;
}

void Read_param_eeprom(void)  //从STM32 内部 EEPROM 读 自身参数 节点ID  硬件编号等
{
	u8 tetsbuf[400];
//	u8 tetsbuf1[sizeof(_EEPROM_param)];
//	u8 k;
	EEPROM_Read(0,tetsbuf,sizeof(_EEPROM_param)+PARAM_WR_Addr);
	memcpy((u8*)&EEPROM_param_buf,tetsbuf+PARAM_WR_Addr,sizeof(_EEPROM_param));
//	printf("\n read eeprom param ");
collection_time_min				=EEPROM_param_buf.Collection_time_Min;
		GettimeEcho_time_Hour			=EEPROM_param_buf.GetTimeEcho_Time_Hour;
		Re_Registration_time_min	=EEPROM_param_buf.Re_Registration_Time_Min;
		Address_ID			=EEPROM_param_buf.Address;
		Rf_fequence	=EEPROM_param_buf.Rf_feq;
		
		WIFI_or_RF_SendMode	=EEPROM_param_buf.WIFI_RF_Mode;

		memcpy(sta,EEPROM_param_buf.Sta,70);
		memcpy(staip,EEPROM_param_buf.Staip,100);
		memcpy(sock,EEPROM_param_buf.Sock,70);
		
		retry_Register_num=EEPROM_param_buf.Retry_Send_num;	
		
		display_mode=EEPROM_param_buf.Display_mode;
		lcd_time_min=EEPROM_param_buf.Lcd_time_Min;
		
		VOC_State=EEPROM_param_buf.Ccs811_mode;
		VOC_collection_time_min=EEPROM_param_buf.Ccs811_time_Min;
		HCHO_State=EEPROM_param_buf.Hcho_mode;
		HCHO_collection_time_min=EEPROM_param_buf.hcho_time_Min;
		COZ_State=EEPROM_param_buf.Coz_mode;
		COZ_collection_time_min=EEPROM_param_buf.Coz_time_Min;
		Wait_WIFI_ConnectTime=EEPROM_param_buf.Wait_Wifi_ConnectTime*10;
		
//		memcpy(WIFI_Send_TIME_Range,EEPROM_param_buf.Wifi_Send_TimeRange,4);这句有bug
		WIFI_Send_TIME_Range[0]=EEPROM_param_buf.Wifi_Send_TimeRange[0];
		WIFI_Send_TIME_Range[1]=EEPROM_param_buf.Wifi_Send_TimeRange[1];
		WIFI_Send_TIME_Range[2]=EEPROM_param_buf.Wifi_Send_TimeRange[2];
		WIFI_Send_TIME_Range[3]=EEPROM_param_buf.Wifi_Send_TimeRange[3];
		
		send_time_min=Re_Registration_time_min;
		send_time=Re_Registration_time_min*60;
		
		collection_time=collection_time_min*60;
		GettimeEcho_time=GettimeEcho_time_Hour*3600;	
		Re_Registration_time=	 Re_Registration_time_min*60;
		lcd_time= lcd_time_min*60;
		VOC_collection_time=VOC_collection_time_min*60;
		HCHO_collection_time=HCHO_collection_time_min*60;
		COZ_collection_time=COZ_collection_time_min*60;
		retry_senddata_num=retry_Register_num;//发送数据不成功 重新发送的次数 == retry_Register_num
		WIFI_RETRY_SEC=retry_Register_num;//WIFI连接服务器重发的次数 ==retry_Register_num
		RetryRegisterTime=Re_Registration_time;//上电第一次注册失败(RTC 没有时间) 重新注册时间 (12s) ==断网之后的注册周期 Re_Registration_time
		HARDWARE_ERROR_sleeptime=collection_time;//(S)进入HARDWARE_ERROR休眠的时间 时间到了检测电压  ==collection_time

	LoRa_FREQENCY_R=Rf_fequence*1000000;
//	printf("\n  WIFI_or_RF_SendMode=%d	Wait_WIFI_ConnectTime=%d",WIFI_or_RF_SendMode,Wait_WIFI_ConnectTime);
	printf("\ncollection_time=%d,send_time=%d,min_vol=%f,RF=%dM",collection_time,send_time,minimum_operate_voltage,Rf_fequence);
	printf("\nGettimeEcho_time=%d,retry_senddata_num:%d,RetryRegisterTime=%d,HARDWARE_ERROR_sleeptime=%d",GettimeEcho_time,retry_senddata_num,RetryRegisterTime,HARDWARE_ERROR_sleeptime);
	if(WIFI_or_RF_SendMode==WIFI_SEND)
	{
		printf("\nsta=%s,staip=%s,sock=%s",sta,staip,sock);
		printf("\nWIFISENDT=%04d,%04d,WIFI_RETRY_SEC=%d",WIFI_Send_TIME_Range[0]*100+WIFI_Send_TIME_Range[1],WIFI_Send_TIME_Range[2]*100+WIFI_Send_TIME_Range[3],WIFI_RETRY_SEC);
	}
		
	if(display_mode==ENABLE)
		printf("\nlcd_time=%d,lcd_time_min=%d,",lcd_time,lcd_time_min);
	if(HCHO_State==ENABLE)
		printf("HCHO_collection_time=%d,",HCHO_collection_time);
	if( VOC_State==ENABLE)
		printf("VOC_collection_time=%d,",VOC_collection_time);
	if( COZ_State==ENABLE)
		printf("COZ_collection_time=%d",COZ_collection_time);
}



void write_startmode_normal_eeprom(void)  //写入正常启动方式标志位   //update_20171013
{
//	printf("\r\n write start mode normal flag  \r\n");
	EEPROM_Write(Start_mode_Addr,(u8*)&Start_normalmode_flag,4);
}
u8 in_flash()//数据写入FLASH  写入地址改变一次
{
	u8 flagp;
	save_next_addr	=	save_next_addr	+	sizeof(_FLASH_data);
	if(save_next_addr>255-sizeof(_FLASH_data))
	{
		save_next_addr	=	0;
		save_next_addr_page++;
		if(save_next_addr_page>255)
		{
			save_next_addr_page=0;
			save_next_addr_sect++;
			if(save_next_addr_sect>15)
			{
				save_next_addr_sect=0;
				FLASH_full_flag=1;
				FLASH_first_write_data=1;//不是第一次写数据了
			}
			if( M25P80_state==M25P80_S_OFF)
			{
				 M25P80_WAKE();
			}
			M25P80_Section_Erase(save_next_addr_sect*256*256); //擦除一个扇区
			printf("\r\n **************ERASE section****addr:%X************ \r\n",save_next_addr_sect*256*256);
			if((send_next_addr_sect==save_next_addr_sect)&&(FLASH_full_flag==1))//写地址追上上一圈的发送的地址
			{
				send_next_addr=0;
				send_next_addr_page=0;
				send_next_addr_sect++;
			}
		}
	}
//	 printf("\r\n  save next flash dizhi,page,section: %d  %d %d\r\n",save_next_addr,save_next_addr_page,save_next_addr_sect);
	//存地址
	
	flagp=Write_pos_eeprom();
	return flagp;
}
u8 out_flash()//数据从FLASH读出  发送地址改变一次
{
	u8 flagp;
	send_next_addr	=	send_next_addr	+	sizeof(_FLASH_data);
			
	if(send_next_addr>255-sizeof(_FLASH_data))
	{
		send_next_addr	=	0;
		send_next_addr_page++;
		if(send_next_addr_page>255)
		{
			send_next_addr_page=0;
			send_next_addr_sect++;
			if(send_next_addr_sect>15)
			{
				send_next_addr_sect=0;
				FLASH_full_flag=0;
			}
		}
	}
	FLASH_Start_Busy();
//	printf("\r\n  send next flash dizhi,page,section: %d  %d %d\r\n",send_next_addr,send_next_addr_page,send_next_addr_sect);	
	flagp=Write_pos_eeprom();
	FLASH_Free();
	return flagp;
}
u8 Write_0_to_Flash()//给FLASH写0
{
	if( M25P80_state==M25P80_S_OFF)
	{
		 M25P80_WAKE();
	}
	Save_Addr = 256*256*save_next_addr_sect	+	256*save_next_addr_page	+	save_next_addr;

	FLASH_data_buf.flagf=0;
	if((save_next_addr_sect==0) && (save_next_addr_page==0) && (save_next_addr==0) && (FLASH_first_write_data==0))	
	{
		M25P80_Section_Erase(0*256*256); //擦除第一个扇区   还要加上擦除后将发送指针前移操作
		printf("\r\n **************ERASE first section****************** \r\n");
	}
	M25P80_Write_Bytes(Save_Addr,(u8*)&FLASH_data_buf,sizeof(_FLASH_data));  
	return 1;
}
u8 Write_Data_to_Flash_one()//将采集的传感器数据写入FLASH  一次  返回值为2才算成功
{
  u8 completeflag=0;
	u8 testbuf[100];	
	u8 write_num;
	
	Save_Addr = 256*256*save_next_addr_sect	+	256*save_next_addr_page	+	save_next_addr;

	FLASH_data_buf.flagf=flash_dataflag;
	FLASH_data_buf.Light_intensity=Light;
	FLASH_data_buf.Tem_humi=tem_humi;
	FLASH_data_buf.Tem_temp=tem_temp;
	FLASH_data_buf.Timestamp=SYSTimestamp;
	FLASH_data_buf.Voltage_value=voltage_value; 	
	FLASH_data_buf.SeqNo=packet_sequence_flash;
	FLASH_data_buf.Ultraviolet=ultraviolet;
	FLASH_data_buf.eCO2 = eco2v;
	FLASH_data_buf.TVOC = tvocv;
	FLASH_data_buf.Formaldehyde = formaldehyde;
	FLASH_data_buf.CO2=co2v;
	FLASH_data_buf.Pm2_5=PM2_5;
	FLASH_data_buf.Pm10=PM10;
//	printf(" 12343eco2_flash:%d tvoc_flash:%d formaldehyde_flash:%d",FLASH_data_buf.eCO2,FLASH_data_buf.TVOC ,	FLASH_data_buf.Formaldehyde);
//	printf("\r\n 123FLASH DATA : Voltage= %d V ",voltage_value_flash); 
//	printf(" 1324 light:%d T:%d  H:%d timestamp:%d Save_Addr:%x \r\n",Light_flash,tem_temp_flash,tem_humi_flash,timestamp_flash,Save_Addr);
	if((save_next_addr_sect==0) && (save_next_addr_page==0) && (save_next_addr==0) && (FLASH_first_write_data==0))	
	{
		M25P80_Section_Erase(0*256*256); //擦除第一个扇区   还要加上擦除后将发送指针前移操作
		printf("\r\n **************ERASE first section****************** \r\n");
	}
	M25P80_Write_Bytes(Save_Addr,(u8*)&FLASH_data_buf,sizeof(_FLASH_data));  
	
	M25P80_Read_Bytes(Save_Addr,(u8*)&testbuf,sizeof(_FLASH_data));
//	testbuf[5]=89;
//	testbuf[8]=89;
//	testbuf[10]=89;
		if(Buffercmp(testbuf,(u8*)&FLASH_data_buf,sizeof(_FLASH_data)))
		{
			printf("\r\n write error to flash  11!\r\n");
			write_num=3;
			while(write_num)
			{
				delay_ms(30);
				M25P80_Write_Bytes(Save_Addr,(u8*)&FLASH_data_buf,sizeof(_FLASH_data));  
				delay_ms(1);
				M25P80_Read_Bytes(Save_Addr,(u8*)&testbuf,sizeof(_FLASH_data));
//	testbuf[5]=89;
//	testbuf[8]=89;
//	testbuf[10]=89;
				if(Buffercmp(testbuf,(u8*)&FLASH_data_buf,sizeof(_FLASH_data))==0)
				{
					printf("\r\n write error to flash OK11!\r\n");
					completeflag=1;
					break;
				}
				else
				write_num--;
			}	
		}
		else
			completeflag=1;
		
	
//	printf(" save sense data to FLASH\r\n");
//	for(int k=0; k<sizeof(_FLASH_data); k++ )
//		printf(" %02X", testbuf[k]);
	
	completeflag+=in_flash();
	return completeflag;
	
}

u8 Write_Data_to_Flash(void)//将采集的传感器数据写入FLASH  
{
	u8 returnflag;
	u8 checkflag,cycleflag;
	if( M25P80_state!=M25P80_S_ON)
	  M25P80_WAKE();
	FLASH_Start_Busy();
	++packet_sequence_flash;
	
//	printf("\r\n Write_Data_to_Flash_one \r\n");
	checkflag=Write_Data_to_Flash_one();
	if(checkflag==2)
	{
//		printf("\r Write OK! \n");
		returnflag=1;
	}
	else
	{
		cycleflag=1;
		while(cycleflag)
		{
			Write_0_to_Flash();
			in_flash();
			checkflag=Write_Data_to_Flash_one();
			printf("\r\n *************write error skip a unit*****\r\n");
			if(checkflag==2)
			{
				cycleflag=0;
				returnflag=1;
			}
		}
	}
//		M25P80_SLEEP();
//	SENSE_PWEN_OFF;
	FLASH_Free();
	return returnflag;
}





u8 Read_Data_from_Flash(void)//从FLASH读出要发送的数据
{
  u8 completeflag=0;
	if( M25P80_state==M25P80_S_OFF)
	  M25P80_WAKE();
	FLASH_Start_Busy();
	Send_Addr = 256*256*send_next_addr_sect + 256*send_next_addr_page + send_next_addr;
//		Send_Addr =Save_Addr;//调试用
	M25P80_Read_Bytes(Send_Addr,(u8*)&FLASH_data_buf,sizeof(_FLASH_data)); 
	if(FLASH_data_buf.flagf==flash_dataflag)
	{
		Light_flash=FLASH_data_buf.Light_intensity;
		tem_humi_flash=FLASH_data_buf.Tem_humi;
		tem_temp_flash=FLASH_data_buf.Tem_temp;
		timestamp_flash=FLASH_data_buf.Timestamp;
		voltage_value_flash=FLASH_data_buf.Voltage_value; 
		packet_sequence_send=FLASH_data_buf.SeqNo;
		ultraviolet_flash=FLASH_data_buf.Ultraviolet;
		eco2v_flash=FLASH_data_buf.eCO2;
		tvocv_flash=FLASH_data_buf.TVOC;
		formaldehyde_flash=FLASH_data_buf.Formaldehyde;
		co2v_flash=FLASH_data_buf.CO2;
		PM2_5_flash=FLASH_data_buf.Pm2_5;
		PM10_flash=FLASH_data_buf.Pm10;
		
		printf("\n FLASH DATA : Volt=%dV,",voltage_value_flash); 
	  printf(" light=%d,T=%d,H=%d,timestamp=%d",Light_flash,tem_temp_flash,tem_humi_flash,timestamp_flash);
		printf(" eco2=%d,tvoc=%d,formaldehyde=%d,Send_Addr=%x",eco2v_flash,tvocv_flash,formaldehyde_flash,Send_Addr);
		printf(" co2v= %dppm,pm2.5=%d,pm10=%dug/m3",co2v_flash,PM2_5_flash,PM10_flash);
		completeflag=1;		
	}
	else
	{
		completeflag=0;	
		printf("Data read error");
	}
	FLASH_Free();
	return completeflag;
}
u8 Read_Data_from_eeprom_last(void)//从EEPROM读出上次的CO2
{
  u8 completeflag=0;
	u8 tetsbuf[500];
//	u8 tetsbuf1[sizeof(_EEPROM_COZdata)];
	EEPROM_Read(0,tetsbuf,sizeof(_EEPROM_COZdata)+CO2_WR_Addr);
	memcpy((u8*)&EEPROM_CO2_data,tetsbuf+CO2_WR_Addr,sizeof(_EEPROM_COZdata));

	if(EEPROM_CO2_data.flagp==0x12)
	{
		co2v_flash=EEPROM_CO2_data.Coz_value;
		
		completeflag=1;		
	}
	else
	{
		completeflag=0;	
		printf(" CO2 read error");
	}
	
	if(co2v_flash<200)
		co2v_flash=400;
	
	co2v=co2v_flash;
	printf("\nco2v_flash= %dppm",co2v_flash);
	return completeflag;
}
u8 Write_Data_from_eeprom_last(void)//从EEPROM写入上次的CO2
{
  u8 completeflag=0;

	EEPROM_CO2_data.flagp=0x12;
	EEPROM_CO2_data.Coz_value=co2v;
	
	completeflag=EEPROM_Write_Repeate(CO2_WR_Addr,(u8*)&EEPROM_CO2_data,sizeof(_EEPROM_COZdata));
		
	return completeflag;
}
u16 Read_Data_from_eeprom_lastLight(void)//从EEPROM读出上次的light
{
  u16 completeflag;
	u8 tetsbuf[500];
//	u8 tetsbuf1[sizeof(_EEPROM_COZdata)];
	EEPROM_Read(0,tetsbuf,sizeof(_EEPROM_LIGHTdata)+LIGHT_WR_Addr);
	memcpy((u8*)&EEPROM_LIGHT_data,tetsbuf+LIGHT_WR_Addr,sizeof(_EEPROM_LIGHTdata));

	if(EEPROM_LIGHT_data.flagp==0x12)
	{
		completeflag=EEPROM_LIGHT_data.Light_value;
	}
	else
	{
		printf("light read error");
	}
//	printf("\r\n EEPROM light_flash= %d V ",co2v_flash);
	return completeflag;
}
u8 Write_Data_from_eeprom_lastLight(void)//从EEPROM写入上次的light
{
  u8 completeflag=0;
	EEPROM_LIGHT_data.flagp=0x12;
	printf("\n Light:%d",Light_save);
	EEPROM_LIGHT_data.Light_value=Light_save;
	EEPROM_Write(LIGHT_WR_Addr,(u8*)&EEPROM_LIGHT_data,sizeof(_EEPROM_LIGHTdata));
	return completeflag;
}

u8 Read_Data_from_RAM(void)//从 RAM　读出要发送的数据
{	
		Light_flash=Light;
		tem_humi_flash=tem_humi;
		tem_temp_flash=tem_temp;
		timestamp_flash=SYSTimestamp;
		voltage_value_flash=voltage_value; 
		packet_sequence_send=packet_sequence_flash;
		ultraviolet_flash=ultraviolet;
		eco2v_flash=eco2v;
		tvocv_flash=tvocv;
		formaldehyde_flash=formaldehyde;
		co2v_flash=co2v;
//		printf("\r\n FLASH DATA : Voltage= %d V ",voltage_value_flash); 
//	  printf(" light:%d T:%d  H:%d timestamp:%d\r\n",Light_flash,tem_temp_flash,tem_humi_flash,timestamp_flash);
return 1;
}
void RF_Send_flash_Data(void)//向网关发送FLASH数据
{
	u8 data[60];
	uint16_t framelen,crc;
	uint8_t datalen;
	u8 seq[2];
	data[0] = (Address_ID>>24);
	data[1] = (Address_ID>>16);
	data[2] = (Address_ID>>8);
	data[3] = (Address_ID);
	data[4] = 0;//Hop
	
	data[5]=(u8)(timestamp_flash>>24);	
	data[6]=(u8)(timestamp_flash>>16);
	data[7]=(u8)(timestamp_flash>>8);
	data[8]=(u8)(timestamp_flash);
		
	data[9]=0x81;
	data[10]=(u8)(voltage_value_flash>>8);
	data[11]=(u8)(voltage_value_flash);
	
	data[12]=0x82; 
	data[13]=tem_temp_flash>>8;
	data[14]=tem_temp_flash;
	
	data[15]=0x83; 
	data[16]=(u8)(tem_humi_flash>>8);
	data[17]=(u8)(tem_humi_flash);

	data[18]=0x84; 
	data[19]=(u8)(Light_flash>>8);
	data[20]=(u8)(Light_flash);
	
	data[21]=0x85; 
	data[22]=(u8)(ultraviolet_flash>>8);
	data[23]=(u8)(ultraviolet_flash);
	datalen=24;
	if(VOC_State==ENABLE)
	{
		data[datalen++]=0x86; 
		data[datalen++]=(u8)(tvocv_flash>>8);
		data[datalen++]=(u8)(tvocv_flash);
		
//		data[datalen++]=0x87; 
//		data[datalen++]=(u8)(eco2v_flash>>8);
//		data[datalen++]=(u8)(eco2v_flash);	
	}

	if(HCHO_State==ENABLE)
	{
		data[datalen++]=0x88; 
		data[datalen++]=(u8)(formaldehyde_flash>>8);
		data[datalen++]=(u8)(formaldehyde_flash);
	}
	if(COZ_State==ENABLE)
	{	
		data[datalen++]=0x89; 
		data[datalen++]=(u8)(co2v_flash>>8);
		data[datalen++]=(u8)(co2v_flash);
	}
	SerialpayloadLength2[0]=datalen+2;//2字节包序

	memcpy(LowRF_frame_buf,SerialframeHead2,2);
	LowRF_frame_buf[2]=(Address_ID>>24);
	LowRF_frame_buf[3]=(Address_ID>>16);
	LowRF_frame_buf[4]=(Address_ID>>8);
	LowRF_frame_buf[5]=(Address_ID);
	
	
	memcpy(LowRF_frame_buf+6,sink_id2,4);//目的地址
	memcpy(LowRF_frame_buf+10,MessageType2_11,1);//消息类型
	memcpy(LowRF_frame_buf+11,Serialreserve2,1);//保留字节	
	memcpy(LowRF_frame_buf+12,SerialpayloadLength2,1);//payload长度

	memcpy(LowRF_frame_buf+13,&data,datalen); 
	
//	memcpy(LowRF_frame_buf+datalen+13,&packet_sequence_send,2); 
printf("\r\n packet_sequence_send=%d",packet_sequence_send);
	seq[0]=packet_sequence_send>>8;
	seq[1]=packet_sequence_send;
	memcpy(LowRF_frame_buf+datalen+13,&seq,2); 
	crc=crc_cal(LowRF_frame_buf+1, 14+datalen);
	SerialframeCRC[0]=(uint8_t)(crc>>8);
  SerialframeCRC[1]=(uint8_t)crc;
	
	memcpy(LowRF_frame_buf+15+datalen,SerialframeCRC,2);
  memcpy(LowRF_frame_buf+17+datalen,SerialframeTail,1);
	
	framelen=18+datalen;

//	printf("hdlc_encode and send: ");
//	for(i=0;i < framelen;i++)
//	{
//		printf(" %02X",LowRF_frame_buf[i]);
//	}
	
  Gateway_sensordata_len= hdlc_encode(LowRF_frame_buf, Gateway_sensordata_buf, framelen);
	
	
	if(Gateway_sensordata_len>80)
	{
		Gateway_sensordata_len=70;
	}
//printf("a ");
//	printf("\r\n send sm to gateway");
//	for(int k=0; k<hdlcframelen; k++ )
//		printf(" %02X", hdlc_frame_buf[k]);
		
	
	
	
//		rfrece_flag2=RF_Send_Rouse(hdlc_frame_buf,hdlcframelen);
//	
//	if(rfrece_flag2==RF_TIMEOUT)
//	{
//		printf("\r\n 88888888888888888888888888888 SX1278_RESET  8888888888888888888888888888888888");
//		Reset_sx1276();
//	}
//	else
//	{
//		RF_StartRX();//配置成接受 
//		rfrece_flag1= RF_Receive_general(RF_Rece_Buff,&RF_Rece_Lenth);
//	}
//		
//	if(rfrece_flag1==RF_SUCCESS)
//	{
//		printf(" Read_RSSI=%f",Read_RSSI()); 
//		send_event(EVENT_RF_PACK_IN,2);
////		printf("\r rece: ");
////		for(i=0;i < RF_Rece_Lenth;i++)
////		{
////			printf(" %02X",RF_Rece_Buff[i]);
////		}
////		printf("\n");
//		
//	}

}


void Send_echo_package(u8 mode)//发送注册包 mode为RF或者WIFI发送
{
	if(mode==RF_SEND)
	{
		assemble_echo_pkt2(&se2);
		send_se2(se2);
	}
	else if(mode==WIFI_SEND)
	{
		WIFI_send_echo2();
	}
	else
	{}
}

void Send_Data_package(u8 mode)//发送数据包 mode为RF或者WIFI发送
{
	if(mode==RF_SEND)
	{
		RF_Send_flash_Data();
	}
	else if(mode==WIFI_SEND)
	{
		WIFI_send_data2();
	}
	else
	{}
}

u8 check_flashdata_first(void)
{
	u8 checkflag=0;
	printf(" ck");
	Save_Addr = 256*256*save_next_addr_sect	+	256*save_next_addr_page	+	save_next_addr;
	Send_Addr = 256*256*send_next_addr_sect + 256*send_next_addr_page + send_next_addr;
	if( (Save_Addr>Send_Addr) || ( (Save_Addr<Send_Addr)&&(FLASH_full_flag) ) )  //有数据
	{
		checkflag=1;
	}
	printf(" re");
	return checkflag;
}	
u8 check_flashdata_have_or_not(void)
{
		u8 checkflag=0;
	Save_Addr = 256*256*save_next_addr_sect	+	256*save_next_addr_page	+	save_next_addr;
	Send_Addr = 256*256*send_next_addr_sect + 256*send_next_addr_page + send_next_addr;
	if( (Save_Addr>Send_Addr) || ( (Save_Addr<Send_Addr)&&(FLASH_full_flag) ) )  //有数据
	{
		if( M25P80_state==M25P80_S_OFF)
				M25P80_WAKE();
			M25P80_Read_Bytes(Send_Addr,(u8*)&FLASH_data_buf,sizeof(_FLASH_data)); 
		if(FLASH_data_buf.flagf==flash_dataflag)
		{
			checkflag=1;
		}
		else
		{
			printf("\r\n This is useless data package  has remove this package\r\n");
			out_flash();	
			checkflag=2;
		}
	}
	else
		checkflag=0;
	return checkflag;
}


u8 check_Flash_data(void)//检测FLASH是否有数据要发送
{
	u8 checkflagis;
	u8 returnflag;
	u8 cyclecheck;
	if( M25P80_state!=M25P80_S_ON)
	M25P80_WAKE();
	FLASH_Start_Busy();
		checkflagis=check_flashdata_have_or_not();
	if(checkflagis==1)
		returnflag=1;
	else if(checkflagis==0)
		returnflag=0;
	else if(checkflagis==2)
	{
		printf("\r\n start cycle\r\n");
		cyclecheck=1;
		while(cyclecheck)
		{
			checkflagis=check_flashdata_have_or_not();
			printf("\r\n start cycle checkflagis=%d\r\n",checkflagis);
			if(checkflagis==1)
			{
				returnflag=1;
				cyclecheck=0;//跳出
			}
			else if(checkflagis==0)
			{
				returnflag=0;
				cyclecheck=0;//跳出
			}
		}
	}
	
	FLASH_Free();
	return 	returnflag;
}
 uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
//    printf("\r\n pBuffer1 =%s\r\n",pBuffer1);
//    printf("\r\n pBuffer2 =%s\r\n",pBuffer2);
  }
  return 0;
}
 
uint8_t check_wifi_send_or_not(void)
{
	uint8_t flag=0;//0表示不工作  1是工作
//	WIFI_Send_TIME_Range[0]=16;
//	WIFI_Send_TIME_Range[1]=40;
//	WIFI_Send_TIME_Range[2]=16;
//	WIFI_Send_TIME_Range[3]=40;
	
//	RTC_CurrentHour=16;
//	RTC_CurrentMinute=15;
	if(WIFI_or_RF_SendMode==RF_SEND)
		return 1;
	if((WIFI_Send_TIME_Range[0]==WIFI_Send_TIME_Range[2])&&(WIFI_Send_TIME_Range[1]==WIFI_Send_TIME_Range[3]))
		return 1;
	
	if((WIFI_Send_TIME_Range[0])<(WIFI_Send_TIME_Range[2]))//比如：8点-16点
	{
		if( ( RTC_CurrentHour>WIFI_Send_TIME_Range[0]) && (RTC_CurrentHour<WIFI_Send_TIME_Range[2]))//10点
		{
			printf ("\r\n 1");
			flag=1;
		}
		else if(WIFI_Send_TIME_Range[0]==RTC_CurrentHour)//8点
		{
			if(RTC_CurrentMinute>=WIFI_Send_TIME_Range[1])
			{
				flag=1;	printf ("\r\n 2");
			}
			else
			{
				flag=0;	printf ("\r\n 3");
			}
		}
		else if(WIFI_Send_TIME_Range[2]==RTC_CurrentHour)//16点
		{
			if(RTC_CurrentMinute<=WIFI_Send_TIME_Range[3])
			{
				flag=1;	printf ("\r\n 4");
			}
			else
			{
				flag=0;	printf ("\r\n 5");
			}
		}
		else
			{
				flag=0;	printf ("\r\n 6");
			}
	}
	else if((WIFI_Send_TIME_Range[0])>(WIFI_Send_TIME_Range[2]))//比如：16点-8点
	{
		if( (RTC_CurrentHour>WIFI_Send_TIME_Range[0]) || (RTC_CurrentHour<WIFI_Send_TIME_Range[2]))//22点
		{
			flag=1;printf ("\r\n 7");
		}
		else if(WIFI_Send_TIME_Range[0]==RTC_CurrentHour)//16点
		{
			if(RTC_CurrentMinute>=WIFI_Send_TIME_Range[1])
			{
				flag=1;	printf ("\r\n 8");
			}
			else
			{
				flag=0;	printf ("\r\n 9");
			}
		}
		else if(WIFI_Send_TIME_Range[2]==RTC_CurrentHour)//8点
		{
			if(RTC_CurrentMinute<=WIFI_Send_TIME_Range[3])
			{
				flag=1;	printf ("\r\n 10");
			}
			else
			{
				flag=0;	printf ("\r\n 11");
			}
		}
		else
		{
			flag=0;	printf ("\r\n 12");
		}
	}
	else if((WIFI_Send_TIME_Range[0])==(WIFI_Send_TIME_Range[2]))//比如：16点-16点
	{
		if(WIFI_Send_TIME_Range[1]<WIFI_Send_TIME_Range[3])
		{
			if(WIFI_Send_TIME_Range[0]==RTC_CurrentHour)
			{
					if( (RTC_CurrentMinute>=WIFI_Send_TIME_Range[1]) && (RTC_CurrentMinute<WIFI_Send_TIME_Range[3]))
					{
						flag=1;	printf ("\r\n 13");
					}
					else 
					{
						flag=0;	printf ("\r\n 14");
					}
				
			}
			else
			{
				flag=0;	printf ("\r\n 15");
			}
		}
		else if(WIFI_Send_TIME_Range[1]>WIFI_Send_TIME_Range[3])
		{
			if(WIFI_Send_TIME_Range[0]==RTC_CurrentHour)
			{
					if( (RTC_CurrentMinute>=WIFI_Send_TIME_Range[1]) || (RTC_CurrentMinute<WIFI_Send_TIME_Range[3]))
					{
						flag=1;	printf ("\r\n 16");
					}
					else 
					{
						flag=0;	printf ("\r\n 17");
					}
				
			}
			else
			{
				flag=1;	printf ("\r\n 18");
			}
		}
	}
//	if(flag==1)
//		printf ("\r\n WIFI 工作");
//	else
//			printf ("\r\n WIFI 不工作");
	
	return flag;
}

 void M25P80_SLEEP(void)
 {
	 delay_ms(2);
	 M25P80_DEEP_SLEEP();
	 M25P80_state=M25P80_S_OFF;
//	 M25P80_PWEN_OFF;
	 printf("   M25P80_SLEEP");
	 
 }
 void M25P80_WAKE(void)
 {
	 M25P80_PWEN_OFF;delay_ms(3);M25P80_PWEN_ON;delay_ms(10);
	 M25P80_RES();
	 delay_ms(5);
	 M25P80_state=M25P80_S_ON;
	 printf(" M25P80_WAKE ");
 }
 
 void FLASH_Start_Busy(void)//M25P80  和  RF共同使用SPI
 {
	 OS_ERR err;
	 	u8 whilenum=6;
		 while(M25P80_runstate==M25P80_BUSY)
		{
			OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
//			vTaskDelay(10);
			printf("\n *****M25P80_BUSY****");
			whilenum--;
			if(whilenum==0)
				break;
		}
		M25P80_runstate=M25P80_BUSY;
 }
  void FLASH_Free(void)
 {
	 M25P80_runstate=M25P80_FREE;
 }
 
   void Peripher_sleep(void)//休眠传感器 RF FLASH  Note_wx  需要修改
 { 
//	 DEBUG_UART_DeInit();
	 if( PERIPHER_state!=PERIPHER_S_OFF)
	 {
		 PERIPHER_state=PERIPHER_S_OFF;
		 RF_STATE=RF_INIT_OFF;
		 M25P80_state=M25P80_S_OFF;
	//	delay_ms(100);
		SPI_DeInit();
		WIFI_PWEN_OFF;
		SENSE_PWEN_OFF;
//		USART1_UART_DeInit();
//		HAL_GPIO_DeInit(SCL_GPIO,SCL_PIN|SDA_PIN); 
//		HAL_GPIO_DeInit(M25P80_NSS_GPIO, M25P80_NSS_PIN );
//		HAL_GPIO_DeInit(LED_GPIO,LED_PIN); 	
//		HAL_GPIO_DeInit(M25P80_PWEN_GPIO,M25P80_PWEN_PIN);//PWEN SENSE　and M25P80 
//		HAL_GPIO_DeInit(SENSE_PWEN_GPIO,SENSE_PWEN_PIN);		 
//		#ifdef EZ265
//		HAL_GPIO_DeInit(LORA_PWEN_GPIO,LORA_PWEN_PIN);
//		HAL_GPIO_DeInit(DIO0_IOPORT,DIO0_PIN);
//		HAL_GPIO_DeInit(DIO1_IOPORT,DIO1_PIN);
//		HAL_GPIO_DeInit(DIO2_IOPORT,DIO2_PIN);
//		HAL_GPIO_DeInit(DIO3_IOPORT,DIO3_PIN);
//		HAL_GPIO_DeInit(DIO4_IOPORT,DIO4_PIN);
//		HAL_GPIO_DeInit(DIO5_IOPORT,DIO5_PIN);
//		HAL_GPIO_DeInit(NSS_IOPORT,NSS_PIN);
//		HAL_GPIO_DeInit(RESET_IOPORT,RESET_PIN); 
//		
//		HAL_GPIO_DeInit(WIFI_PWEN_GPIO,WIFI_PWEN_PIN); 
//		HAL_GPIO_DeInit(WIFI_NETWORK_CONNECT_GPIO,WIFI_NETWORK_CONNECT_PIN);
//		#endif
//		 
//		#ifdef EZ267
//		 HAL_GPIO_DeInit(NB_PWEN_GPIO,NB_PWEN_PIN);
//		 
//		#endif
//		printf("\n Peripher_sleep2");
	}
 }
   void Peripher_on(void)//休眠传感器 RF FLASH
 {
	 if( PERIPHER_state!=PERIPHER_S_ON)
	 {
//		  __HAL_RCC_GPIOC_CLK_ENABLE();
//			__HAL_RCC_GPIOB_CLK_ENABLE();
//			__HAL_RCC_GPIOA_CLK_ENABLE();
//		 	__HAL_RCC_GPIOD_CLK_ENABLE();
//			__HAL_RCC_GPIOE_CLK_ENABLE();
//			__HAL_RCC_GPIOH_CLK_ENABLE();
		 
//		 DEBUG_UART_ReInit();
//			SYS_GPIO_Init();
//			M25P80_GPIO_Init();

//			SPI_Init( );	
			SENSE_PWEN_OFF;
		 M25P80_PWEN_ON;
		
			M25P80_SLEEP();
		 #ifdef EZ265
			if(WIFI_or_RF_SendMode==RF_SEND)
			{
				SX1276InitIo();
				LORA_PWEN_init();
//				RF_Init();
			}
			else if(WIFI_or_RF_SendMode==WIFI_SEND)
			{
				WIFI_GPIO_Init();
				WIFI_UART_ReInit();
//				DEBUG_UART_ReInit();
//				printf("\r\n MX_USART4_UART_Init \r\n");
				
			}
			#endif
			
			#ifdef EZ267
			
			#endif
			
		 PERIPHER_state=PERIPHER_S_ON;
		printf("\n Peripher_on1");
//		 delay_ms(10);
	//		printf("\r\n I2C SRI USART 34DeInit \r\n");
	 }
 }
  

/******end******/
