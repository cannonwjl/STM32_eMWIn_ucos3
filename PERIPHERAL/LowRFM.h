#ifndef LOWRFM_H
#define LOWRFM_H

#include "ggsensor.h"
extern u32 GatewayTime;//网关得到的时间戳
enum {
	REPLY_CONFIG_WIFI=0x01,
	REPLY_CONFIG_UART=0x02,
};
void setLowRFGroupID(uint8_t gid);
uint8_t getRecvgroupId(void);
uint8_t getLowRFGroupID(void);
u8 LowRF_input(char *LowRF_input_data,uint8_t len);
u8 check_WIFI_pack_single(char *LowRF_input_data,uint8_t len);
uint16_t crc_cal(uint8_t* buf, uint16_t len);
uint16_t hdlc_encode(uint8_t* buf, uint8_t* hdlcbuf, uint16_t len);
uint16_t hdlc_decode(uint8_t * hdlcbuf, uint8_t* buf, uint16_t len);//加入转义的转换，将第一个参数hdlcbuf变成buf
u8 USART_input_param(char *LowRF_input_data,uint8_t len);
void Reply_configure(u8 WifiOrUart,uint8_t *pData,uint16_t size);

#endif
