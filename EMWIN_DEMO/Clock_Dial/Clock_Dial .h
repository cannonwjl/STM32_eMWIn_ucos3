#ifndef __Clock_dial_H
#define __Clock_dial_H
#include "ili93xx.h"
#include "bmpdisplay.h"

#include "GUI.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include "WM.h"
#include "FRAMEWIN.h"
#include "bmpdisplay.h"
#include "timer.h"
#include "sht3x.h"
#include "rtc.h" 		    



extern float tem,hum;
#define Clock_Dial_num  2


#define countof(Obj) (sizeof(Obj) / sizeof(Obj[0]))
#define DEG2RAD      (3.1415926f / 180)
#define MAG          6
#define T_MAX   10000

#if Clock_Dial_num == 1  //界面1
    static const GUI_POINT _aNeedle[] = {
      { MAG * ( 0), MAG * (  0+50 ) },
      { MAG * (-3), MAG * (-10+50 ) },
      { MAG * (-3), MAG * (-40+50 ) },
      { MAG * ( 3), MAG * (-40+50 ) },
      { MAG * ( 3), MAG * (-10+50 ) },
    };

    static const GUI_POINT _aNeedle1[] = {
      { MAG * ( 0), MAG * (  0+45 ) },
      { MAG * (-3), MAG * (-10+45 ) },
      { MAG * (-3), MAG * (-30+45 ) },
      { MAG * ( 3), MAG * (-30+45 ) },
      { MAG * ( 3), MAG * (-10+45 ) },
    };

#endif


    
#if Clock_Dial_num == 2  //界面2 指针坐标需要修改

    static const GUI_POINT _aNeedle[] = {
      { MAG * ( 0), MAG * (  0+50 ) },
      { MAG * (-3), MAG * (-10+50 ) },
      { MAG * (-3), MAG * (-40+50 ) },
      { MAG * ( 3), MAG * (-40+50 ) },
      { MAG * ( 3), MAG * (-10+50 ) },
    };

    static const GUI_POINT _aNeedle1[] = {
      { MAG * ( 0), MAG * (  0+45 ) },
      { MAG * (-3), MAG * (-10+45 ) },
      { MAG * (-3), MAG * (-30+45 ) },
      { MAG * ( 3), MAG * (-30+45 ) },
      { MAG * ( 3), MAG * (-10+45 ) },
    };

#endif



/*
*********************************************************************************************************
*                                       类型定义
*********************************************************************************************************
*/ 
typedef struct {
  GUI_AUTODEV_INFO AutoDevInfo;  // Information about what has to be displayed  关于必须显示的内容的信息
  GUI_POINT        aPoints[7];   // Polygon data多边形数据
  float            Angle;
  
  GUI_POINT        aPoints1[7];   // Polygon data
  float            Angle1;
	
} PARAM;


static void _Draw(void * p); 
float map(float value,float fromStart,float fromEnd,float toStart,float toEnd);//将value的值从fromEnd-fromStart 映射到 toEnd-toStart

void Creat_device(float wen,float shi) ;
void MainTask(void);
void Draw_Qr_code(void);
void Time_Show(void);
//void GUI_QR_Delete(void);
void Draw_Qr_code(void);

#endif

