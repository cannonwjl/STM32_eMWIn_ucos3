#ifndef __DS1302_H
#define __DS1302_H
#include "sys.h"

//IO方向设置
#define DS1302_IO_IN()   {GPIOC->CRL&=0XFFFFFFF0;GPIOC->CRL|=8<<0;}  // DAT_MODE_IN()   // 
#define DS1302_IO_OUT()  {GPIOC->CRL&=0XFFFFFFF0;GPIOC->CRL|=3<<0;}  //DAT_MODE_OUT()  //


//IO方向设置
//#define DS1302_IO_IN()   DAT_MODE_IN()   // {GPIOC->CRL&=0XFFFFFFF0;GPIOC->CRL|=8<<(4*4);}
//#define DS1302_IO_OUT()  DAT_MODE_OUT()  //{GPIOC->CRL&=0XFFFFFFF0;GPIOC->CRL|=3<<(4*4);}


//IO操作函数									   
#define	DS1302_DATA_OUT PCout(0) //数据端口
#define	DS1302_DATA_IN  PCin(0)  //数据端口	

#define	DS1302_SCK  PCout(2)
#define	DS1302_RST  PCout(1)

//DS1302地址定义
#define ds1302_sec_add			  0x80		//秒数据地址
#define ds1302_min_add			  0x82		//分数据地址
#define ds1302_hr_add			    0x84		//时数据地址
#define ds1302_date_add			  0x86		//日数据地址
#define ds1302_month_add		  0x88		//月数据地址
#define ds1302_day_add			  0x8a		//星期数据地址
#define ds1302_year_add			  0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

void DS1302_Init(void);
void DS1302_Write_Byte(u8 addr, u8 data);
u8 DS1302_Read_Byte(u8 addr);
void DS1302_Write_Time(void);
void DS1302_Read_Time(void);
void DS1302_Get_Time(u8 *time);
void DAT_MODE_IN(void);
void DAT_MODE_OUT(void);
#endif
