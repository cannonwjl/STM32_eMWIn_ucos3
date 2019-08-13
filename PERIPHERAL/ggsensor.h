#ifndef __GGSENSOR_H
#define __GGSENSOR_H

#include "Platform_Configure.h"
//#include "main.h"
#include "string.h"
#include "stdlib.h"

#include "timer.h"
//#include "rtc.h"
#include "LowRFM.h"

extern uint16_t tvocv,eco2v;
extern float formaldehyde;
extern uint16_t co2v;
extern uint16_t PM2_5_flash,PM10_flash;
extern int16_t tem_temp;//温度有负数
extern uint16_t tem_humi,voltage_value;//传感器参数 --温度 湿度 电压
extern uint16_t Light;//传感器参数 --光照
extern uint16_t ultraviolet;//紫外
extern uint16_t PM2_5,PM10;

extern float tem_temp_displayf,tem_humi_displayf;
extern uint16_t tem_temp_display,tem_humi_display;

extern uint8_t Gateway_echodata_buf[58];
extern uint8_t Gateway_echodata_len;
extern uint8_t Gateway_sensordata_buf[58];
extern uint8_t Gateway_sensordata_len;
extern u8 sta[70];
extern	u8 staip[100];
extern	u8 sock[70];
extern  u8 Is_register_flag;//注册是否成功标志位 0不成功 1表示成功
extern u32 CollectTime;//传感器采集时间戳

extern u16 packet_sequence_send;

extern u8 RF_Rece_Buff[200];//SX1278 收到数
extern u16 RF_Rece_Lenth;
//	u8  rf_param[6]={(u8)(433>>8),(u8)433,20,8,7,2};//433参数
extern	u32 collection_time;		//数据采集周期 S
extern u32 send_time;		//数据发送周期  S
extern	u8  display_mode;					//液晶屏打开|关闭  1|0
extern  u32 Address_ID;//节点自身的地址
extern	u8 retry_Register_num;//注册不成功 重发注册包的次数 
extern	u16  Rf_fequence;//433参数
extern	u8 WIFI_or_RF_SendMode;//发送模式选择 必须写入flash
extern	u16  collection_time_min;		//数据采集周期 min
extern u16  send_time_min;	
extern	u16 Re_Registration_time_min;//断网之后需要重新注册的  发送包的周期 重要参数
extern	u16 lcd_time_min;//液晶屏刷新周期
extern  u16 GettimeEcho_time_Hour; //== GettimeEcho_time/3600

extern u32 GettimeEcho_time;  //新版协议获取时间注册的周期

extern u8 VOC_State; 
extern u16 VOC_collection_time_min;
extern u32 VOC_collection_time;
extern	u8 HCHO_State; //HCHO是否开启
extern	u16 HCHO_collection_time_min;
extern	u32 HCHO_collection_time;	
extern	u8 COZ_State; //COZ是否开启
extern	u16 COZ_collection_time_min;
extern u32 COZ_collection_time;
extern	u8 Wait_WIFI_ConnectTime;

extern	uint8_t HCHO_NeedCollectTime;//?”2é??é??é??é?�è|?????—?é—′
extern	uint8_t COZ_NeedCollectTime;//CO2é??é??é?�è|?????—?é—′
extern	u16 VOC_collect_need_time;//é??è|??????°
	
	
extern u8 SysStart_mode_flag;
	
extern u32 Last_SendTime;//上次一次发送时间戳
extern u32 Next_SendTime;//下一次发送时间戳
extern u32 Last_CollectTime;//上次一次传感器采集时间戳
extern u32 Next_CollectTime;//下一次传感器采集时间戳

extern u32 Last_SendpacketTime;//上一次发包时间
extern u32 Next_SendpacketTime;//下一次发包时间
extern u32 Last_LcdrefreshTime;//上一次液晶刷新时间
extern u32 Next_LcdrefreshTime;//下一次液晶刷新时间
extern u8 LCD_DISPLAY_SWITCH_FLAG;
extern u32 Last_LcdCollectTime;
extern u32 Next_LcdCollectTime;

extern u32 Last_HchoCollectTime;
extern u32 Next_HchoCollectTime;
extern u8 HCHO_start_collect_flag;
extern u32 HCHO_startCollect_time;

extern u32 Last_CozCollectTime;
extern u32 Next_CozCollectTime;
extern u8 COZ_start_collect_flag;
extern u32 COZ_startCollect_time;

extern u32 Last_VocCollectTime;
extern u32 Next_VocCollectTime;
extern u8 VOC_start_collect_flag;
extern u32 VOC_startCollect_time;

extern u32 lcd_time;//液晶屏刷新周期
extern u8 check_wifi_send_or_not_flag;//WIFI是否工作标志位

extern  u32 NB_first_down_waittime;
extern	u8 NB_echo_sec;//NB注册等待时间   单位S
extern	u32 NB_seng_time;//  单位ms  NB发送时等待回包时间
extern u8 NB_node_or_weather_mode;
extern float tem_temp_displayf,tem_humi_displayf;
//void basic_init(void);
void parameter_init(void);
u8 sensor_init(void);
void sensor_disp(void);//液晶屏显示 采集后显示

void All_sensor_collect(void);//温度湿度 电压 时间等

enum SYS_STAT {

	SEARCH_NET_STAT=0,
  SYN_WORK_STAT=1,
  UNSYN_WORK_STAT=2,
	PREPARE_TO_SLEEP=3,
};

enum {

	RF_PACK_ECHO=0x02,
  RF_PACK_DATA=0x01,
  RF_PACK_ACK=0x10,
};

__packed typedef struct {
    u32 flag;
		u8 Save_next_addr;				//该页存放的地址0
	  u16 Save_next_addr_page;		//扇区第多少页1
	  u8 Save_next_addr_sect;   //存放在第几个扇区2

  	u8 Send_next_addr;			//该页存放的下一个要发送的地址3
  	u16 Send_next_addr_page;	//要发送的数据在第几个页4
	  u8 Send_next_addr_sect;	//要发送的数据在第几个扇区5

    u8 	fullflag;
} _EEPROM_pos;

__packed typedef struct {
    u32 flagp;
		u16  Rf_feq;//433参数
		u16  Collection_time_Min;	//数据采集周期
		
		u32 Address;//区域地址 
		u8 Retry_Send_num;//重发包的次数
		u8 WIFI_RF_Mode;
		u16 GetTimeEcho_Time_Hour;//新版协议获取时间注册的周期 单位：小时
		u16 Re_Registration_Time_Min;//断网之后需要重新注册的周期
		u8 Wait_Wifi_ConnectTime;//等待WIFI连上服务器的时间  *100ms
		u8  Display_mode;	//液晶屏打开|关闭
		u16 Lcd_time_Min;//液晶屏刷新周期
		u8 Ccs811_mode;
		u16 Ccs811_time_Min;
		u8 Hcho_mode;
		u16 hcho_time_Min;
		u8 Coz_mode;
		u16 Coz_time_Min;
	
		u8 Sta[70];
		u8 Staip[100];
		u8 Sock[70];
    
		u8 Wifi_Send_TimeRange[4];
	
} _EEPROM_param;
__packed typedef struct {
    u8 flagp;
		u16 Coz_value;
} _EEPROM_COZdata;

__packed typedef struct {
    u8 flagp;
		u16 Light_value;
} _EEPROM_LIGHTdata;


__packed typedef struct {
    u32 flagf;
		u32 Timestamp;//传感器参数 --时间戳
		int16_t Tem_temp;
  	uint16_t Tem_humi;
	  uint16_t Voltage_value;//传感器参数 --温度 湿度 电压
		uint16_t Light_intensity;//传感器参数 --光照
		uint16_t SeqNo;
		uint16_t Ultraviolet;
		uint16_t TVOC;
		uint16_t eCO2;
		uint16_t Formaldehyde;
		uint16_t CO2;
		uint16_t Pm2_5;
		uint16_t Pm10;
} _FLASH_data;


typedef struct {
	uint8_t pre[3];
    uint8_t dest[2];
    uint8_t lsadrr[2];
    uint8_t len;
    uint8_t groupID;
    uint8_t amtype;
}FrameHead;

typedef struct {
    uint8_t crc[2];
    uint8_t end; 
}FrameEnd;

typedef struct{ 		//V2.2版协议的报头部分数据包
    u8 frame[2];  	// 节点给网关 7E 45   网关给节点消息 7E 44   
    u8 lsadrr[4];		//链接源地址
    u8 dtadrr[4];		//目的地址0xFFFFFFFF WiFI为0x00000000
    u8 subtype;			//消息类型 0x10(节点注册) 0x11(节点数据包) 0x12(网关回节点注册)	 0x13(网关回节点数据包)		
    u8 Reserve;			//保留字节			
    u8 payloadlen;	//payload 长度		
		u8 seadrr[4];		//采集节点源地址
		uint8_t crc[2];	//crc校验
		uint8_t end;		//结束 0x7e
}MsgHeaderV2;


typedef struct {						
    uint8_t voltage[2];			//节点电压
    uint8_t	hop;						//路由使用
    uint8_t	reserve[2];  		//备用信息
    uint8_t packetnum[2];  	//循环编号
}SensorEchoV2;

typedef struct {						//节点起始注册包 0x10
		uint8_t frame[2];  	// 节点给网关 7E 45   网关给节点消息 7E 44   
    uint8_t lsadrr[4];		//链接源地址
    uint8_t dtadrr[4];		//目的地址0xFFFFFFFF WiFI为0x00000000
    uint8_t subtype;			//消息类型 0x10(节点注册) 0x11(节点数据包) 0x12(网关回节点注册)	 0x13(网关回节点数据包)		
    uint8_t Reserve;			//保留字节			
    uint8_t payloadlen;	//payload 长度		
		uint8_t seadrr[4];		//采集节点源地址 
		SensorEchoV2 data;
		uint8_t crc[2];	//crc校验
		uint8_t end;		//结束 0x7e
}SensorEchoV2_All;


typedef struct {						//传感器数据包  0x11  有什么发什么 
    uint8_t hop;						//路由使用
    uint8_t timestamp[4];		//时间戳
    uint8_t sensortype1;		//传感器类型 0x81 点压  (传感器类型数据 高两位表示长度 低六位表示类型)
    uint8_t voltage[2];			
    uint8_t	sensortype2;		//传感器类型 0x82 温度
    uint8_t	temperature[2]; 
		uint8_t sensortype3;		//传感器类型 0x83 湿度
    uint8_t humidity[2];		
    uint8_t sensortype4;		//传感器类型 0x84 光照
		uint8_t light[2];  			
    uint8_t sensortype5; 		//传感器类型 0x85 紫外 
		uint8_t ultraviole[2];			
		uint8_t sensortype6;		//传感器类型 0x86 TVOC
		uint8_t tvoc[2];
		uint8_t sensortype7;		//传感器类型 0x87 eCO2
		uint8_t eco2[2];
		uint8_t sensortype8;		//传感器类型 0x88 甲醛
		uint8_t formaldehyde[2];
		uint8_t packetnum[2];		//循环包编号

} SensorMsgV2;
typedef struct {						//传感器数据包  0x11  有什么发什么 
    uint8_t hop;						//路由使用
    uint8_t timestamp[4];		//时间戳
    uint8_t sensortype1;		//传感器类型 0x81 点压  (传感器类型数据 高两位表示长度 低六位表示类型)
    uint8_t voltage[2];			
    uint8_t	sensortype2;		//传感器类型 0x82 温度
    uint8_t	temperature[2]; 
		uint8_t sensortype3;		//传感器类型 0x83 湿度
    uint8_t humidity[2];		
    uint8_t sensortype4;		//传感器类型 0x84 光照
		uint8_t light[2];  			

		uint8_t packetnum[2];		//循环包编号

} SensorMsgV2NoCCS811;

typedef struct{							//网关回应节点注册包  0x12
    uint8_t timestamp[4];		//时间戳				
    uint8_t commandtype1;		//命令类型 0xC1 采样间隔  (110开始 低5位表示指令)
    uint8_t samplingper[2];
    uint8_t commandtype2;	  //命令类型 0xC2 节点通信周期
    uint8_t transmitper[2];
    uint8_t reserve[2];			//保留字节
	  uint8_t packetnum[2];		//同注册包编号
}MsgAckEchoV2;

typedef struct {						//网关回应节点数据包  0x13
  uint8_t commandtype1;			//命令类型 0xC1 采样间隔 
  uint8_t samplingper[2];		
  uint8_t commandtype2;			//命令类型 0xC2 节点通信周期
  uint8_t transmitper[2];
  uint8_t	packetnum[2];			//同数据包编号
}GateWayAckMsgV2;


typedef struct{ 		//V2.2版协议的报头部分数据包 （网关与服务器）
    u8 frame[2];  	// 网关给服务器 7E 45   服务器回复网关 7E 44   
    u8 lsadrr[4];		//网关源地址
    u8 dtadrr[4];		//服务器地址0x00000000
    u8 subtype;			//消息类型 0x14(网关注册) 0x15(网关数据包) 0x16(服务器回应网关注册)	 0x17(服务器回应网关数据包)		
    u8 Reserve;			//保留字节			
    u8 payloadlen;	//payload 长度		
}GSMsgHeaderV2;

typedef struct {						//网关起始注册包 0x14
		GSMsgHeaderV2 GSM;
    uint8_t voltage[2];			//网关电压
    uint8_t	reserve[2];  		//备用信息
    uint8_t packetnum[2];  	//循环编号
		uint8_t crc[2];
		uint8_t end;						//0x7e
}GatewayEchoV2;


typedef struct {	
		uint8_t sensoraddr[4];	//采集节点地址
    uint8_t hop;						//路由使用
    uint8_t timestamp[4];		//时间戳
    uint8_t sensortype1;		//传感器类型 0x81 点压  (传感器类型数据 高两位表示长度 低六位表示类型)
    uint8_t voltage[2];			
    uint8_t	sensortype2;		//传感器类型 0x82 温度
    uint8_t	temperature[2]; 
		uint8_t sensortype3;		//传感器类型 0x83 湿度
    uint8_t humidity[2];		
    uint8_t sensortype4;		//传感器类型 0x84 光照
		uint8_t light[2];  			
//    uint8_t sensortype5; 		//传感器类型 0x85 紫外 
//		uint8_t ultraviole[2];			
		uint8_t sensortype6;		//传感器类型 0x86 TVOC
		uint8_t tvoc[2];
		uint8_t sensortype7;		//传感器类型 0x87 eCO2
		uint8_t eco2[2];
//		uint8_t sensortype8;		//传感器类型 0x88 甲醛
//		uint8_t formaldehyde[2];
		uint8_t nodpacketnum[2];//节点包循环包编号
} GatewaydataV2;
typedef struct {	
		uint8_t sensoraddr[4];	//采集节点地址
    uint8_t hop;						//路由使用
    uint8_t timestamp[4];		//时间戳
    uint8_t sensortype1;		//传感器类型 0x81 点压  (传感器类型数据 高两位表示长度 低六位表示类型)
    uint8_t voltage[2];			
    uint8_t	sensortype2;		//传感器类型 0x82 温度
    uint8_t	temperature[2]; 
		uint8_t sensortype3;		//传感器类型 0x83 湿度
    uint8_t humidity[2];		
    uint8_t sensortype4;		//传感器类型 0x84 光照
		uint8_t light[2];  			
		uint8_t nodpacketnum[2];//节点包循环包编号
} GatewaydataV2NoCCS;
typedef struct {						//传感器数据包  0x15
	
		GSMsgHeaderV2 GSM;
		uint8_t gwvoltage[2];		//网关电压
		GatewaydataV2 GWD;
		uint8_t packetnum[2];		//包循环包编号
		uint8_t crc[2];
		uint8_t end;						//0x7e
} GatewayMsgV2;
typedef struct {						//传感器数据包  0x15
	
		GSMsgHeaderV2 GSM;
		uint8_t gwvoltage[2];		//网关电压
		GatewaydataV2NoCCS GWD;
		uint8_t packetnum[2];		//包循环包编号
		uint8_t crc[2];
		uint8_t end;						//0x7e
} GatewayMsgV2NoCCS;

 typedef struct{							//服务器回应网关注册包  0x16
	GSMsgHeaderV2 GSM;
    uint8_t timestamp[4];		//时间戳				
    uint8_t commandtype1;		//命令类型 0xC1 采样间隔  (110开始 低5位表示指令)
    uint8_t samplingper[2];
    uint8_t commandtype2;	  //命令类型 0xC2 节点通信周期
    uint8_t transmitper[2];
		uint8_t commmandtype3;	//命令类型 0xC3 网关通信周期
		uint8_t gawtransper[2];	
    uint8_t reserve[2];			//保留字节
	  uint8_t packetnum[2];		//同注册包编号
		uint8_t crc[2];
		uint8_t end;						//0x7e
}ServeAckEchoV2;
 typedef struct{							//服务器回应网关注册包  0x17
		GSMsgHeaderV2 GSM;		
    uint8_t commandtype1;		//命令类型 0xC1 采样间隔  (110开始 低5位表示指令)
    uint8_t samplingper[2];
    uint8_t commandtype2;	  //命令类型 0xC2 节点通信周期
    uint8_t transmitper[2];
		uint8_t commmandtype3;	//命令类型 0xC3 网关通信周期
		uint8_t gawtransper[2];	
    uint8_t reserve[2];			//保留字节
	  uint8_t packetnum[2];		//同注册包编号
		uint8_t crc[2];
		uint8_t end;						//0x7e
}ServeAckMsgV2;
typedef union {
	unsigned long  timetick32;
	uint8_t  timetick[4];
} uint8_to_uint32;


enum M25P80_STATE
{
	M25P80_S_OFF=0,// 上电默认模式 M25P80不能读写
	M25P80_S_ON=1,

};
enum M25P80_RUNSTATE
{
	M25P80_BUSY=1,
	M25P80_FREE=0,	
};
enum RF_INIT_STATE
{
	RF_INIT_OFF=0,// 上电默认模式 M25P80不能读写
	RF_INIT_ON=1,
};
enum PERIPHER_STATE
{
	PERIPHER_S_OFF=0,// 上电默认模式 M25P80不能读写
	PERIPHER_S_ON=1,
};
enum RF_STATE
{
	RF_S_FSEND=1,//一般模式 
	RF_S_LISTEN=2,//省电模式 监听
	RF_S_SSEND=3,//唤醒模式
	RF_S_IDLE=0,//休眠模式  上电默认模式
};
enum wifi_ptype
{
	WIFI_PACK_ECHO=0x07,
	WIFI_PACK_DATA1=0x01,
  WIFI_PACK_DATA2=0x02	
};
enum send_mode
{
	WIFI_SEND=0x01,
	RF_SEND=0x02,
};
enum NB_mode
{
	NORMAL_NODE=0x01,//正常节点
	WEATHER_STATION=0x02,//气象站
};
void ggsensor_test(void);
void sensor_prfint(void);
 uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void write_startmode_normal_eeprom(void);
void Init_pos_eeprom(void);
void Read_pos_eeprom(void);
u8 Write_pos_eeprom(void);
u8 Write_pos_eeprom_Clear(void);
void Read_param_eeprom(void);
u8 Wirte_param_eeprom(void);
void Init_param_eeprom(void);

u8 out_flash(void);//数据从FLASH读出  发送地址改变一次
u8 in_flash(void);//数据写入FLASH  写入地址改变一次
u8 Write_Data_to_Flash(void);//将采集的传感器数据写入FLASH
u8 Read_Data_from_Flash(void);//从FLASH读出要发送的数据
u8 Read_Data_from_RAM(void);
void RF_Send_flash_Data(void);
u8 check_Flash_data(void);//检测FLASH是否有数据要发送
u8 check_flashdata_first(void);//第一次检查
//u8 check_WIFI_pack_single(char *LowRF_input_data,uint8_t len);/////解析WIFI 收到的单个包


u8 Read_Data_from_eeprom_last(void);//从EEPROM读出上次的CO2
u8 Write_Data_from_eeprom_last(void);//从EEPROM写入上次的CO2
u16 Read_Data_from_eeprom_lastLight(void);
u8 Write_Data_from_eeprom_lastLight(void);
 void Peripher_sleep(void);
 void Peripher_on(void);
//// void sensor_wake(void);
 
 void M25P80_SLEEP(void);
 void M25P80_WAKE(void);
void FLASH_Start_Busy(void);
void FLASH_Free(void);
void assemble_echo_pkt2(SensorEchoV2 *septr);
void send_se2(SensorEchoV2 septr1);
void WIFI_send_data2(void);
void Send_echo_package(u8 mode);
void Send_Data_package(u8 mode);
void NB_send_data_weather(void);

uint8_t check_wifi_send_or_not(void);

void I2C_PWEN_Init(void);
void I2C_PWEN_DeInit(void);

#endif
