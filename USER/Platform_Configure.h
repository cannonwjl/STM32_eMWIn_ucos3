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

enum GRPS_BACK_FLAG//���wifi�Ƿ�������·��
{
	No_Back=0,//û�з���
	BACK_Error=1,//���ش���
	BACK_Ok=2,//���غͶԱȵ���ȷ
};

enum RECE_BACK_DATA_CHECK//���ص����ݰ���� 
{
	RECEBack_ERROR=0,//û�з���
	RECEBack_Echo=1,//������ȷ��ע����ذ�
	RECEBack_Data=2,//������ȷ�����ݰ��ذ�
};

#define  EZ281





#define  DISPLAY_ENABLE





#endif
