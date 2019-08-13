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
 * \file       spi.c
 * \brief      SPI hardware driver
 *
 * \version    1.0
 * \date       Feb 12 2010
 * \author     Miguel Luis
 */
 
//#include "stm32f10x_spi.h"
#include "spi_rf.h"
#include "system_platform.h"
#include "sys.h"
#include "stdio.h"
SPI_InitTypeDef SpiHandle;


/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_InitTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  SPIx_SCK_GPIO_CLK_ENABLE();
//  SPIx_MISO_GPIO_CLK_ENABLE();
//  SPIx_MOSI_GPIO_CLK_ENABLE();
  /* Enable SPI clock */
  SPIx_CLK_ENABLE(); 
   

  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* SPI SCK GPIO pin configuration  */
  GPIO_InitStruct.GPIO_Pin       = SPIx_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode      = GPIO_Mode_AF_PP;
//  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.GPIO_Speed     = GPIO_Speed_50MHz  ;
//  GPIO_InitStruct.Alternate = SPIx_SCK_AF;
  
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

 
  /* SPI MISO GPIO pin configuration  */
  GPIO_InitStruct.GPIO_Pin = SPIx_MISO_PIN;
	GPIO_InitStruct.GPIO_Mode      = GPIO_Mode_AF_PP;
//  GPIO_InitStruct.Alternate = SPIx_MISO_AF;
  GPIO_InitStruct.GPIO_Speed     = GPIO_Speed_50MHz  ;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
  
  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStruct.GPIO_Pin = SPIx_MOSI_PIN;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed     = GPIO_Speed_50MHz  ;
	
//  GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);    
  GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);  //PB13/14/15上拉
	
	
  
	
}

/**
  * @brief SPI MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_InitTypeDef *hspi)
{
  /*##-1- Reset peripherals ##################################################*/
//  SPIx_FORCE_RESET();
//  SPIx_RELEASE_RESET();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, DISABLE);	 //使能PB,PE端口时钟
  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure SPI SCK as alternate function  */
  GPIO_DeInit(SPIx_SCK_GPIO_PORT);
}



void SpiInit( void )
{
  SPI_InitTypeDef  SPI_InitStructure;
	SPI_Init1();
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1 , &SPI_InitStructure);
  SPI_Cmd(SPI1,ENABLE);
  SPI1_ReadWriteByte(0xff);
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	printf("SPI1_init--------------------SpiInit()-----spi_rf.c---\n");
}

void SPI_DeInit(void)
{
	HAL_SPI_MspDeInit(&SpiHandle);
}
void SPI_Init1(void)
{
//	SpiInit();
	HAL_SPI_MspInit(&SpiHandle);
}
u8 SpiRxBuffer[50];


uint8_t SpiInOut( uint8_t outData )
{

//	u8 retry=0;	
//	printf("\r\n 110\r\n");
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位
	/* Send SPIy data */
	SPI_I2S_SendData( SPI1, outData );
//	printf("\n oudata =%d\r\n",outData);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
	
	
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
//    while( SPI_I2S_GetFlagStatus( SPI1, SPI_I2S_FLAG_RXNE ) != SET );
//		printf("\n Status \r\n");
//    return SPI_I2S_ReceiveData( SPI1 );
}

void SPI1_FLASH_SendByte(u8 byte)
{
	SPI_I2S_SendData(SPI1,byte);
}


u8 SPI_FLASH_SendByte_read(uint8_t byte)
{
	u8 SpiRxBuffer1;
	
	SpiRxBuffer1=SPI_I2S_ReceiveData(SPI1);
		return SpiRxBuffer1;
}
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	
	printf("\r\n start \r\n");
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
}

