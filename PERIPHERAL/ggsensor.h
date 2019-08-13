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
extern int16_t tem_temp;//�¶��и���
extern uint16_t tem_humi,voltage_value;//���������� --�¶� ʪ�� ��ѹ
extern uint16_t Light;//���������� --����
extern uint16_t ultraviolet;//����
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
extern  u8 Is_register_flag;//ע���Ƿ�ɹ���־λ 0���ɹ� 1��ʾ�ɹ�
extern u32 CollectTime;//�������ɼ�ʱ���

extern u16 packet_sequence_send;

extern u8 RF_Rece_Buff[200];//SX1278 �յ���
extern u16 RF_Rece_Lenth;
//	u8  rf_param[6]={(u8)(433>>8),(u8)433,20,8,7,2};//433����
extern	u32 collection_time;		//���ݲɼ����� S
extern u32 send_time;		//���ݷ�������  S
extern	u8  display_mode;					//Һ������|�ر�  1|0
extern  u32 Address_ID;//�ڵ�����ĵ�ַ
extern	u8 retry_Register_num;//ע�᲻�ɹ� �ط�ע����Ĵ��� 
extern	u16  Rf_fequence;//433����
extern	u8 WIFI_or_RF_SendMode;//����ģʽѡ�� ����д��flash
extern	u16  collection_time_min;		//���ݲɼ����� min
extern u16  send_time_min;	
extern	u16 Re_Registration_time_min;//����֮����Ҫ����ע���  ���Ͱ������� ��Ҫ����
extern	u16 lcd_time_min;//Һ����ˢ������
extern  u16 GettimeEcho_time_Hour; //== GettimeEcho_time/3600

extern u32 GettimeEcho_time;  //�°�Э���ȡʱ��ע�������

extern u8 VOC_State; 
extern u16 VOC_collection_time_min;
extern u32 VOC_collection_time;
extern	u8 HCHO_State; //HCHO�Ƿ���
extern	u16 HCHO_collection_time_min;
extern	u32 HCHO_collection_time;	
extern	u8 COZ_State; //COZ�Ƿ���
extern	u16 COZ_collection_time_min;
extern u32 COZ_collection_time;
extern	u8 Wait_WIFI_ConnectTime;

extern	uint8_t HCHO_NeedCollectTime;//?��2��??��??��??��?���|?????��?������
extern	uint8_t COZ_NeedCollectTime;//CO2��??��??��?���|?????��?������
extern	u16 VOC_collect_need_time;//��??��|??????��
	
	
extern u8 SysStart_mode_flag;
	
extern u32 Last_SendTime;//�ϴ�һ�η���ʱ���
extern u32 Next_SendTime;//��һ�η���ʱ���
extern u32 Last_CollectTime;//�ϴ�һ�δ������ɼ�ʱ���
extern u32 Next_CollectTime;//��һ�δ������ɼ�ʱ���

extern u32 Last_SendpacketTime;//��һ�η���ʱ��
extern u32 Next_SendpacketTime;//��һ�η���ʱ��
extern u32 Last_LcdrefreshTime;//��һ��Һ��ˢ��ʱ��
extern u32 Next_LcdrefreshTime;//��һ��Һ��ˢ��ʱ��
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

extern u32 lcd_time;//Һ����ˢ������
extern u8 check_wifi_send_or_not_flag;//WIFI�Ƿ�����־λ

extern  u32 NB_first_down_waittime;
extern	u8 NB_echo_sec;//NBע��ȴ�ʱ��   ��λS
extern	u32 NB_seng_time;//  ��λms  NB����ʱ�ȴ��ذ�ʱ��
extern u8 NB_node_or_weather_mode;
extern float tem_temp_displayf,tem_humi_displayf;
//void basic_init(void);
void parameter_init(void);
u8 sensor_init(void);
void sensor_disp(void);//Һ������ʾ �ɼ�����ʾ

void All_sensor_collect(void);//�¶�ʪ�� ��ѹ ʱ���

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
		u8 Save_next_addr;				//��ҳ��ŵĵ�ַ0
	  u16 Save_next_addr_page;		//�����ڶ���ҳ1
	  u8 Save_next_addr_sect;   //����ڵڼ�������2

  	u8 Send_next_addr;			//��ҳ��ŵ���һ��Ҫ���͵ĵ�ַ3
  	u16 Send_next_addr_page;	//Ҫ���͵������ڵڼ���ҳ4
	  u8 Send_next_addr_sect;	//Ҫ���͵������ڵڼ�������5

    u8 	fullflag;
} _EEPROM_pos;

__packed typedef struct {
    u32 flagp;
		u16  Rf_feq;//433����
		u16  Collection_time_Min;	//���ݲɼ�����
		
		u32 Address;//�����ַ 
		u8 Retry_Send_num;//�ط����Ĵ���
		u8 WIFI_RF_Mode;
		u16 GetTimeEcho_Time_Hour;//�°�Э���ȡʱ��ע������� ��λ��Сʱ
		u16 Re_Registration_Time_Min;//����֮����Ҫ����ע�������
		u8 Wait_Wifi_ConnectTime;//�ȴ�WIFI���Ϸ�������ʱ��  *100ms
		u8  Display_mode;	//Һ������|�ر�
		u16 Lcd_time_Min;//Һ����ˢ������
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
		u32 Timestamp;//���������� --ʱ���
		int16_t Tem_temp;
  	uint16_t Tem_humi;
	  uint16_t Voltage_value;//���������� --�¶� ʪ�� ��ѹ
		uint16_t Light_intensity;//���������� --����
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

typedef struct{ 		//V2.2��Э��ı�ͷ�������ݰ�
    u8 frame[2];  	// �ڵ������ 7E 45   ���ظ��ڵ���Ϣ 7E 44   
    u8 lsadrr[4];		//����Դ��ַ
    u8 dtadrr[4];		//Ŀ�ĵ�ַ0xFFFFFFFF WiFIΪ0x00000000
    u8 subtype;			//��Ϣ���� 0x10(�ڵ�ע��) 0x11(�ڵ����ݰ�) 0x12(���ػؽڵ�ע��)	 0x13(���ػؽڵ����ݰ�)		
    u8 Reserve;			//�����ֽ�			
    u8 payloadlen;	//payload ����		
		u8 seadrr[4];		//�ɼ��ڵ�Դ��ַ
		uint8_t crc[2];	//crcУ��
		uint8_t end;		//���� 0x7e
}MsgHeaderV2;


typedef struct {						
    uint8_t voltage[2];			//�ڵ��ѹ
    uint8_t	hop;						//·��ʹ��
    uint8_t	reserve[2];  		//������Ϣ
    uint8_t packetnum[2];  	//ѭ�����
}SensorEchoV2;

typedef struct {						//�ڵ���ʼע��� 0x10
		uint8_t frame[2];  	// �ڵ������ 7E 45   ���ظ��ڵ���Ϣ 7E 44   
    uint8_t lsadrr[4];		//����Դ��ַ
    uint8_t dtadrr[4];		//Ŀ�ĵ�ַ0xFFFFFFFF WiFIΪ0x00000000
    uint8_t subtype;			//��Ϣ���� 0x10(�ڵ�ע��) 0x11(�ڵ����ݰ�) 0x12(���ػؽڵ�ע��)	 0x13(���ػؽڵ����ݰ�)		
    uint8_t Reserve;			//�����ֽ�			
    uint8_t payloadlen;	//payload ����		
		uint8_t seadrr[4];		//�ɼ��ڵ�Դ��ַ 
		SensorEchoV2 data;
		uint8_t crc[2];	//crcУ��
		uint8_t end;		//���� 0x7e
}SensorEchoV2_All;


typedef struct {						//���������ݰ�  0x11  ��ʲô��ʲô 
    uint8_t hop;						//·��ʹ��
    uint8_t timestamp[4];		//ʱ���
    uint8_t sensortype1;		//���������� 0x81 ��ѹ  (�������������� ����λ��ʾ���� ����λ��ʾ����)
    uint8_t voltage[2];			
    uint8_t	sensortype2;		//���������� 0x82 �¶�
    uint8_t	temperature[2]; 
		uint8_t sensortype3;		//���������� 0x83 ʪ��
    uint8_t humidity[2];		
    uint8_t sensortype4;		//���������� 0x84 ����
		uint8_t light[2];  			
    uint8_t sensortype5; 		//���������� 0x85 ���� 
		uint8_t ultraviole[2];			
		uint8_t sensortype6;		//���������� 0x86 TVOC
		uint8_t tvoc[2];
		uint8_t sensortype7;		//���������� 0x87 eCO2
		uint8_t eco2[2];
		uint8_t sensortype8;		//���������� 0x88 ��ȩ
		uint8_t formaldehyde[2];
		uint8_t packetnum[2];		//ѭ�������

} SensorMsgV2;
typedef struct {						//���������ݰ�  0x11  ��ʲô��ʲô 
    uint8_t hop;						//·��ʹ��
    uint8_t timestamp[4];		//ʱ���
    uint8_t sensortype1;		//���������� 0x81 ��ѹ  (�������������� ����λ��ʾ���� ����λ��ʾ����)
    uint8_t voltage[2];			
    uint8_t	sensortype2;		//���������� 0x82 �¶�
    uint8_t	temperature[2]; 
		uint8_t sensortype3;		//���������� 0x83 ʪ��
    uint8_t humidity[2];		
    uint8_t sensortype4;		//���������� 0x84 ����
		uint8_t light[2];  			

		uint8_t packetnum[2];		//ѭ�������

} SensorMsgV2NoCCS811;

typedef struct{							//���ػ�Ӧ�ڵ�ע���  0x12
    uint8_t timestamp[4];		//ʱ���				
    uint8_t commandtype1;		//�������� 0xC1 �������  (110��ʼ ��5λ��ʾָ��)
    uint8_t samplingper[2];
    uint8_t commandtype2;	  //�������� 0xC2 �ڵ�ͨ������
    uint8_t transmitper[2];
    uint8_t reserve[2];			//�����ֽ�
	  uint8_t packetnum[2];		//ͬע������
}MsgAckEchoV2;

typedef struct {						//���ػ�Ӧ�ڵ����ݰ�  0x13
  uint8_t commandtype1;			//�������� 0xC1 ������� 
  uint8_t samplingper[2];		
  uint8_t commandtype2;			//�������� 0xC2 �ڵ�ͨ������
  uint8_t transmitper[2];
  uint8_t	packetnum[2];			//ͬ���ݰ����
}GateWayAckMsgV2;


typedef struct{ 		//V2.2��Э��ı�ͷ�������ݰ� ���������������
    u8 frame[2];  	// ���ظ������� 7E 45   �������ظ����� 7E 44   
    u8 lsadrr[4];		//����Դ��ַ
    u8 dtadrr[4];		//��������ַ0x00000000
    u8 subtype;			//��Ϣ���� 0x14(����ע��) 0x15(�������ݰ�) 0x16(��������Ӧ����ע��)	 0x17(��������Ӧ�������ݰ�)		
    u8 Reserve;			//�����ֽ�			
    u8 payloadlen;	//payload ����		
}GSMsgHeaderV2;

typedef struct {						//������ʼע��� 0x14
		GSMsgHeaderV2 GSM;
    uint8_t voltage[2];			//���ص�ѹ
    uint8_t	reserve[2];  		//������Ϣ
    uint8_t packetnum[2];  	//ѭ�����
		uint8_t crc[2];
		uint8_t end;						//0x7e
}GatewayEchoV2;


typedef struct {	
		uint8_t sensoraddr[4];	//�ɼ��ڵ��ַ
    uint8_t hop;						//·��ʹ��
    uint8_t timestamp[4];		//ʱ���
    uint8_t sensortype1;		//���������� 0x81 ��ѹ  (�������������� ����λ��ʾ���� ����λ��ʾ����)
    uint8_t voltage[2];			
    uint8_t	sensortype2;		//���������� 0x82 �¶�
    uint8_t	temperature[2]; 
		uint8_t sensortype3;		//���������� 0x83 ʪ��
    uint8_t humidity[2];		
    uint8_t sensortype4;		//���������� 0x84 ����
		uint8_t light[2];  			
//    uint8_t sensortype5; 		//���������� 0x85 ���� 
//		uint8_t ultraviole[2];			
		uint8_t sensortype6;		//���������� 0x86 TVOC
		uint8_t tvoc[2];
		uint8_t sensortype7;		//���������� 0x87 eCO2
		uint8_t eco2[2];
//		uint8_t sensortype8;		//���������� 0x88 ��ȩ
//		uint8_t formaldehyde[2];
		uint8_t nodpacketnum[2];//�ڵ��ѭ�������
} GatewaydataV2;
typedef struct {	
		uint8_t sensoraddr[4];	//�ɼ��ڵ��ַ
    uint8_t hop;						//·��ʹ��
    uint8_t timestamp[4];		//ʱ���
    uint8_t sensortype1;		//���������� 0x81 ��ѹ  (�������������� ����λ��ʾ���� ����λ��ʾ����)
    uint8_t voltage[2];			
    uint8_t	sensortype2;		//���������� 0x82 �¶�
    uint8_t	temperature[2]; 
		uint8_t sensortype3;		//���������� 0x83 ʪ��
    uint8_t humidity[2];		
    uint8_t sensortype4;		//���������� 0x84 ����
		uint8_t light[2];  			
		uint8_t nodpacketnum[2];//�ڵ��ѭ�������
} GatewaydataV2NoCCS;
typedef struct {						//���������ݰ�  0x15
	
		GSMsgHeaderV2 GSM;
		uint8_t gwvoltage[2];		//���ص�ѹ
		GatewaydataV2 GWD;
		uint8_t packetnum[2];		//��ѭ�������
		uint8_t crc[2];
		uint8_t end;						//0x7e
} GatewayMsgV2;
typedef struct {						//���������ݰ�  0x15
	
		GSMsgHeaderV2 GSM;
		uint8_t gwvoltage[2];		//���ص�ѹ
		GatewaydataV2NoCCS GWD;
		uint8_t packetnum[2];		//��ѭ�������
		uint8_t crc[2];
		uint8_t end;						//0x7e
} GatewayMsgV2NoCCS;

 typedef struct{							//��������Ӧ����ע���  0x16
	GSMsgHeaderV2 GSM;
    uint8_t timestamp[4];		//ʱ���				
    uint8_t commandtype1;		//�������� 0xC1 �������  (110��ʼ ��5λ��ʾָ��)
    uint8_t samplingper[2];
    uint8_t commandtype2;	  //�������� 0xC2 �ڵ�ͨ������
    uint8_t transmitper[2];
		uint8_t commmandtype3;	//�������� 0xC3 ����ͨ������
		uint8_t gawtransper[2];	
    uint8_t reserve[2];			//�����ֽ�
	  uint8_t packetnum[2];		//ͬע������
		uint8_t crc[2];
		uint8_t end;						//0x7e
}ServeAckEchoV2;
 typedef struct{							//��������Ӧ����ע���  0x17
		GSMsgHeaderV2 GSM;		
    uint8_t commandtype1;		//�������� 0xC1 �������  (110��ʼ ��5λ��ʾָ��)
    uint8_t samplingper[2];
    uint8_t commandtype2;	  //�������� 0xC2 �ڵ�ͨ������
    uint8_t transmitper[2];
		uint8_t commmandtype3;	//�������� 0xC3 ����ͨ������
		uint8_t gawtransper[2];	
    uint8_t reserve[2];			//�����ֽ�
	  uint8_t packetnum[2];		//ͬע������
		uint8_t crc[2];
		uint8_t end;						//0x7e
}ServeAckMsgV2;
typedef union {
	unsigned long  timetick32;
	uint8_t  timetick[4];
} uint8_to_uint32;


enum M25P80_STATE
{
	M25P80_S_OFF=0,// �ϵ�Ĭ��ģʽ M25P80���ܶ�д
	M25P80_S_ON=1,

};
enum M25P80_RUNSTATE
{
	M25P80_BUSY=1,
	M25P80_FREE=0,	
};
enum RF_INIT_STATE
{
	RF_INIT_OFF=0,// �ϵ�Ĭ��ģʽ M25P80���ܶ�д
	RF_INIT_ON=1,
};
enum PERIPHER_STATE
{
	PERIPHER_S_OFF=0,// �ϵ�Ĭ��ģʽ M25P80���ܶ�д
	PERIPHER_S_ON=1,
};
enum RF_STATE
{
	RF_S_FSEND=1,//һ��ģʽ 
	RF_S_LISTEN=2,//ʡ��ģʽ ����
	RF_S_SSEND=3,//����ģʽ
	RF_S_IDLE=0,//����ģʽ  �ϵ�Ĭ��ģʽ
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
	NORMAL_NODE=0x01,//�����ڵ�
	WEATHER_STATION=0x02,//����վ
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

u8 out_flash(void);//���ݴ�FLASH����  ���͵�ַ�ı�һ��
u8 in_flash(void);//����д��FLASH  д���ַ�ı�һ��
u8 Write_Data_to_Flash(void);//���ɼ��Ĵ���������д��FLASH
u8 Read_Data_from_Flash(void);//��FLASH����Ҫ���͵�����
u8 Read_Data_from_RAM(void);
void RF_Send_flash_Data(void);
u8 check_Flash_data(void);//���FLASH�Ƿ�������Ҫ����
u8 check_flashdata_first(void);//��һ�μ��
//u8 check_WIFI_pack_single(char *LowRF_input_data,uint8_t len);/////����WIFI �յ��ĵ�����


u8 Read_Data_from_eeprom_last(void);//��EEPROM�����ϴε�CO2
u8 Write_Data_from_eeprom_last(void);//��EEPROMд���ϴε�CO2
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
