#ifndef __VEML6070_H_
#define __VEML6070_H_
#include "Platform_Configure.h"
//#include "stm32l0xx_hal.h"
//#include "main.h"
//	#define VEML6070_WR_ADDR       	0x70 //VEML6070, write command address
//	#define VEML6070_RL_ADDR        0x71 //VEML6070, Read LSB address
//	#define VEML6070_RM_ADDR        0x73 //VEML6070, Read MSB address
//	
	#define VEML6070_ADDR_ARA 			0x18
	#define VEML6070_ADDR_CMD 			0x70
	#define VEML6070_ADDR_DATA_LSB 	0x71
	#define VEML6070_ADDR_DATA_MSB 	0x73
	
//#define VEML6070_ADDR_ARA (0x18 >> 1)
//#define VEML6070_ADDR_CMD (0x70 >> 1)
//#define VEML6070_ADDR_DATA_LSB (0x71 >> 1)
//#define VEML6070_ADDR_DATA_MSB (0x73 >> 1)
// VEML6070 command register bits
#define VEML6070_CMD_SD 0x01
#define VEML6070_CMD_WDM 0x02
#define VEML6070_CMD_IT_0_5T 0x00
#define VEML6070_CMD_IT_1T 0x04
#define VEML6070_CMD_IT_2T 0x08
#define VEML6070_CMD_IT_4T 0x0C
#define VEML6070_CMD_DEFAULT (VEML6070_CMD_WDM |VEML6070_CMD_IT_1T)
typedef enum {
				LOW, 
				MODERATE, 
				HIGH, 
				VERY_HIGH, 
				EXTREME} RISK_LEVEL;


//RISK_LEVEL risk_level;
struct i2c_msg {
u16 addr;
u16 flags;
#define I2C_M_TEN 0x0010
#define I2C_M_RD 0x0001
#define I2C_M_NOSTART 0x4000
#define I2C_M_REV_DIR_ADDR 0x2000
#define I2C_M_IGNORE_NAK 0x1000
#define I2C_M_NO_RD_ACK 0x0800
#define I2C_M_RECV_LEN 0x0400
u16 len;
u8 *buf;
};

enum VEML6070_STATE{
	VEML6070_DISABLE=0,
	VEML6070_ENABLE=1,
};
	
	void initialize_VEML6070(void);
	
	void veml6070_on(void);
	void veml6070_off(void);
	u16 read_uvs_step(void);
	RISK_LEVEL convert_to_risk_level(u16 uvs_step);
uint16_t convert_to_risk_level_uw(u16 uvs_step);
#endif
