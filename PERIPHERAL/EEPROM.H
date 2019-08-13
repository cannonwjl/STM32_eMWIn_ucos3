#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "stm32f10x.h"
#include "stdint.h"
#include "include.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 512 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值
 
u16 STMFLASH_ReadHalfWord(u32 faddr);

void EEPROM_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead) ;  	
u8 EEPROM_Write_Repeate(uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);//这个函数相当于一个写入读出的一个校验  看写入的数据和读出的数据是不是一样的
void EEPROM_Write(uint32_t startAddress,uint8_t *writeData,uint16_t countToWrite);
void EEPROM_ErasePages(uint32_t startPage,uint32_t endPage);//OK
void Get_ChipID(void);
void Get_CustomChipID(void);


#endif
