#ifndef __VEML6030_H_
#define __VEML6030_H_

#include "stm32f10x.h"
#define WORD u16 
#define BYTE u8 
	// VEML6030 slave address can be 0x20 or 0x90, determined by pin ADDR configuration
#define VEML6030_ADDR_ALS 0x20 // 7-bit: 0x10
//#define VEML6030_ADDR_ALS 0x90 // 7-bit: 0x48
// VEML6030 registers
#define ALS_CONF 0x00
#define ALS_THDH 0x01
#define ALS_THDL 0x02
#define ALS_PSM 0x03
#define ALS_DATA 0x04
#define ALS_STATUS 0x06
// VEML6030 command code 00 register bits
#define VEML6030_ALS_CONF_SD 0x0001   //shutdown
#define VEML6030_ALS_CONF_INT_EN 0x0002  // INT EN
#define VEML6030_ALS_CONF_PERS_MASK 0x0030
#define VEML6030_ALS_CONF_PERS_1 0x0000
#define VEML6030_ALS_CONF_PERS_2 0x0010
#define VEML6030_ALS_CONF_PERS_4 0x0020
#define VEML6030_ALS_CONF_PERS_8 0x0030
#define VEML6030_ALS_CONF_IT_MASK 0x00C0
#define VEML6030_ALS_CONF_IT_100MS 0x0000
#define VEML6030_ALS_CONF_IT_200MS 0x0040
#define VEML6030_ALS_CONF_IT_400MS 0x0080
#define VEML6030_ALS_CONF_IT_800MS 0x00C0
#define VEML6030_ALS_CONF_SM_MASK 0x1800
#define VEML6030_ALS_CONF_SM_x1 0x0000      //  1
#define VEML6030_ALS_CONF_SM_x2 0x0800     //   2
#define VEML6030_ALS_CONF_SM_x1_8 0x1000   //  1/8
#define VEML6030_ALS_CONF_SM_x1_4 0x1800  //  11  1/4
#define VEML6030_ALS_CONF_DEFAULT 0x0000

#define I2C_M_WR 1

struct i2c_msg_6030 {
	WORD addr;
	WORD flags;
	#define I2C_M_TEN 0x0010
	#define I2C_M_RD 0x0001
	#define I2C_M_NOSTART 0x4000
	#define I2C_M_REV_DIR_ADDR 0x2000
	#define I2C_M_IGNORE_NAK 0x1000
	#define I2C_M_NO_RD_ACK 0x0800
	#define I2C_M_RECV_LEN 0x0400
	WORD len;
	BYTE *buf;
};

 extern  float lux_6030;
int VEML6030_read_word(WORD addr, BYTE command, WORD *val);
int VEML6030_write_word(WORD addr, BYTE command, WORD val);

void disable_sensor_veml6030(void);
void enable_sensor_veml6030(void);
WORD read_als_data(void);
u16 veml6030_read(void);

#endif
