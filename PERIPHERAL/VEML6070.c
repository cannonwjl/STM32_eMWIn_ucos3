#include "VEML6070.h"
#include "Platform_Configure.h"
#include "stm32f10x.h"
#include "sht3x.h"
//#define _MATH_DEFINES_DEFINED
//#include "math.h"

extern uint16_t ultraviolet;//紫外
u8 VEML6070_write_byte(u8 adr, u8 data)
{
	u8 temp=0;
	I2c_StartCondition();
	temp=I2c_WriteByte(adr);
	temp+=I2c_WriteByte(data);
	I2c_StopCondition();
	return temp;
}
u8 VEML6070_read_byte(u8 adr, u8* p)
{
	u8 temp=0;
	I2c_StartCondition();
	temp+=I2c_WriteByte(adr);
	I2c_ReadByte(p,ACK,100);
	I2c_StopCondition();
	return temp;
}

enum VEML6070_STATE  VEML6070_state;
u8 cmd = VEML6070_CMD_DEFAULT;
void initialize_VEML6070(void)
{
	// Read ARA to clear interrupt
	u8 address;
	VEML6070_read_byte(VEML6070_ADDR_ARA, &address);
	// Initialize command register
	VEML6070_write_byte(VEML6070_ADDR_CMD, cmd);
//	printf(" initialize_VEML6070 ");
	delay_ms(50);
}
//----------------------------------------------------------------------------
// C main function
//----------------------------------------------------------------------------
//void main(void)
//{
//	initialize_VEML6070();
//	// Loop for polling VEML6070 data
//	while (1)
//	{
//		uvs_step = read_uvs_step();
//		risk_level = convert_to_risk_level(uvs_step);
//		delay(1000);
//	}
//}

void veml6070_on(void)
{
//	cmd &= ~VEML6070_CMD_SD;//不要了
//	VEML6070_write_byte(VEML6070_ADDR_CMD, cmd);
//	VEML6070_state=VEML6070_ENABLE;
}
void veml6070_off(void)
{
	cmd |= VEML6070_CMD_SD;
	VEML6070_write_byte(VEML6070_ADDR_CMD, cmd);
	VEML6070_state=VEML6070_DISABLE;
}

u16 read_uvs_step(void)
{
	u8 lsb, msb;
	u16 data;
	float ultraviolet_f;
	VEML6070_read_byte(VEML6070_ADDR_DATA_MSB, &msb);
	VEML6070_read_byte(VEML6070_ADDR_DATA_LSB, &lsb);
	data = ((u16)msb << 8) | (u16)lsb;
	
	if(data<6)
		ultraviolet_f=data*2.5;
	else
		ultraviolet_f=data*5.7096-15.634;
			
//	data=round(ultraviolet_f);	
	data=(u16)ultraviolet_f;
	return data;
}
RISK_LEVEL convert_to_risk_level(u16 uvs_step)
{
	u16 risk_level_mapping_table[4] = {2241, 4482, 5976, 8217};

	u16 i;
	for (i = 0; i < 4; i++)
	{
		if (uvs_step <= risk_level_mapping_table[i])
		{
			break;
		}
	}
	return (RISK_LEVEL)i;
}

uint16_t convert_to_risk_level_uw(u16 uvs_step)
{
	uint16_t uvi;
	if(uvs_step>=4109)
	{
		uvi=28;
	}
	else{
		uvi=((float)uvs_step/4109.0)*28;
	}
	ultraviolet=uvi;
	printf("\r\n uvi=%d",uvi);
	return uvi;
	
}
//int VEML6070_read_byte(u8 addr, u8 *data)
//{
//	int err = 0;
//	int retry = 3;
//	struct i2c_msg msg1;
//	// Read byte data
//	msg1.addr = addr;
//	msg1.flags = I2C_M_RD;
//	msg1.len = 1;
//	msg1.buf = data;
//	while (retry--)
//	{
////		err = i2c_transfer(msg1, 1);
//		if (err >= 0)
//		return err;
//	}
//	return err;
//}

//int VEML6070_write_byte(u16 addr, u8 data)
//{
//	int err = 0;
//	int retry = 3;
//	struct i2c_msg msg;
//	while (retry--)
//	{
//		// Send slave address & command
//		msg.addr = addr;
//		msg.flags = I2C_M_WR;
//		msg.len = 1;
//		msg.buf = &data;
//		err = i2c_transfer(msg, 1);
//		if (err >= 0)
//		return 0;
//	}
//	return err;
//}
