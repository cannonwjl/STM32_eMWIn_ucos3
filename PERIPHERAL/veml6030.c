#include "VEML6030.h"
#include "Platform_Configure.h"
#include "i2c_hal.h"
#include "i2c_hal.h"

//#include "I2C.h"


WORD cmd_6030[4] = {VEML6030_ALS_CONF_DEFAULT | VEML6030_ALS_CONF_PERS_1 | VEML6030_ALS_CONF_IT_100MS | VEML6030_ALS_CONF_SM_x2, 315, 285, 0};
WORD cmd1_6030[4] = {VEML6030_ALS_CONF_DEFAULT | VEML6030_ALS_CONF_PERS_1 | VEML6030_ALS_CONF_IT_100MS | VEML6030_ALS_CONF_SM_x1, 315, 285, 0};
WORD cmd2_6030[4] = {VEML6030_ALS_CONF_DEFAULT | VEML6030_ALS_CONF_PERS_1 | VEML6030_ALS_CONF_IT_100MS | VEML6030_ALS_CONF_SM_x1_4, 315, 285, 0};
WORD cmd3_6030[4] = {VEML6030_ALS_CONF_DEFAULT | VEML6030_ALS_CONF_PERS_1 | VEML6030_ALS_CONF_IT_100MS | VEML6030_ALS_CONF_SM_x1_8, 315, 285, 0};

WORD als_code = 300;
const float change_sensitivity = 5; // in percent
const float calibration_factor = 0.0288;
float lux_6030;


void clear_interrupt(void)
{
	WORD value;
	// Read ALS_STATUS register to clear interrupt
	VEML6030_read_word(VEML6030_ADDR_ALS, ALS_STATUS, &value);
}

void enable_interrupt(void)
{
	cmd_6030[ALS_CONF] |= VEML6030_ALS_CONF_INT_EN;
	VEML6030_write_word(VEML6030_ADDR_ALS, ALS_CONF, cmd_6030[ALS_CONF]);
}
void disable_interrupt(void)
{
	cmd_6030[ALS_CONF] &= ~VEML6030_ALS_CONF_INT_EN;
	VEML6030_write_word(VEML6030_ADDR_ALS, ALS_CONF, cmd_6030[ALS_CONF]);
}

void enable_sensor_veml6030(void)
{
	cmd_6030[ALS_CONF] &= ~VEML6030_ALS_CONF_SD;
	
//	cmd_6030[0]=0x0032;
	
//	printf("\n start");
	VEML6030_write_word(VEML6030_ADDR_ALS, ALS_CONF, cmd_6030[ALS_CONF]);
	disable_interrupt();
	
}
void disable_sensor_veml6030(void)
{
	cmd_6030[ALS_CONF] |= VEML6030_ALS_CONF_SD;
	VEML6030_write_word(VEML6030_ADDR_ALS, ALS_CONF, cmd_6030[ALS_CONF]);
}

WORD read_als_data(void)
{
	WORD value;
	VEML6030_read_word(VEML6030_ADDR_ALS, ALS_DATA, &value);
	return value;
}




int VEML6030_read_word(WORD addr, BYTE command, WORD *val)
{
	int err = 0;
//	int retry = 3;
//	struct i2c_msg_6030 msg[2];
	u8 data[5];
	
	err=Single_ReadI2C(VEML6030_ADDR_ALS,command,data,2);
//	val=(WORD *)((data[0]<<8)+data[1]);
	*val = ((WORD)data[1] << 8) | (WORD)data[0];
	
	return err;
}


int VEML6030_write_word(WORD addr, BYTE command, WORD val)
{
	int err = 0;
//	int retry = 3;
//	struct i2c_msg_6030 msg;
	BYTE data[3];
	
	data[0] = (BYTE)(val & 0xFF);
	data[1] = (BYTE)((val & 0xFF00) >> 8);
	
	err = Single_MWriteI2C_byte(VEML6030_ADDR_ALS,command,data,2);
	
	return err;
}


u16 veml6030_read(void)
{
//	clear_interrupt();
	u16 Lux_ok;
	
	als_code = read_als_data();
	lux_6030 = als_code * 0.0288;
	
	if(lux_6030>1880)//超量程  换为 *1
	{
		printf("\n switch *1");
		cmd1_6030[ALS_CONF] &= ~VEML6030_ALS_CONF_SD;
		VEML6030_write_word(VEML6030_ADDR_ALS, ALS_CONF, cmd1_6030[ALS_CONF]);
		delay_ms(120);
		als_code = read_als_data();
		lux_6030 = als_code * 0.0576;
		
		if(lux_6030>3760)//超量程  换为 *1/4
		{
			printf("\n switch *1/4");
			cmd2_6030[ALS_CONF] &= ~VEML6030_ALS_CONF_SD;
			VEML6030_write_word(VEML6030_ADDR_ALS, ALS_CONF, cmd2_6030[ALS_CONF]);
			delay_ms(120);
			als_code = read_als_data();
			lux_6030 = als_code * 0.2304;
			if(lux_6030>15000)//超量程  换为 1/8
			{
				printf("\n switch *1/8");
				cmd3_6030[ALS_CONF] &= ~VEML6030_ALS_CONF_SD;
				VEML6030_write_word(VEML6030_ADDR_ALS, ALS_CONF, cmd3_6030[ALS_CONF]);
				delay_ms(120);
				als_code = read_als_data();
				lux_6030 = als_code * 0.4608;
			}
		}
	
	}
//	Lux_ok=lux_6030*lux_6030*0.0036+lux_6030*2.0375+16.1090;
	
	if(lux_6030<30)
		Lux_ok=lux_6030*2.83;
	else
		Lux_ok=lux_6030*lux_6030*0.0029+lux_6030*2.2112+18.5227;
	
//	printf("\n als_code=%d,lux_6030=%d",als_code,Lux_ok);
	return Lux_ok;
}

