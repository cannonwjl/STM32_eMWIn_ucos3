/*
 * Copyright (c) 2018 Beijing ZhongKeTianHe Co., Ltd.
 * All rights reserved.
 *
 * @author WX
 * @version 2018/9/30
 * 
   M25P80驱动函数
 */
 
#include "spi.h"
#include "lcd.h"
//#include "include.h" 
/* Private typedef -----------------------------------------------------------*/
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PerWritePageSize      256

//JTAG????,????JTAG???
//mode:jtag,swd????;00,???;01,??SWD;10,???;	   
//#define JTAG_SWD_DISABLE   0X02
//#define SWD_ENABLE         0X01
//#define JTAG_SWD_ENABLE    0X00		  
void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;
	RCC->APB2ENR|=1<<0;     //??????	   
	AFIO->MAPR&=0XF8FFFFFF; //??MAPR?[26:24]
	AFIO->MAPR|=temp;       //??jtag??
} 


/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);	//打开GPIO口时钟，先打开服用
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,DISABLE);// 要先开时钟，再开重映射 

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
	//RCC_PCLK2Config(RCC_HCLK_Div4);
//#define GPIO_Remap_SWJ_NoJTRST      ((uint32_t)0x00300100)  /*!< Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST */
//#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */
//#define GPIO_Remap_SWJ_Disable      ((uint32_t)0x00300400)  /*!< Full SWJ Disabled (JTAG-DP + SW-DP) */
//RCC_APB2Periph_SPI1
 // JTAG_Set(00);
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  /*!< SPI_FLASH_SPI Periph clock enable */
	macSPI_APBxClock_FUN ( RCC_APB2Periph_SPI1, ENABLE );
 
  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
	macSPI_CS_APBxClock_FUN ( RCC_APB2Periph_GPIOD, ENABLE );
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  /*!< Configure SPI_FLASH_SPI pins: SCK */
	macSPI_SCK_APBxClock_FUN ( RCC_APB2Periph_GPIOB, ENABLE );
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
	macSPI_MISO_APBxClock_FUN ( RCC_APB2Periph_GPIOB, ENABLE );
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
	macSPI_MOSI_APBxClock_FUN ( RCC_APB2Periph_GPIOB, ENABLE );
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


		GPIO_SetBits( GPIOB, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5 );
  /* Deselect the FLASH: Chip Select high */
  macSPI_FLASH_CS_DISABLE();
//	GPIO_SetBits( GPIOD, GPIO_Pin_3 );
//macSPI_FLASH_CS_ENABLE();
  /* SPI1 configuration */
  // W25X16: data input on the DIO pin is sampled on the rising edge of the CLK. 
  // Data on the DO and DIO pins are clocked out on the falling edge of CLK.
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(macSPIx , &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(macSPIx , ENABLE);
	//SPI1_ReadWriteByte(0xff);
	//SPI_FLASH_SendByte(0xff);
//	M25P80_Write_En();
//	SPI2_ReadWriteByte(0xff);
}

  
//void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
//{
//  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
//	SPI2->CR1&=0XFFC7;
//	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
//	SPI_Cmd(SPI2,ENABLE); 

//} 

void FLASH_CS(uint8_t d)
{
	if(d==1)
        GPIO_SetBits(GPIOD,GPIO_Pin_3); 						 //PD.3 输出高 
	else
        GPIO_ResetBits(GPIOD,GPIO_Pin_3);
		
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*		8Mbit = 1,048,576 bytes = 16 sectors (512 Kbits, 65536 bytes each) =
* 		4096 pages (256 bytes each)
*       1sector = 256 page                                      
* address range 00000 ~ fffffh
*******************************************************************************/
u16 SPI_FLASH_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(macSPIx, SPI_I2S_FLAG_TXE) == RESET);
  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(macSPIx, (uint16_t)byte);
  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(macSPIx, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(macSPIx);
}

/*******************************************************************************
* Function Name  : void M25P80_CMD1B(u8 cmd)
* Description    : ????????
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
void M25P80_CMD1B(u8 cmd)
{
	macSPI_FLASH_CS_ENABLE();
	SPI_FLASH_SendByte(cmd);
macSPI_FLASH_CS_DISABLE();

}
/*******************************************************************************
* Function Name  : u8 M25P80_CMD1B_READ1B(u8 cmd)
* Description    : ??????????????
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u16 M25P80_CMD1B_READ1B(u16 cmd)
{
	u16 data;
	macSPI_FLASH_CS_ENABLE();
	SPI_FLASH_SendByte(cmd);
	data = SPI_FLASH_SendByte(0XFF);
	macSPI_FLASH_CS_DISABLE();

	return data;
}
/*******************************************************************************
* Function Name  : void M25P80_CMD1B_S1B(u16 cmd , u16 data)
* Description    : ????????
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
void M25P80_CMD1B_S1B(u16 cmd , u16 data)
{

	macSPI_FLASH_CS_ENABLE();
	SPI_FLASH_SendByte(cmd);
	SPI_FLASH_SendByte(data);
	macSPI_FLASH_CS_DISABLE();

}
/*******************************************************************************
* Function Name  : void M25P80_WP_En(void)
* Description    : ???
*                  ???????WEL?
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void M25P80_Write_En(void)
{
	u16 sta;
	//?? Block Protect?:PB0,PB1,PB2,?????????
	sta = M25P80_CMD1B_READ1B(RDSR) & (~0x1c);
	M25P80_CMD1B_S1B(WRSR, sta);
	// Protected Area Upper sixteenth (Sector 15)??WEL?
	M25P80_CMD1B(WREN);
}
/*******************************************************************************
* Function Name  : void M25P80_WP_En(void)
* Description    : ???
*                  ???????WEL?
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void M25P80_WP_En(void)
{
	u16 sta;
	//??PB0,PB1,PB2????
	sta = M25P80_CMD1B_READ1B(RDSR) | 0x1c;
	M25P80_CMD1B_S1B(WRSR, sta);
	
	M25P80_CMD1B(WRDI);
}
/*******************************************************************************
* Function Name  : u8 M25P80_Busy(void)
* Description    : ????
*                  ???WIP?1?????
* Input          : None
* Output         : None
* Return         : return (sta & 0x01);
*******************************************************************************/
u8 M25P80_Busy(void)
{
	u8 sta;
	sta = M25P80_CMD1B_READ1B(RDSR);
	return (sta & 0x01);
}
/*******************************************************************************
* Function Name  : void M25P80_Section_Erase(u32 addr)
* Description    : ???
*                  ??FLASH???????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void M25P80_Section_Erase(u32 addr)
{
	u8 ad[3] ;
	ad[0] = (addr & 0x00ff0000) >> 16;
	ad[1] = (addr & 0x0000ff00) >> 8;
	ad[2] = (addr & 0x000000ff);
	M25P80_Write_En();
	macSPI_FLASH_CS_ENABLE();
	SPI_FLASH_SendByte(SECTOR_ERASER);
	SPI_FLASH_SendByte(ad[0]);
	SPI_FLASH_SendByte(ad[1]);
	SPI_FLASH_SendByte(ad[2]);
	macSPI_FLASH_CS_DISABLE();

	while(M25P80_Busy());
	M25P80_WP_En();

}
/*******************************************************************************
* Function Name  : void M25P80_Bulk_Erase(void)
* Description    : ?????
*                  ???8MFLASH
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void M25P80_Bulk_Erase(void)
{
	M25P80_Write_En();
	M25P80_CMD1B(BULK_ERASER);
	printf("\r\n  1---------spi.c擦除功能！！  \r\n");
	while(M25P80_Busy());
	M25P80_WP_En();
}

/*******************************************************************************
* Function Name  : void M25P80_Write_1Byte(u32 addr , u8 data)
* Description    : ???
*                  ??????????
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void M25P80_Write_1Byte(u32 addr , u8 data)
{
	u8 ad[3] ;
	ad[0] = (addr & 0x00ff0000) >> 16;
	ad[1] = (addr & 0x0000ff00) >> 8;
	ad[2] = (addr & 0x000000ff);
	
	
	M25P80_Write_En();
	
  macSPI_FLASH_CS_ENABLE();
	SPI_FLASH_SendByte(PAGE_PROG);
	SPI_FLASH_SendByte(ad[0]);
	SPI_FLASH_SendByte(ad[1]);
	SPI_FLASH_SendByte(ad[2]);
	SPI_FLASH_SendByte(data);
	macSPI_FLASH_CS_DISABLE();

	
	M25P80_WP_En();
	while(M25P80_Busy());
}
/*******************************************************************************
* Function Name  : void M25p80_Write_Bytes(u32 addr , u8* wr_buf_p , u16 no)
* Description    : ???
*                  ???????
* Input          : u32 addr???? , u8* wr_buf_p????????? ,
*				   u16 no??????
* Output         : None
* Return         : None
*******************************************************************************/
void M25P80_Write_Bytes(u32 addr , u8* wr_buf_p , u16 no)
{
	/*
	u8 ad[3] ;
	ad[0] = (addr & 0x00ff0000) >> 16;
	ad[1] = (addr & 0x0000ff00) >> 8;
	ad[2] = (addr & 0x000000ff);
	if( no < 255 -ad[2] ) return 0 ;//PP?????????????????
	
	M25p80_Cmd1b(WREN);
	M25P80_SELECT();
	SPI_SeRe_Byte(PAGE_PROG);
	SPI_SeRe_Byte(ad[0]);
	SPI_SeRe_Byte(ad[1]);
	SPI_SeRe_Byte(ad[2]);
	for(; no > 0; no--)
	   *wr_buf_p++ = SPI_SeRe_Byte(0xff);
	M25P80_DESELECT();
	M25p80_Cmd1b(WRDI);
	return 1;
	*/
	for(; no > 0; no--)
	{
	   M25P80_Write_1Byte(addr , *wr_buf_p);
	   addr++;wr_buf_p++;
	}
	M25P80_WP_En();

}

/*******************************************************************************
* Function Name  : u8 M25P80_Read_1Byte(u32 addr )
* Description    : ???
*                  ??????????
* Input          : None
* Output         : None
* Return         : return data;
*******************************************************************************/
u8 M25P80_Read_1Byte(u32 addr )
{
	u8 ad[3] , data;
	ad[0] = (addr & 0x00ff0000) >> 16;
	ad[1] = (addr & 0x0000ff00) >> 8;
	ad[2] = (addr & 0x000000ff);
	macSPI_FLASH_CS_ENABLE();
	SPI_FLASH_SendByte(READ);
	SPI_FLASH_SendByte(ad[0]);
	SPI_FLASH_SendByte(ad[1]);
	SPI_FLASH_SendByte(ad[2]);
	data = SPI_FLASH_SendByte(0xff);
	macSPI_FLASH_CS_DISABLE();

	return data;
}
/*******************************************************************************
* Function Name  : void M25P80_Read_Bytes(u32 addr , u8* re_buf_p , u16 no)
* Description    : ???
*                  ??????N???
* Input          : u32 addr?? , u8* re_buf_p????????
*				 , u16 no??????N
* Output         : None
* Return         : return data;
*******************************************************************************/
void M25P80_Read_Bytes(u32 addr , u8* re_buf_p , u16 no)
{
	u8 ad[3] ;
	ad[0] = (addr & 0x00ff0000) >> 16;
	ad[1] = (addr & 0x0000ff00) >> 8;
	ad[2] = (addr & 0x000000ff);
	macSPI_FLASH_CS_ENABLE();
//	M25P80_SPI_Write24(ADDR); //写地址
	SPI_FLASH_SendByte(READ);
//	SPI_FLASH_SendByte(addr); //写地址
	SPI_FLASH_SendByte(ad[0]);
	SPI_FLASH_SendByte(ad[1]);
	SPI_FLASH_SendByte(ad[2]);
	for(; no > 0; no--)
	   *re_buf_p++ = SPI_FLASH_SendByte(0xff);
	macSPI_FLASH_CS_DISABLE();


}

void getFlashtypeID(void)
{
	u8 i,*id;
	FLASH_CS(0);
	SPI_FLASH_SendByte(Read_Id);
	for(i=0;i<20;i++)
	{
		id[i]=SPI_FLASH_SendByte(0xff);
		delay_ms(100);
			LCD_ShowNum(0+i*20,3,id[i],12,24);
	}
	
	FLASH_CS(1);
	//LCD_ShowNum(30,210,id[i],12,24);
	//LCD_ShowNum(30,210,id[i],12,24);
		for(i=0;i<20;i++)
	{
		printf("MID= %d\t",id[i]);
		//LCD_ShowString(30,180,210,24,24,id[i]);
		//LCD_ShowNum(30,180,id[i],12,24);
	}
		 
}
//读取ID
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  macSPI_FLASH_CS_ENABLE();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(0x9f);  //W25X_JedecDeviceID

  /* Read a byte from the FLASH */
  Temp0 = SPI_FLASH_SendByte(0xff);

  /* Read a byte from the FLASH */
  Temp1 = SPI_FLASH_SendByte(0xff);

  /* Read a byte from the FLASH */
  Temp2 = SPI_FLASH_SendByte(0xff);

  /* Deselect the FLASH: Chip Select high */
  macSPI_FLASH_CS_DISABLE();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

void M25P80_DEEP_SLEEP(void)  //深度休眠
{
	M25P80_Write_En();
	
	FLASH_CS(0);
	delay_us(5);
	SPI_FLASH_SendByte( DEEP_SLEEP );
	FLASH_CS(1);
	
	LCD_ShowString(30,180,210,24,24,"M25P80_DEEP_SLEEP!!"); 
//	M25P80_CMD1B(DEEP_SLEEP);
	 delay_us(10);
//	while(M25P80_Busy()){}
//	M25P80_WP_En();
}


void M25P80_RES(void)
{
	M25P80_Write_En();
	M25P80_CMD1B(RES);
	while(M25P80_Busy());
	M25P80_WP_En();
		LCD_ShowString(30,180,210,24,24,"M25P80_RES!!"); 
}
