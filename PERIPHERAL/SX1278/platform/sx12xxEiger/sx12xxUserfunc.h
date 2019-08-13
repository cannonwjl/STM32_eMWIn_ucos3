/*! 
 * \file       sx12xxUserfunc.h
 * \brief      user func  driver
 *
 * \version    1.0
 * \date       Feb 12 2010
 * \author     
 */
#ifndef __SX12XXUSERFUNC_H__
#define __SX12XXUSERFUNC_H__

#include "sys.h"

#include "spi_rf.h"

//#include "platform.h"

enum RF_RE_SE_STATE
{
	RF_SUCCESS=0,
	RF_FAILURE=1,
	RF_TIMEOUT=2,
};


void Reset_sx1276(void);

void RF_Init(void);
void RF_Receive( void *buffer, uint16_t *size );
u8 RF_Receive_general( void *buffer, uint16_t *size );
u8 RF_Send_General(uint8_t *buffer, uint16_t size);//һ��ģʽ����
u8 RF_Send_Rouse(uint8_t *buffer, uint16_t size);//����ģʽ ����  ���ڻ��ѵ͹��Ľ��յ�ģ��
void RF_Receive_LowPower( void *buffer, uint16_t *size );
void RF_StartRX(void);
void RF_StartTX(void);
void SX1278Sleep(void);
double Read_RSSI(void);
u8 RF_Send_LowPower(uint8_t *buffer, uint16_t size);
#endif //__SPI_H__
