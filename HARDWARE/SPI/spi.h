#ifndef __SPI_H
#define __SPI_H

#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


//uint8_t aTxBuffer[10] = {8,9,0,15,2,6,6,6,6};//测试
///* Size of buffer */
#define BUFFERSIZE                       (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
///* Exported functions ------------------------------------------------------- */

#define WREN 0x06
#define WRDI 0x04
#define RDSR 0x05
#define WRSR 0x01
#define READ 0x03
#define FAST_READ 0x0b
#define PAGE_PROG 0x02
#define SECTOR_ERASER 0xD8
#define BULK_ERASER 0xC7
#define DEEP_SLEEP 0xB9
#define RES 0xAB


#define Read_Id      0x9F





/************************** SPI Flash 连接引脚定义********************************/
#define      macSPIx                                     SPI1
#define      macSPI_APBxClock_FUN                        RCC_APB2PeriphClockCmd
#define      macSPI_CLK                                  RCC_APB2Periph_SPI1

#define      macSPI_CS_APBxClock_FUN                     RCC_APB2PeriphClockCmd
#define      macSPI_CS_CLK                               RCC_APB2Periph_GPIOD    
#define      macSPI_CS_PORT                              GPIOD
#define      macSPI_CS_PIN                               GPIO_Pin_3
//NSS
#define      macSPI_CS_APBxClock_FUN                     RCC_APB2PeriphClockCmd
#define      macSPI_CS_CLK                               RCC_APB2Periph_GPIOD  
#define			 M25P80_NSS_GPIO														 GPIOD
#define	  		M25P80_NSS_PIN														 GPIO_Pin_3


#define      macSPI_SCK_APBxClock_FUN                    RCC_APB2PeriphClockCmd
#define      macSPI_SCK_CLK                              RCC_APB2Periph_GPIOB   
#define      macSPI_SCK_PORT                             GPIOB  
#define      macSPI_SCK_PIN                              GPIO_Pin_3

#define      macSPI_MISO_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define      macSPI_MISO_CLK                             RCC_APB2Periph_GPIOB    
#define      macSPI_MISO_PORT                            GPIOB
#define      macSPI_MISO_PIN                             GPIO_Pin_4

#define      macSPI_MOSI_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define      macSPI_MOSI_CLK                             RCC_APB2Periph_GPIOB    
#define      macSPI_MOSI_PORT                            GPIOB 
#define      macSPI_MOSI_PIN                             GPIO_Pin_5



/************************** SPI Flash 函数宏定义********************************/
#define      macSPI_FLASH_CS_ENABLE()                       GPIO_ResetBits( macSPI_CS_PORT, macSPI_CS_PIN )
#define      macSPI_FLASH_CS_DISABLE()                      GPIO_SetBits( macSPI_CS_PORT, macSPI_CS_PIN )
//测试 2019-7-29
//#define      macSPI_FLASH_CS_ENABLE()                       GPIO_SetBits( macSPI_CS_PORT, macSPI_CS_PIN )
//#define      macSPI_FLASH_CS_DISABLE()                      GPIO_ResetBits( macSPI_CS_PORT, macSPI_CS_PIN )

void SPI_FLASH_Init(void);

u16 SPI_FLASH_SendByte(u8 byte);



void M25P80_INIT(void);
void M25P80_Section_Erase(u32 addr);
void M25P80_Bulk_Erase(void);
void M25P80_Write_1Byte(u32 addr , u8 data);
void M25P80_Write_Bytes(u32 addr , u8* wr_buf_p , u16 no);
u8 M25P80_Read_1Byte(u32 addr );
void M25P80_Read_Bytes(u32 addr , u8* re_buf_p , u16 no);
u32 SPI_FLASH_ReadID(void);
void getFlashtypeID(void);
void M25P80_DEEP_SLEEP(void) ; //深度休眠
void M25P80_RES(void);
void FLASH_CS(uint8_t d);

#endif
