#ifndef __PLATFORN_CONFIGURE_H  
#define __PLATFORN_CONFIGURE_H
//Note_WX  
//#include "stm32f10x.h"
//#include "ggsensor.h"
//#include "api.h"
//#include "LowRFM.h"
//#include "usart.h"	 
//#include "led.h"
//#include "delay.h"
//#include "eeprom.h"
//#include "system_platform.h"
#include "include.h"

#define Enable_GUI     1   /* Include code for Round-Robin scheduling                               */
#define Disable_GUI     0   /* Include code for Round-Robin scheduling                               */

typedef enum 
{
  NO = 0, 
  YES = !NO
} IsItRight;

enum GRPS_BACK_FLAG//检查wifi是否连接上路由
{
	No_Back=0,//没有返回
	BACK_Error=1,//返回错误
	BACK_Ok=2,//返回和对比的正确
};

enum RECE_BACK_DATA_CHECK//返回的数据包检查 
{
	RECEBack_ERROR=0,//没有返回
	RECEBack_Echo=1,//返回正确的注册包回包
	RECEBack_Data=2,//返回正确的数据包回包
};

#define  EZ281





#define  DISPLAY_ENABLE





#endif
