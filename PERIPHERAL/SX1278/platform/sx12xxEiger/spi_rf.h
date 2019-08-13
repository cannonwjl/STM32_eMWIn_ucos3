/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       spi.h
 * \brief      SPI hardware driver
 *
 * \version    1.0
 * \date       Feb 12 2010
 * \author     Miguel Luis
 */
#ifndef __SPI_RF_H__
#define __SPI_RF_H__




//#include "platform.h"
#include "sys.h"




typedef enum 
{
  OK       = 0x00U,
  ERROR0    = 0x01U,
  BUSY     = 0x02U,
  TIMEOUT  = 0x03U
} StatusTypeDef;
void SpiInit( void );
void SPI_Init1(void);
void SPI_DeInit(void);
//uint8_t * SpiInOut( uint8_t outData ,uint8_t size);

uint8_t SpiInOut( uint8_t outData );
void SPI1_FLASH_SendByte(uint8_t byte);
u8 SPI_FLASH_SendByte_read(uint8_t byte);
u8 SPI1_ReadWriteByte(u8 TxData);
void SpiInit( void );

#endif //__SPI_H__
