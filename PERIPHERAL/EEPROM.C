#include "eeprom.h"
#include "string.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#define STM_SECTOR_SIZE 1024 //字节
#define FLASH_SAVE_ADDR  0X08070000		//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)

extern u32 Address_ID; //Note_wx  
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void EEPROM_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)   	
{
	u16 i;
    uint32_t Read_address=ReadAddr+FLASH_SAVE_ADDR; 
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(Read_address);//读取2个字节.
		Read_address+=2;//偏移2个字节.	
	}
}

 //从指定地址开始写入多个数据
void EEPROM_Write(uint32_t startAddress,uint8_t *writeData,uint16_t countToWrite)
{ 
   uint32_t Write_address=startAddress+FLASH_SAVE_ADDR; 
   uint32_t sectorPosition=Write_address/STM_SECTOR_SIZE;            //计算扇区地址，对于STM32F103VET6为0~255
   uint32_t sectorStartAddress=sectorPosition*STM_SECTOR_SIZE;    //对应扇区的首地址
   uint16_t dataIndex;
   if(Write_address<FLASH_BASE||((Write_address+countToWrite*2)>=(FLASH_BASE + 1024*STM_SECTOR_SIZE)))
   {
     return;//非法地址
   }
   FLASH_Unlock();         //解锁写保护
   FLASH_ErasePage(sectorStartAddress);//擦除这个扇区
//   EEPROM_ErasePages(0,20);
   for(dataIndex=0;dataIndex<countToWrite;dataIndex++)
   {
     FLASH_ProgramHalfWord(Write_address+dataIndex*2,writeData[dataIndex]);
   }
   
   FLASH_Lock();//上锁写保护
 }

u8 EEPROM_Write_Repeate(uint32_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)//这个函数相当于一个写入读出的一个校验  看写入的数据和读出的数据是不是一样的
{
	u8 completeflag=0;
	uint8_t tetsbuf[500]={0};
	uint8_t tetsbuf1[500]={0};
	uint8_t tetsbuf2[500]={0};
	u8 write_num;
	memcpy(tetsbuf2,pBuffer,NumToWrite);
	write_num=5;
	while(write_num)
	{
		EEPROM_Write(WriteAddr,(u8*)tetsbuf2,NumToWrite);
		delay_ms(5);
		EEPROM_Read(WriteAddr,(u8*)tetsbuf,NumToWrite);
		memcpy(tetsbuf1,tetsbuf,NumToWrite);
		if(memcmp(tetsbuf1,tetsbuf2,NumToWrite)==0)
		{
			completeflag=1;
			break;
		}
		else
		{
			write_num--;
			printf("\r\n write error!\r\n");
			delay_ms(30);
		}

	}	
	return completeflag;
}

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
void EEPROM_ErasePages(uint32_t startPage,uint32_t endPage)//有问题
{
   uint16_t t;
   uint32_t sectorPosition;
   FLASH_Unlock();         //解锁写保护
   sectorPosition=(endPage -startPage)/STM_SECTOR_SIZE;            //计算扇区地址，对于STM32F103VET6为0~255
   FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	for(t = 0 ;(t<sectorPosition)&&(FLASHStatus==FLASH_COMPLETE);t++)
   {
		FLASHStatus=FLASH_ErasePage(startPage+(STM_SECTOR_SIZE*t));
   }
   switch(FLASHStatus)
   {
	   case FLASH_FLAG_EOP: printf("\r\n a\r\n"); break;
	   case FLASH_FLAG_PGERR: printf("\r\n b\r\n"); break;
	   case FLASH_FLAG_WRPRTERR: printf("\r\n c\r\n"); break;
//	   case FLASH_COMPLETE: printf("\r\n d\r\n"); break;
	   case FLASH_FLAG_BSY: printf("\r\n e\r\n"); break;
	   default: break;
   }
	FLASH_Lock();//上锁写保护
}

void Get_CustomChipID(void)
{
	Address_ID= *( u16*)(0x1FFFF7E0);    
	printf(" ======================================================= \t\n");
	printf(" ---------Address_ID=%d -------------EEPROM.c \t\n",Address_ID);
}

void Get_ChipID(void)
{
	u32 ChipUniqueID[3];
	ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8);  // 低字节
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 高字节
	printf("\r\n芯片的唯一ID为: %X  %X   %X  \r\n",ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
}

