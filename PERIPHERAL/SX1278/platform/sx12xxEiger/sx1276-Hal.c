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
 * \file       sx1276-Hal.c
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */
//#include <stdint.h>
//#include <stdbool.h> 

//#include "platform.h"
#include "spi_rf.h"
#include "system_platform.h"
#include "sys.h"
#include "stdio.h"
#include "sx1276-Hal.h"
#include "timer.h"
//#if defined( USE_SX1276_RADIO )

//#include "ioe.h"


/*!
 * SX1276 RESET I/O definitions
 */
//#define RESET_IOPORT                                GPIOC
//#define RESET_PIN                                   GPIO_PIN_13

/*!
 * SX1276 SPI NSS I/O definitions
 */
//#define NSS_IOPORT                                  GPIOA
//#define NSS_PIN                                     GPIO_PIN_4     //ԭGPIO_Pin_15

/*!
 * SX1276 DIO pins  I/O definitions
 */
//#define DIO0_IOPORT                                 GPIOB
//#define DIO0_PIN                                    GPIO_PIN_9

//#define DIO1_IOPORT                                 GPIOB
//#define DIO1_PIN                                    GPIO_PIN_8

//#define DIO2_IOPORT                                 GPIOB
//#define DIO2_PIN                                    GPIO_PIN_5

//#define DIO3_IOPORT                                 GPIOB
//#define DIO3_PIN                                    GPIO_PIN_4

//#define DIO4_IOPORT                                 GPIOB
//#define DIO4_PIN                                    GPIO_PIN_3

//#define DIO5_IOPORT                                 GPIOA
//#define DIO5_PIN                                    GPIO_PIN_15

//#define RXTX_IOPORT                                 
//#define RXTX_PIN                                    FEM_CTX_PIN


void Set_RF_Switch_RX(void)
{

}

void Set_RF_Switch_TX(void)
{
//	RXE_LOW();
//	TXE_HIGH();
}


void SX1276InitIo( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
//	  __HAL_RCC_GPIOC_CLK_ENABLE();
//		__HAL_RCC_GPIOB_CLK_ENABLE();
//	__HAL_RCC_GPIOA_CLK_ENABLE();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口时钟
		GPIO_InitStructure.GPIO_Pin = NSS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     // Configure SPI-->NSS as output
    
    GPIO_Init( NSS_IOPORT, &GPIO_InitStructure );
	  GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );

	
    // Configure radio DIO as inputs
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    // Configure DIO0
    GPIO_InitStructure.GPIO_Pin =  DIO0_PIN;
    GPIO_Init( DIO0_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO1
    GPIO_InitStructure.GPIO_Pin =  DIO1_PIN;
    GPIO_Init( DIO1_IOPORT, &GPIO_InitStructure );
    
    // Configure DIO2
    GPIO_InitStructure.GPIO_Pin =  DIO2_PIN;
    GPIO_Init( DIO2_IOPORT, &GPIO_InitStructure );
    
    // REAMARK: DIO3/4/5 configured are connected to IO expander

    // Configure DIO3 as input
    GPIO_InitStructure.GPIO_Pin =  DIO3_PIN;
    GPIO_Init( DIO3_IOPORT, &GPIO_InitStructure );
    // Configure DIO4 as input
    GPIO_InitStructure.GPIO_Pin =  DIO4_PIN;
    GPIO_Init( DIO4_IOPORT, &GPIO_InitStructure );
    // Configure DIO5 as input
	GPIO_InitStructure.GPIO_Pin =  DIO5_PIN;
    GPIO_Init( DIO5_IOPORT, &GPIO_InitStructure );

}

void SX1276SetReset( u8 state )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if( state == RADIO_RESET_ON )
    {
        // Configure RESET as output
			GPIO_InitStructure.GPIO_Pin = RESET_PIN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
			GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );

			// Set RESET pin to 0
			GPIO_WriteBit( RESET_IOPORT, RESET_PIN, Bit_RESET );
    }
    else
    {
			GPIO_InitStructure.GPIO_Pin =  RESET_PIN;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init( RESET_IOPORT, &GPIO_InitStructure );

			// Set RESET pin to 1
			GPIO_WriteBit( RESET_IOPORT, RESET_PIN, Bit_SET );

    }
}

void SX1276Write( u8 addr, u8 data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( u8 addr, u8 *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_RESET );

    SpiInOut( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        SpiInOut( buffer[i] );
    }

    //NSS = 1;
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
}


//void SX1276WriteBuffer( u8 addr, u8 *buffer, u8 size )
//{
//    u8 i;
////    u8 *data;
//    //NSS = 0;
//    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_RESET );

//    SpiInOut( addr | 0x80 ,0);
//    for( i = 0; i < size; i++ )
//    {
//        SpiInOut( buffer[i],0);
////			 buffer[i] =*(data + i);
//    }

//    //NSS = 1;
//    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
//}

//void SX1276ReadBuffer( u8 addr, u8 *buffer, u8 size )
//{
//    u8 i;
//	u8 *data;
//printf("N\r\n");
//    //NSS = 0;
//    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_RESET );
//printf("M\r\n");
//    data=SpiInOut( addr & 0x7F  ,size);
//		printf("\r\nSpiInOut = %d\r\n",data);
//printf("L\r\n");
//    for( i = 0; i < size; i++ )
//    {
//        buffer[i] =*(data + i);
//    }
////		printf("\r\n buffer = %d\r\n",buffer);
//    //NSS = 1;
//    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
//}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_RESET );
//		printf("\n GPIO_WriteBit \r\n");
		SpiInOut( addr & 0x7F );
//		printf("\n Spinout \r\n");
    for( i = 0; i < size; i++ )
    {
        buffer[i] = SpiInOut( 0 );
//				printf("buffer0= %d\r\n",buffer[i]);
    }
//		printf("\r\n buffer =0x%02X\n",buffer);
    //NSS = 1;
    GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );
}

void SX1276WriteFifo( u8 *buffer, u8 size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( u8 *buffer, u8 size )
{
    SX1276ReadBuffer( 0, buffer, size );
}

uint8_t SX1276ReadDio0( void )
{
    return GPIO_ReadInputDataBit( DIO0_IOPORT, DIO0_PIN );
}

 uint8_t SX1276ReadDio1( void )
{
    return GPIO_ReadInputDataBit( DIO1_IOPORT, DIO1_PIN );
}

 uint8_t SX1276ReadDio2( void )
{
    return GPIO_ReadInputDataBit( DIO2_IOPORT, DIO2_PIN );
}

 uint8_t SX1276ReadDio3( void )
{
    return GPIO_ReadInputDataBit( DIO3_IOPORT, DIO3_PIN );
}

 uint8_t SX1276ReadDio4( void )
{
    return GPIO_ReadInputDataBit( DIO4_IOPORT, DIO4_PIN );
}

 uint8_t SX1276ReadDio5( void )
{
    return GPIO_ReadInputDataBit( DIO5_IOPORT, DIO5_PIN );
}



//射频芯片收发切换
void SX1276WriteRxTx( u8 txEnable )
{
//	printf("SX1276WriteRxTx! \n");
//    if( txEnable != 0 )
//    {
//		Set_RF_Switch_TX(); //单片机将射频开关芯片切换成发射状态
////        IoePinOn( FEM_CTX_PIN );
////        IoePinOff( FEM_CPS_PIN );
//    }
//    else
//    {
//		Set_RF_Switch_RX();  //单片机将射频开关芯片切换成接收状态
////        IoePinOff( FEM_CTX_PIN );
////        IoePinOn( FEM_CPS_PIN );
//    }
}
uint32_t uwTick;
//void HAL_IncTick(void)
//{
//  uwTick++;
//	printf("uwTick=%d\r\n",uwTick);
//}
//uint32_t HAL_GetTick(void)
//{
//  return uwTick;
//}


int GET_TICK_COUNT( void)
{
	return uwTick;
}	
//#endif // USE_SX1276_RADIO